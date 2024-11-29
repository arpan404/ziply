#include <iostream>
#include "ziply.hpp"
#include "parser.hpp"
#include "error.hpp"

int main(int argc, char *argv[])
{
    try
    {
        std::string fileName;
        std::string outputFileName;
        std::string password = "ziplySecret";
        std::string processingMode;
        int frameHeight, frameWidth;
        float compressionPrevention;
        bool isRestoring;

        // pointers
        std::string *const fileNamePtr = &fileName;
        std::string *const outputFileNamePtr = &outputFileName;
        std::string *const passwordPtr = &password;
        std::string *const processingModePtr = &processingMode;
        int *const frameHeightPtr = &frameHeight;
        int *const frameWidthPtr = &frameWidth;
        float *const compressionPreventionPtr = &compressionPrevention;
        bool *const isRestoringPtr = &isRestoring;

        Parser parser;

        parser.parse(argc, argv, isRestoringPtr, fileNamePtr, outputFileNamePtr, passwordPtr, processingModePtr, frameHeightPtr, frameWidthPtr, compressionPreventionPtr);
    }
    catch (Error *e)
    {
        e->handle();
        delete e;
    }
}