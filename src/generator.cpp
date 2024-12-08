#include "generator.hpp"

Generator::Generator(std::string inputFileName, std::string outputFileName, std::string password, std::string processingMode, int frameWidth, int frameHeight, float bitPixelRatio)
{
    this->inputFileName = inputFileName;
    this->outputFileName = outputFileName;
    this->password = password;
    this->processingMode = processingMode;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    this->bitPixelRatio = bitPixelRatio;
}

void Generator::generate()
{

}