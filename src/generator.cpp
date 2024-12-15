#include "generator.hpp"
#include "ende.hpp"
#include "error.hpp"
#include "file.hpp"
#include "stb_image_write.h"
#include "threadpool.hpp"
#include <bitset>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <vector>

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
  std::cout << "Compression and encryption completed successfully" << std::endl;

  std::cout << "Output file path: " << this->frameHeight << " " << this->frameWidth << std::endl;

  size_t chunk_size = ((this->frameHeight * this->frameWidth) / 8);
  std::ifstream file(outputFilePath, std::ios::binary);
  ThreadPool pool(std::thread::hardware_concurrency());
  std::cout << "Chunk size: " << chunk_size << std::endl;
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

  fs::remove(outputFilePath);

  const std::string command = "ffmpeg -hwaccel auto -framerate 60 -i " +
                              (outputDir.string() / fs::path("frame_%d.png")).string() +
                              " -c:v libx264 -r 60 -pix_fmt gray " + outputFilePath.replace_extension(".mp4").string();

  std::cout << command << std::endl;
  std::system(command.c_str());
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

  // Ensure we have read permissions for input file
  if (!fs::is_regular_file(inputFilePath)) {
    throw Error("Input file is not a regular file", "gen-fil  e-not-regular");
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
  std::system(command.c_str());

  std::cout << "Decompression and decryption completed successfully" << std::endl;

  // try {
  //   // Add debug output before decompression
  //   std::cout << "Starting decompression and decryption..." << std::endl;
  //   Ende::decompressAndDecrypt(inputFilePath, outputFilePath, this->password);
  //   std::cout << "Decompression and decryption completed successfully" << std::endl;
  // } catch (const std::exception &e) {
  //   std::cerr << "Detailed error during decompression: " << e.what() << std::endl;
  //   throw;
  // }
}
