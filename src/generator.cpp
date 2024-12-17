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

int determineBit(cv::Vec3b pixel) {
  int blue = static_cast<int>(pixel[0]);
  int green = static_cast<int>(pixel[1]);
  int red = static_cast<int>(pixel[2]);
  int average = (blue + green + red) / 3;
  if (average >= 200) {
    return 1;
  } else if (average <= 40) {
    return 0;
  } else {
    return 2;
  }
}
std::bitset<8> vectorToBitset(const std::vector<int> &vec) {
  std::bitset<8> bits;
  for (size_t i = 0; i < vec.size() && i < bits.size(); ++i) { bits[i] = vec[i]; }
  return bits;
}

Generator::Generator(const std::string &fileName, const std::string &outputFileName, const std::string &password,
                     const int frameWidth, const int frameHeight, const int bitPixelRatio) {
  this->inputFileName = fileName;
  this->outputFileName = outputFileName;
  this->password = password;
  this->frameWidth = frameWidth;
  this->frameHeight = frameHeight;
  this->bitPixelRatio = bitPixelRatio;
}

void Generator::generate() {
  std::vector<fs::path> filePaths = file::getConvertFilePath(this->inputFileName, this->outputFileName);
  fs::path outputFilePath = filePaths[1].replace_extension(".ziply");
  Ende::compressAndEncrypt(filePaths[0], outputFilePath, this->password, 9);

  size_t chunk_size = ((this->frameHeight * this->frameWidth) / 8);
  std::ifstream file(outputFilePath, std::ios::binary);
  ThreadPool pool(std::thread::hardware_concurrency());
  if (!file) {
    throw Error("Could not open the file", "gen-file-open");
  }

  std::vector<char> buffer(chunk_size);
  int currentFrame = 0;
  fs::path outputDir =
      outputFilePath.parent_path() / std::string(outputFilePath.stem().string() + "_frames" +
                                                 std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                    std::chrono::system_clock::now().time_since_epoch())
                                                                    .count()));
  if (!fs::create_directories(outputDir)) {
    throw Error("Could not create the output directory", "gen-dir-create");
  }

  std::string inputFileExtension = filePaths[0].extension().string();
  std::vector<char> byteVector(inputFileExtension.begin(), inputFileExtension.end());

  std::string frameName = outputDir / (std::string("frame_") + std::to_string(currentFrame++) + ".png");
  std::streamsize bytes_read = byteVector.size();

  std::cout << "Generating frames..." << std::endl;

  pool.enqueue([this, buffer = std::vector<char>(byteVector), bytes_read, frameName]() {
    return convertToFrames(buffer, bytes_read, frameName);
  });

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
  pool.wait();
  std::cout << "Completed generating the frames ✅" << std::endl;

  // fs::remove(outputFilePath);
  std::cout << "Generating the final video using ffmpeg..." << std::endl;
  const std::string command = "ffmpeg -hwaccel auto -framerate 60 -i " + outputDir.string() +
                              "/frame_%d.png -c:v libx264 -pix_fmt yuv420p -preset veryslow -qp 0 " +
                              outputFilePath.replace_extension(".mp4").string();

  int result = std::system((command + " > /dev/null 2>&1").c_str());
  if (result != 0) {
    throw Error("Error occurred while generating the video", "gen-video-gen");
  }
  std::cout << "Completed generating the video ✅" << std::endl;

  // fs::remove_all(outputDir);
}

std::future<void> Generator::convertToFrames(const std::vector<char> buffer, std::streamsize bytes_read,
                                             const std::string frameName) {
  std::unique_ptr<unsigned char[]> image(new unsigned char[this->frameWidth * this->frameHeight * 3]);
  size_t currentPixelIndex = 0;
  size_t totalPixelsToWrite = bytes_read * 8;

  for (size_t i = 0; i < bytes_read; ++i) {
    std::bitset<8> currentByte(buffer[i]);
    for (int j = 0; j < 8; ++j) {
      if (currentByte[j]) {
        image[currentPixelIndex++] = 255;
        image[currentPixelIndex++] = 255;
        image[currentPixelIndex++] = 255;
      } else {
        image[currentPixelIndex++] = 0;
        image[currentPixelIndex++] = 0;
        image[currentPixelIndex++] = 0;
      }
    }
  }

  while (currentPixelIndex < this->frameWidth * this->frameHeight * 3) {
    image[currentPixelIndex++] = 255;
    image[currentPixelIndex++] = 0;
    image[currentPixelIndex++] = 0;
  }

  std::future<void> future = std::async(std::launch::async, [frameName, img = std::move(image), this]() {
    stbi_write_png(frameName.c_str(), this->frameWidth, this->frameHeight, 3, img.get(), this->frameWidth * 3);
  });
  return future;
}

