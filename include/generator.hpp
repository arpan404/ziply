#ifndef GENERATOR_HPP
#define GENERATOR_HPP

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

  std::future<void> writeFramesToZiply(const std::vector<char> data, const std::string framePath);
};

#endif