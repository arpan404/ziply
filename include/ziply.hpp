#ifndef ZIPLY_HPP
#define ZIPLY_HPP

#include <string>

#include "file.hpp"
#include "error.hpp"

class Ziply
{
private:
    std::string *fileName;
    std::string *filePath;
    int *frameHeight, *frameWidth;

public:
    Ziply(std::string fileName, int frameWidth, int frameHeight);
    ~Ziply();
};

#endif