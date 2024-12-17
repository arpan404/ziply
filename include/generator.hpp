#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "file.hpp"
#include <future>
#include <iostream>
#include <string>
#include <vector>

class Generator {
public:
  Generator(const std::string &fileName, const std::string &outputFileName, const std::string &password,
            const int frameWidth, const int frameHeight, const int bitPixelRatio);
  void generate();
  void restore();

private:
  std::string inputFileName;
  std::string outputFileName;
  std::string password;
  int frameWidth;
  int frameHeight;
  int bitPixelRatio;
  std::future<void> convertToFrames(const std::vector<char> buffer, std::streamsize bytes_read,
                                    const std::string frameName);
  std::vector<char> restoreFrameData(const std::string framePath);

  std::future<void> writeFramesToZiplyFrag(const std::string framePath, const fs::path ziplyFragPath);
};

#endif