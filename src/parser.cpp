#include "parser.hpp"

void Parser::parse(int argc, char *argv[], std::string *const fileName, std::string *const outputFileName, std::string *const password, std::string *const processingMode, int *const frameHeight, int *const frameWidth, float *const compressionPrevention)
{
    // std::string example = "Ziply -f example.zip -o example -res 1440p"
    if (argc < 2)
    {
        throw new Error("No arguments passed. Trying using: ziply --help", "par-ex1");
    }
    if (argc == 1)
    {
        std::cout<<argv[0];
        if (std::string(argv[0]) == "--help")
        {
            std::cout << "Help";
        }
        else
        {
            throw new Error("No arguments passed. Trying using: ziply --help", "par-ex2");
        }
    }
    // if (argc <)
    // {
    //     throw new Error("No arguments passed. Trying using\nziply --help")
    // }
}