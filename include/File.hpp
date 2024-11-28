#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <filesystem>
#include <fstream>
#include <bitset>
// #include <Magick++.h>
#include <thread>
#include "Error.hpp"
#include "stb_image_write.h"

namespace fs = std::filesystem;

class File
{
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