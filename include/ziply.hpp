#ifndef ZIPLY_HPP
#define ZIPLY_HPP

#include <string>

#include "file.hpp"
#include "error.hpp"

class Ziply
{
private:
    std::string fileName;
    std::string outputFileName;
    int frameHeight, frameWidth;
    float bitPixelRatio;
    std::string password;
    std::string processingMode;

public:
    Ziply(const std::string &fileName, const std::string &outputFileName, const std::string &password, const std::string &processingMode, const int frameWidth, const int frameHeight, float bitPixelRatio);

    void convert();
    void restore();
};

#endif