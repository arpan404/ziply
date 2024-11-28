#include <iostream>
#include "ziply.hpp"
#include "parser.hpp"

int main(int argc, char *argv[])
{
    std::string fileName;
    std::string outputFileName;
    std::string password;
    std::string processingMode;
    int frameHeight, frameWidth;
    float compressionPrevention;

    // pointers
    std::string *const fileNamePtr = &fileName;
    std::string *const outputFileNamePtr = &outputFileName;
    std::string *const passwordPtr = &password;
    std::string *const processingModePtr = &processingMode;
    int *const frameHeightPtr = &frameHeight;
    int *const frameWidthPtr = &frameWidth;
    float *const compressionPreventionPtr = &compressionPrevention;

    Parser parser;

    parser.parse(arc, argv, fileNamePtr, outputFileNamePtr, passwordPtr, frameHeightPtr, frameWidthPtr, compressionPreventionPtr)
    
}