#ifndef ZIPLY_HPP
#define ZIPLY_HPP

#include <string>

#include "file.hpp"
#include "error.hpp"

class Ziply
{
private:
    std::string fileName;
    std::string filePath;
    int frameHeight, frameWidth;

public:
    Ziply(const std::string &fileName, const int frameWidth, const int frameHeight);

};

#endif