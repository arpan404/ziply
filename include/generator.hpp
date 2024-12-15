#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <future>
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
  void convertToFrames(const std::vector<char> buffer, std::streamsize bytes_read, const std::string frameName);
};

#endif