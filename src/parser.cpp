#include "parser.hpp"

void Parser::parse(int argc, char *argv[], std::string *const fileName, std::string *const outputFileName, std::string *const password, std::string *const processingMode, int *const frameHeight, int *const frameWidth, float *const compressionPrevention)
{
    const std::unordered_set<std::string> availableCommands = {
        "-f",
        "-o",
        "-r",
        "-p",
        "-m",
        "-c"};
    if (argc < 2)
    {
        throw new Error("No arguments passed.\nIf you need help running ziply, run 'ziply --help'", "par-ex1");
    }
    if (argc == 2)
    {
        std::string command = std::string(argv[1]);

        if (std::string(argv[1]) == "--help")
        {
            this->displayHelpTexts();
        }
        else
        {
            if (availableCommands.find(command) != availableCommands.end())
            {
                throw new Error("Invalid argument set. \n\nExample of a valid command:\nziply -f example.png -o zipled -r 1080p -p ziplySecret2 -m gpu -c 8\n\nFor detailed information on the available options, try running 'ziply --help'.",
                                "par-ex2");
            }
            else
            {
                throw new Error("Invalid argument '" + command + "' provided.\n\nFor detailed information on the available options, try running 'ziply --help'.", "par-ex3");
            }
        }
    }

    for (int i = 1; i < argc; i++)
    {
        this->params.push_back(std::string(argv[i]));
    }
    this->validateArguments(fileName, outputFileName, password, processingMode, frameHeight, frameWidth, compressionPrevention);
}

void Parser::validateArguments(std::string *const fileName, std::string *const outputFileName, std::string *const password, std::string *const processingMode, int *const frameHeight, int *const frameWidth, float *const compressionPrevention)
{
}

void Parser::displayHelpTexts()
{
    std::cout << "\nZiply Command Options:\n";

    std::cout << "\nMandatory:\n";
    std::cout << "-f  > File path or name to convert.\n";

    std::cout << "\nOptional:\n";
    std::cout << "-o  > Output file path or name (extension is ignored if provided).\n";
    std::cout << "-r  > Resolution for output video (480p, 720p, 1080p, 1440p, or 4k, default: 1080p).\n";
    std::cout << "-p  > Secret key for encryption (default: 'ziplySecret').\n";
    std::cout << "-m  > Processing mode: 'cpu-single' (single core), 'cpu-multi' (all cores), 'gpu' (GPU or CPU fallback) [default], 'gpu-cpu' (all cores of CPU & GPU).\n";
    std::cout << "-c  > Compression prevention (Pixel to bit ratio, default: 1). Higher values increase output length, suitable for uploading to compression-based platforms.\n";

    std::cout << "\nExamples:\n";
    std::cout << "ziply -f example.zip\n"
              << "\tConverts 'example.zip' to 'example.mp4'.\n";
    std::cout << "ziply -f example.png -o zipled -r 1080p -p ziplySecret2 -m gpu -c 8\n"
              << "\tConverts 'example.png' to 'zipled.mp4' at 1080p resolution, encrypted with key 'ziplySecret2', using GPU, with pixel-to-bit ratio of 8.\n";

    exit(0);
}