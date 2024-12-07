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
        std::string processingMode = "cpu-multi";
        int frameHeight, frameWidth;
        float bitPixelRatio = 1;
        bool isRestoring;

        Parser parser;
        parser.parse(argc, argv, isRestoring, fileName, outputFileName, password, processingMode, frameHeight, frameWidth, bitPixelRatio);

        Ziply ziply(fileName, frameWidth, frameHeight);
        if (isRestoring)
            ziply.restore();
        else
            ziply.convert();
    }
    catch (Error *e)
    {
        e->handle();
        delete e;
    }
}