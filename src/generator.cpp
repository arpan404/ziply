#include "generator.hpp"
#include "ende.hpp"
#include "error.hpp"
#include "file.hpp"
#include "stb_image_write.h"
#include "threadpool.hpp"
#include <algorithm>
#include <bitset>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

// Function to determine the bit value based on the pixel's average color intensity
int determineBit(cv::Vec3b pixel) {
  int blue = static_cast<int>(pixel[0]);
  int green = static_cast<int>(pixel[1]);
  int red = static_cast<int>(pixel[2]);
  int average = (blue + green + red) / 3; // Calculate the average of RGB values

  // Return 1 for bright pixels, 0 for dark pixels, 2 for neutral pixels
  if (average >= 200) {
    return 1;
  } else if (average <= 40) {
    return 0;
  } else {
    return 2;
  }
}

// Converts a vector of integers into an 8-bit bitset for further processing
std::bitset<8> vectorToBitset(const std::vector<int> &vec) {
  std::bitset<8> bits;
  for (size_t i = 0; i < vec.size() && i < bits.size(); ++i) { bits[i] = vec[i]; }
  return bits;
}

// Constructor for the Generator class
Generator::Generator(const std::string &fileName, const std::string &outputFileName, const std::string &password,
                     const int frameWidth, const int frameHeight, const int bitPixelRatio) {
  this->inputFileName = fileName;
  this->outputFileName = outputFileName;
  this->password = password;
  this->frameWidth = frameWidth;
  this->frameHeight = frameHeight;
  this->bitPixelRatio = bitPixelRatio;
}

// Main function to generate the video from the file
void Generator::generate() {
  // Get file paths for input and output
  std::vector<fs::path> filePaths = file::getConvertFilePath(this->inputFileName, this->outputFileName);
  fs::path outputFilePath = filePaths[1].replace_extension(".ziply");

  // Compress and encrypt the file before processing
  Ende::compressAndEncrypt(filePaths[0], outputFilePath, this->password, 9);

  // Calculate the chunk size for processing frames
  size_t chunk_size = ((this->frameHeight * this->frameWidth) / 8);
  std::ifstream file(outputFilePath, std::ios::binary);

  if (!file) {
    throw Error("Could not open the file", "gen-file-open");
  }

  // Prepare buffer for reading chunks of the file
  std::vector<char> buffer(chunk_size);
  int currentFrame = 0;

  // Create an output directory to store frames
  fs::path outputDir =
      outputFilePath.parent_path() / std::string(outputFilePath.stem().string() + "_frames" +
                                                 std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                    std::chrono::system_clock::now().time_since_epoch())
                                                                    .count()));
  if (!fs::create_directories(outputDir)) {
    throw Error("Could not create the output directory", "gen-dir-create");
  }

  // Convert the file extension to a vector for further processing
  std::string inputFileExtension = filePaths[0].extension().string();
  std::vector<char> byteVector(inputFileExtension.begin(), inputFileExtension.end());

  // Generate the first frame
  std::string frameName = outputDir / (std::string("frame_") + std::to_string(currentFrame++) + ".png");
  std::streamsize bytes_read = byteVector.size();

  std::cout << "Generating frames..." << std::endl;

  // Create a thread pool to handle the frame processing in parallel
  ThreadPool pool(std::thread::hardware_concurrency());

  // Enqueue the frame generation task for the first frame
  pool.enqueue([this, buffer = std::vector<char>(byteVector), bytes_read, frameName]() {
    return convertToFrames(buffer, bytes_read, frameName);
  });

  // Process the file in chunks and generate frames
  while (file) {
    file.read(buffer.data(), buffer.size());
    std::streamsize bytes_read = file.gcount();
    if (bytes_read > 0) {
      std::string frameName = outputDir / (std::string("frame_") + std::to_string(currentFrame++) + ".png");
      pool.enqueue([this, buffer = std::vector<char>(buffer), bytes_read, frameName]() {
        return convertToFrames(buffer, bytes_read, frameName);
      });
    }
  }

  // Wait for all frame generation tasks to complete
  pool.wait();
  std::cout << "Completed generating the frames ✅" << std::endl;

  // Remove the original compressed file
  fs::remove(outputFilePath);

  // Use ffmpeg to generate the final video from the frames
  std::cout << "Generating the final video using ffmpeg..." << std::endl;
  const std::string command = "ffmpeg -hwaccel auto -framerate 60 -i " + outputDir.string() +
                              "/frame_%d.png -c:v libx264 -pix_fmt yuv420p -preset veryslow -qp 0 " +
                              outputFilePath.replace_extension(".mp4").string();

  int result = std::system((command + " > /dev/null 2>&1").c_str());
  if (result != 0) {
    throw Error("Error occurred while generating the video", "gen-video-gen");
  }

  std::cout << "Completed generating the video ✅" << std::endl;

  // Clean up frame files
  fs::remove_all(outputDir);
}

