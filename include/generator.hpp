#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <string>

class Generator
{
private:
    std::string inputFileName;
    std::string outputFileName;
    std::string password;
    std::string processingMode;
    int frameWidth;
    int frameHeight;
    float bitPixelRatio;

public:
    Generator(const std::string &fileName, const std::string &outputFileName, const std::string &password, const std::string &processingMode, const int frameWidth, const int frameHeight, const float bitPixelRatio);
    void generate();
    void restore();
};

#endif