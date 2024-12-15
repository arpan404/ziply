#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <string>
#include <future>
#include <vector>
#include <string>
#include <fstream>
#include "file.hpp"

class Generator
{
private:
    std::string inputFileName;
    std::string outputFileName;
    std::string password;
    int frameWidth;
    int frameHeight;
    int bitPixelRatio;
    void convertToFrames(std::vector<char> &buffer, std::streamsize &bytes_read, std::string &frameName);

public:
    Generator(const std::string &fileName, const std::string &outputFileName, const std::string &password, const int frameWidth, const int frameHeight, const int bitPixelRatio);
    void generate();
    void restore();
};

#endif