// Function to convert the buffered data into individual frames and save them
std::future<void> Generator::convertToFrames(const std::vector<char> buffer, std::streamsize bytes_read,
                                             const std::string frameName) {
  // Prepare an image buffer with the specified dimensions
  std::unique_ptr<unsigned char[]> image(new unsigned char[this->frameWidth * this->frameHeight * 3]);
  size_t currentPixelIndex = 0;
  size_t totalPixelsToWrite = bytes_read * 8;

  // Process each byte and set the corresponding pixels to black or white based on the bit value
  for (size_t i = 0; i < bytes_read; ++i) {
    std::bitset<8> currentByte(buffer[i]);
    for (int j = 0; j < 8; ++j) {
      if (currentByte[j]) {
        image[currentPixelIndex++] = 255; // White pixel
        image[currentPixelIndex++] = 255; // White pixel
        image[currentPixelIndex++] = 255; // White pixel
      } else {
        image[currentPixelIndex++] = 0; // Black pixel
        image[currentPixelIndex++] = 0; // Black pixel
        image[currentPixelIndex++] = 0; // Black pixel
      }
    }
  }

  // Fill the remaining pixels with a specific color (red) to maintain image size
  while (currentPixelIndex < this->frameWidth * this->frameHeight * 3) {
    image[currentPixelIndex++] = 255;
    image[currentPixelIndex++] = 0;
    image[currentPixelIndex++] = 0;
  }

  // Asynchronously write the frame to a PNG file
  std::future<void> future = std::async(std::launch::async, [frameName, img = std::move(image), this]() {
    stbi_write_png(frameName.c_str(), this->frameWidth, this->frameHeight, 3, img.get(), this->frameWidth * 3);
  });
  return future;
}
void Generator::restore() {
  // Retrieve file paths for input and output files
  std::vector<fs::path> filePaths = file::getRestoreFilePath(this->inputFileName, this->outputFileName);
  fs::path inputFilePath = filePaths[0];
  fs::path outputFilePath = filePaths[1].replace_extension(".ziply");

  // Check if the input file exists and is a regular .mp4 file
  if (!fs::exists(inputFilePath)) {
    throw Error("Input file does not exist", "gen-file-not-exist");
  }

  if (!fs::is_regular_file(inputFilePath)) {
    throw Error("Input file is not a regular file", "gen-file-not-regular");
  }

  if (inputFilePath.extension() != ".mp4") {
    throw Error("Input file is not a video", "gen-file-not-video");
  }

  // Create a new output directory to store frames extracted from the video
  fs::path outputDir =
      outputFilePath.parent_path() / std::string(outputFilePath.stem().string() + "_frames" +
                                                 std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                    std::chrono::system_clock::now().time_since_epoch())
                                                                    .count()));

  // Ensure the output directory is created, else throw an error
  if (!fs::create_directories(outputDir)) {
    throw Error("Could not create the output directory", "gen-dir-create");
  }

  // Construct the command to extract frames from the video using ffmpeg
  std::string command = "ffmpeg -hwaccel auto -i " + inputFilePath.string() + " -vf fps=60 " +
                        (outputDir / fs::path("frame_%d.png")).string();

  // Execute the ffmpeg command to extract frames, redirecting the output to /dev/null to suppress logs
  std::system((command + " > /dev/null 2>&1").c_str());

  // Collect all the extracted frame files into a vector
  std::vector<fs::path> frames;

  // Iterate over the directory to store the frames in the vector
  for (const auto &entry : fs::directory_iterator(outputDir)) { frames.push_back(entry); }

  // Ensure that the first frame exists and contains the required metadata
  auto extensionFrame = std::find_if(frames.begin(), frames.end(),
                                     [](const fs::path &frame) { return frame.stem().string() == "frame_1"; });

  if (extensionFrame == frames.end()) {
    throw Error("Could not find the frame containing file details.", "gen-frame-err");
  }

  // Sort the frames based on their numeric order extracted from the filenames
  std::sort(frames.begin(), frames.end(), [](const fs::path &a, const fs::path &b) {
    int numA = std::stoi(a.stem().string().substr(6));
    int numB = std::stoi(b.stem().string().substr(6));
    return numA < numB;
  });
  // Read the first frame to extract image metadata
  cv::Mat image = cv::imread(frames[0]);

  // If the image cannot be opened, throw an error
  if (image.empty()) {
    throw Error("Could not open the image at: " + frames[0].string(), "gen-restore-empty-image");
  }

  // Set frame width and height from the first frame
  this->frameHeight = image.rows;
  this->frameWidth = image.cols;

  // Restore frame data from the first frame
  std::vector<char> extensionVector = restoreFrameData(frames[0]);

  // Convert the frame data into a string to determine the file extension
  std::string fileExtension(extensionVector.begin(), extensionVector.end());

  std::cout << "Restoring data from frames..." << std::endl;

  // Create a thread pool to handle the frame processing in parallel
  ThreadPool pool(std::thread::hardware_concurrency());

  // Create a directory to store fragmented ziply data
  fs::path fragDir =
      outputFilePath.parent_path() / std::string(outputFilePath.stem().string() + "_ziply_frags" +
                                                 std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                    std::chrono::system_clock::now().time_since_epoch())
                                                                    .count()));

  // Ensure the fragment directory is created, else throw an error
  if (!fs::create_directories(fragDir)) {
    throw Error("Could not create directory to store fragment data.", "ziply-err-dir");
  }

  // Enqueue tasks to process each frame in parallel and save them as ziply fragments
  for (int i = 1; i < frames.size(); i++) {
    std::string currentFramePath = frames[i];
    fs::path currentZiplyPath = fragDir / std::string("ziply_frame_" + std::to_string(i) + ".zfrag");
    pool.enqueue([this, framePath = currentFramePath, ziplyFragPath = currentZiplyPath]() {
      return writeFramesToZiplyFrag(framePath, ziplyFragPath);
    });
  }

  // Wait for all frame processing tasks to complete
  pool.wait();

  // Clean up by removing the extracted frames directory
  fs::remove_all(outputDir);
  std::cout << "Completed restoring data from frames ✅" << std::endl;

  std::cout << "Generating ziply file..." << std::endl;

  // Open the output ziply file for writing in binary mode
  std::ofstream outFile(outputFilePath, std::ios::binary);
  if (!outFile) {
    throw Error("Failed to open output file: " + outputFilePath.string(), "ziply-err-open-output");
  }

  // Write each ziply fragment into the output ziply file
  for (int i = 1; i < frames.size(); i++) {
    fs::path currentZiplyFragPath = fragDir / std::string("ziply_frame_" + std::to_string(i) + ".zfrag");
    std::ifstream ziplyFragFile(currentZiplyFragPath, std::ios::binary);
    if (!ziplyFragFile) {
      throw Error("Failed to open ziply fragment file: " + currentZiplyFragPath.string(), "ziply-err-open");
    }

    outFile << ziplyFragFile.rdbuf();
    if (ziplyFragFile.fail()) {
      throw Error("Failed to read from ziply fragment file: " + currentZiplyFragPath.string(), "ziply-err-read");
    }
    ziplyFragFile.close();
  }

  // Close the output file after writing all fragments
  outFile.close();
  std::cout << "Completed generating ziply file ✅" << std::endl;

  // Convert the generated ziply file back to the original file format
  std::cout << "Converting ziply file to original file..." << std::endl;
  fs::path finalFilePath = outputFilePath;
  finalFilePath.replace_extension(fileExtension); // Restore original file extension

  // Decompress and decrypt the file
  Ende::decompressAndDecrypt(outputFilePath, finalFilePath, this->password);

  // Clean up by removing the ziply file and fragments directory
  fs::remove(outputFilePath);
  fs::remove_all(fragDir);

  std::cout << "Completed restoring the original file ✅" << std::endl
            << "File saved at: " << finalFilePath << std::endl;
}

