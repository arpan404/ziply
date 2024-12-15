#ifndef FILE_HPP
#define FILE_HPP

#include "error.hpp"
#include "stb_image_write.h"
#include <bitset>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>

namespace fs = std::filesystem;

class File {
public:
  File(std::string fileName, int frameWidth, int frameHeight);
  void convertToFrames();

private:
  std::string fileName;
  std::string filePath;
  int frameWidth;
  int frameHeight;
  bool pathExists(const std::string &path);
  fs::path convertToPath(const std::string &fileName);

  void generateFrames(std::vector<char> buffer, std::streamsize bytes_read, std::string frameName);
};

#endif