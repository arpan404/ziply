#include "error.hpp"
#include "parser.hpp"
#include "ziply.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        std::string fileName;
        std::string outputFileName;
        std::string password = "ziplySecret";
        int frameHeight, frameWidth;
        int bitPixelRatio = 1;
        bool isRestoring;

        Parser parser;
        parser.parse(argc, argv, isRestoring, fileName, outputFileName, password, frameHeight, frameWidth,
                     bitPixelRatio);

        Ziply ziply(fileName, outputFileName, password, frameWidth, frameHeight, bitPixelRatio);
        if (isRestoring)
            ziply.restore();
        else
            ziply.convert();
    } catch (const Error &e) { e.handle(); }
}