/// This function restores frame data from a specified image file.
// It reads pixel data from the image, processes it, and converts it into a vector of characters representing the frame
// data.
std::vector<char> Generator::restoreFrameData(const std::string framePath) {
  // Load the image from the given frame path using OpenCV.
  cv::Mat image = cv::imread(framePath);

  // Check if the image was successfully loaded; if not, throw an error.
  if (image.empty()) {
    throw Error("Could not open an image at: " + framePath, "gen-restore-empty-frame");
  }

  // Initialize coordinates for pixel traversal.
  int currentX = 0;
  int currentY = 0;

  // Vectors to hold the current byte and the complete frame data.
  std::vector<int> currentByte;
  std::vector<std::vector<int>> currentFrameData;

  // Loop to traverse the image pixels and extract data encoded in the image.
  while (true) {
    // Get the current pixel's color values (Blue, Green, Red).
    cv::Vec3b currentPixel = image.at<cv::Vec3b>(currentY, currentX);

    // Determine the bit value (0, 1, or 2 for end of byte) based on the pixel's color.
    int currentBit = determineBit(currentPixel);

    // Check if the current bit indicates the end of a byte (2 means end of byte).
    if (currentBit == 2) {
      // If the current byte is incomplete (not 8 bits), complete it using the next pixels.
      if (currentByte.size() != 8 && currentByte.size() != 0) {
        // Fill the byte with missing bits from subsequent pixels.
        while (currentByte.size() < 8) {
          // Get the next pixel's color values.
          cv::Vec3b pixel = image.at<cv::Vec3b>(currentY, currentX);
          // Check the blue channel value to determine the bit (0 or 1).
          int blue = static_cast<int>(pixel[0]);
          currentByte.push_back(blue <= 100 ? 0 : 1);
          currentX++; // Move to the next pixel horizontally.

          // If the end of the row is reached, move to the next row.
          if (currentX >= this->frameWidth) {
            currentX = 0;
            currentY++;
          }
          // Exit if we've processed all pixels.
          if (currentY >= this->frameHeight) {
            break;
          }
        }
      }
      // If the byte is complete (8 bits), add it to the frame data and reset the byte vector.
      if (currentByte.size() == 8) {
        currentFrameData.push_back(currentByte);
        currentByte.clear();
      }
      break; // Exit loop after processing the end byte.
    } else {
      // Add the valid bit (either 0 or 1) to the current byte.
      currentByte.push_back(currentBit);

      // If the byte is complete (8 bits), add it to the frame data and reset the byte vector.
      if (currentByte.size() == 8) {
        currentFrameData.push_back(currentByte);
        currentByte.clear();
      }
    }

    // Move to the next pixel.
    currentX++;

    // If the end of the row is reached, wrap to the next row.
    if (currentX >= this->frameWidth) {
      currentX = 0;
      currentY++;
    }

    // Exit if we've reached the end of the image.
    if (currentY >= this->frameHeight) {
      break;
    }
  }

  // Convert the collected frame data (bytes) into a vector of characters.
  std::vector<char> frameCharVector;
  for (const std::vector<int> &byteData : currentFrameData) {
    // Convert byte data (vector of bits) into a bitset (8 bits).
    std::bitset<8> bits = vectorToBitset(byteData);
    // Convert the bitset into a character (unsigned char).
    char character = static_cast<unsigned char>(bits.to_ulong());
    // Add the character to the output vector.
    frameCharVector.push_back(character);
  }
  // Return the restored frame data as a vector of characters.
  return frameCharVector;
}

// Asynchronously writes the frame data to a specified Ziply fragment file.
// This function restores the frame data from the image and writes it to the given Ziply fragment file.
std::future<void> Generator::writeFramesToZiplyFrag(const std::string framePath, const fs::path ziplyFragPath) {
  // Restore the frame data from the specified frame image file.
  std::vector<char> currentFrameData = restoreFrameData(framePath);

  // Launch an asynchronous task to write the frame data to the specified Ziply fragment file.
  return std::async(std::launch::async, [currentFrameData, ziplyFragPath]() {
    // Open the output file in binary mode to write the frame data.
    std::ofstream outFile(ziplyFragPath, std::ios::binary);

    // Check if the file was successfully opened; if not, throw an error.
    if (!outFile) {
      throw Error("Could not open the output file for writing", "ziply-file-open");
    }

    // Write the frame data to the Ziply fragment file.
    outFile.write(currentFrameData.data(), currentFrameData.size());

    // Close the output file after writing.
    outFile.close();
  });
}

