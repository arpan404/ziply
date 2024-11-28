#include "parser.hpp"

void Parser::parse(int argc, char *argv[], std::string *const fileName, std::string *const outputFileName, std::string *const password, std::string *const processingMode, int *const frameHeight, int *const frameWidth, float *const compressionPrevention)
{
    // std::string example = "Ziply -f example.zip -o example -res 1440p"
    if (argc < 2)
    {
        throw new Error("No arguments passed. Trying using: ziply --help", "par-ex1");
    }
    if (argc == 2)
    {
        if (std::string(argv[1]) == "--help")
        {
            std::cout << "\nZiply Command Options:\n";

            std::cout << "\nMandatory:\n";
            std::cout << "-f  > File path or name to convert.\n";

            std::cout << "\nOptional:\n";
            std::cout << "-o  > Output file path or name (extension is ignored if provided).\n";
            std::cout << "-r  > Resolution for output video (480p, 720p, 1080p, 1440p, or 4k).\n";
            std::cout << "-p  > Secret key for encryption (default: 'ziplySecret').\n";
            std::cout << "-m  > Processing mode: 'cpu-single' (single core), 'cpu-multi' (all cores), 'gpu' (GPU or CPU fallback), 'gpu-cpu' (all cores of CPU & GPU).\n";
            std::cout << "-c  > Compression prevention (Pixel to bit ratio, default: 1). Higher values increase output length, suitable for uploading to compression-based platforms.\n";

            std::cout << "\nExamples:\n";
            std::cout << "ziply -f example.zip\n"
                      << "\tConverts 'example.zip' to 'example.mp4'.\n";
            std::cout << "ziply -f example.png -o zipled -r 1080p -p ziplySecret2 -m gpu -c 8\n"
                      << "\tConverts 'example.png' to 'zipled.mp4' at 1080p resolution, encrypted with key 'ziplySecret2', using GPU, with pixel-to-bit ratio of 8.\n";
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