#include "ziply.hpp"

Ziply::Ziply(const std::string &fileName, const std::string &outputFileName, const std::string &password, const std::string &processingMode, const int frameWidth, const int frameHeight, float bitPixelRatio) 
    : fileName(fileName), outputFileName(outputFileName), password(password), processingMode(processingMode), frameWidth(frameWidth), frameHeight(frameHeight), bitPixelRatio(bitPixelRatio) {}