void Generator::restore() {
  std::vector<fs::path> filePaths = file::getRestoreFilePath(this->inputFileName, this->outputFileName);
  fs::path inputFilePath = filePaths[0];
  fs::path outputFilePath = filePaths[1].replace_extension(".ziply");

  if (!fs::exists(inputFilePath)) {
    throw Error("Input file does not exist", "gen-file-not-exist");
  }

  if (!fs::is_regular_file(inputFilePath)) {
    throw Error("Input file is not a regular file", "gen-file-not-regular");
  }

  if (inputFilePath.extension() != ".mp4") {
    throw Error("Input file is not a video", "gen-file-not-video");
  }

  fs::path outputDir =
      outputFilePath.parent_path() / std::string(outputFilePath.stem().string() + "_frames" +
                                                 std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                    std::chrono::system_clock::now().time_since_epoch())
                                                                    .count()));

  if (!fs::create_directories(outputDir)) {
    throw Error("Could not create the output directory", "gen-dir-create");
  }
  std::string command = "ffmpeg -hwaccel auto -i " + inputFilePath.string() + " -vf fps=60 " +
                        (outputDir / fs::path("frame_%d.png")).string();

  std::system((command + " > /dev/null 2>&1").c_str());

  std::vector<fs::path> frames;

  for (const auto &entry : fs::directory_iterator(outputDir)) { frames.push_back(entry); }

  auto extensionFrame =
      std::find_if(frames.begin(), frames.end(), [](const fs::path &frame) { return frame.stem() == "frame_1.png"; });

  if (extensionFrame != frames.end()) {
    throw Error("Could not find the frame containing file details.", "gen-frame-err");
  }

  std::sort(frames.begin(), frames.end(), [](const fs::path &a, const fs::path &b) {
    int numA = std::stoi(a.stem().string().substr(6));
    int numB = std::stoi(b.stem().string().substr(6));
    return numA < numB;
  });

  cv::Mat image = cv::imread(frames[0]);

  if (image.empty()) {
    throw Error("Could not open the image at: " + frames[0].string(), "gen-restore-empty-image");
  }
  this->frameHeight = image.rows;
  this->frameWidth = image.cols;
  std::vector<char> extensionVector = restoreFrameData(frames[0]);

  std::string fileExtension(extensionVector.begin(), extensionVector.end());
  std::cout << "File Extension << " << fileExtension << " -  size :" << fileExtension.size() << std::endl;

  std::cout << "Restoring data from frames..." << std::endl;
  return;
  ThreadPool pool(std::thread::hardware_concurrency());
  fs::path fragDir =
      outputFilePath.parent_path() / std::string(outputFilePath.stem().string() + "_ziply_frags" +
                                                 std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                    std::chrono::system_clock::now().time_since_epoch())
                                                                    .count()));
  if (!fs::create_directories(fragDir)) {
    throw Error("Could not create directory to store fragment data.", "ziply-err-dir");
  }
  for (int i = 1; i < frames.size(); i++) {
    std::string currentFramePath = frames[i];
    fs::path currentZiplyPath = fragDir / std::string("ziply_frame_" + std::to_string(i) + ".zfrag");
    pool.enqueue([this, framePath = currentFramePath, ziplyFragPath = currentZiplyPath]() {
      return writeFramesToZiplyFrag(framePath, ziplyFragPath);
    });
  }

  pool.wait();
  // fs::remove_all(outputDir);
  std::cout << "Completed restoring data from frames ✅" << std::endl;

  std::cout << "Generating ziply file..." << std::endl;

  std::ofstream outFile(outputFilePath, std::ios::binary);
  if (!outFile) {
    throw Error("Failed to open output file: " + outputFilePath.string(), "ziply-err-open-output");
  }
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
  std::cout << "Completed generating ziply file ✅" << std::endl;
  fs::path finalFilePath = outputFilePath;
  finalFilePath.replace_extension(fileExtension);
  Ende::decompressAndDecrypt(outputFilePath, finalFilePath, this->password);
}

std::vector<char> Generator::restoreFrameData(const std::string framePath) {
  cv::Mat image = cv::imread(framePath);

  if (image.empty()) {
    throw Error("Could not open an image at: " + framePath, "gen-restore-empty-frame");
  }
  int currentX = 0;
  int currentY = 0;
  std::vector<int> currentByte;
  std::vector<std::vector<int>> currentFrameData;
  while (true) {
    cv::Vec3b currentPixel = image.at<cv::Vec3b>(currentY, currentX);
    int currentBit = determineBit(currentPixel);
    if (currentBit == 2) {
      if (currentByte.size() != 8 && currentByte.size() != 0) {
        for (int i = 0; i < (8 - currentByte.size()); i++) {
          cv::Vec3b pixel = image.at<cv::Vec3b>(currentY, currentX);
          int blue = static_cast<int>(pixel[0]);
          int green = static_cast<int>(pixel[1]);
          int red = static_cast<int>(pixel[2]);
          if (blue <= 100) {
            currentByte.push_back(0);
          } else {
            currentByte.push_back(1);
          }
          currentX++;
          if (currentX >= this->frameWidth) {
            currentX = 0;
            currentY++;
          }
          if (currentY >= this->frameHeight) {
            break;
          }
        }
      }
      currentFrameData.push_back(currentByte);
      currentByte.clear();
      break;
    } else {
      currentByte.push_back(currentBit);
      if (currentByte.size() == 8) {
        currentFrameData.push_back(currentByte);
        currentByte.clear();
      }
    }
    currentX++;
    if (currentX >= this->frameWidth) {
      currentX = 0;
      currentY++;
    }
    if (currentY >= this->frameHeight) {
      break;
    }
  }
  std::vector<char> frameCharVector;
  for (const std::vector<int> &byteData : currentFrameData) {
    std::bitset<8> bits = vectorToBitset(byteData);
    char character = static_cast<unsigned char>(bits.to_ulong());
    frameCharVector.push_back(character);
  }
  std::cout << "Frame Bytes: " << currentFrameData.size() << std::endl;
  std::cout << "File Bytes: " << frameCharVector.size() << std::endl;
  return frameCharVector;
}
std::future<void> Generator::writeFramesToZiplyFrag(const std::string framePath, const fs::path ziplyFragPath) {
  std::vector<char> currentFrameData = restoreFrameData(framePath);
  return std::async(std::launch::async, [currentFrameData, ziplyFragPath]() {
    std::ofstream outFile(ziplyFragPath, std::ios::binary);
    if (!outFile) {
      std::cout << "Could not create frag file" << ziplyFragPath;
      throw Error("Could not open the output file for writing", "ziply-file-open");
    }
    outFile.write(currentFrameData.data(), currentFrameData.size());
    outFile.close();
  });
}
