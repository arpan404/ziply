#include "generator.hpp"
#include "ende.hpp"
#include "file.hpp"
#include "stb_image_write.h"
#include "threadpool.hpp"
#include <bitset>
#include <fstream>
#include <future>
#include <iostream>

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
    throw std::runtime_error("Could not open the file");
  }

  std::vector<char> buffer(chunk_size);
  std::cout << "Buffer size: " << buffer.size() << std::endl;
  int currentFrame = 0;

  while (file) {
    file.read(buffer.data(), buffer.size());
    std::streamsize bytes_read = file.gcount();
    if (bytes_read > 0) {
      std::string frameName = std::to_string(currentFrame++) + ".png";
      pool.enqueue([this, buffer = std::vector<char>(buffer), bytes_read, frameName]() {
        convertToFrames(buffer, bytes_read, frameName);
      });
    }
  }
}

void Generator::convertToFrames(const std::vector<char> buffer, std::streamsize bytes_read,
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

  auto future = std::async(std::launch::async, [frameName, img = std::move(image), this]() {
    stbi_write_png(frameName.c_str(), this->frameWidth, this->frameHeight, 3, img.get(), this->frameWidth * 3);
  });

  future.get();
}

void Generator::restore() {
  std::vector<fs::path> filePaths = file::getConvertFilePath(this->inputFileName, this->outputFileName);
  fs::path inputFilePath = filePaths[0];
  fs::path outputFilePath = filePaths[1].replace_extension(".png");

  if (!fs::exists(inputFilePath)) {
    std::cerr << "Error: Input file '" << inputFilePath << "' does not exist." << std::endl;
    throw std::runtime_error("Input file not found");
  }

  // Ensure we have read permissions for input file
  if (!fs::is_regular_file(inputFilePath)) {
    std::cerr << "Error: '" << inputFilePath << "' is not a regular file." << std::endl;
    throw std::runtime_error("Invalid input file");
  }

  std::cout << inputFilePath << std::endl;

  try {
    // Add debug output before decompression
    std::cout << "Starting decompression and decryption..." << std::endl;
    Ende::decompressAndDecrypt(inputFilePath, outputFilePath, this->password);
    std::cout << "Decompression and decryption completed successfully" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Detailed error during decompression: " << e.what() << std::endl;
    throw;
  }
}
