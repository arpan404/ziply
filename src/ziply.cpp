#include "ziply.hpp"
#include "generator.hpp"


Ziply::Ziply(const std::string &fileName, const std::string &outputFileName, const std::string &password, const std::string &processingMode, const int frameWidth, const int frameHeight, float bitPixelRatio)
    : fileName(fileName), outputFileName(outputFileName), password(password), processingMode(processingMode), frameWidth(frameWidth), frameHeight(frameHeight), bitPixelRatio(bitPixelRatio) {}

void Ziply::convert()
{
    Generator generator(this->fileName, this->outputFileName, this->password, this->processingMode, this->frameWidth, this->frameHeight, this->bitPixelRatio);
    generator.generate();
}

void Ziply::restore()
{
    Generator generator(this->fileName, this->outputFileName, this->password, this->processingMode, this->frameWidth, this->frameHeight, this->bitPixelRatio);
    generator.restore();
}