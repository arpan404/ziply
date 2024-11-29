#include "parser.hpp"

void Parser::parse(int argc, char *argv[], bool *const isRestoring, std::string *const fileName, std::string *const outputFileName, std::string *const password, std::string *const processingMode, int *const frameHeight, int *const frameWidth, float *const compressionPrevention)
{
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
            if (command == "create")
            {
                throw new Error("Invalid argument set to create a zipled file.\n\nExample of a valid command:\nziply create -f example.png -o zipled -r 1080p -p ziplySecret2 -m gpu -c 8\n\nFor detailed information on the available options, try running 'ziply --help'.",
                                "par-ex2");
            }

            else if (command == "restore")
            {
                throw new Error("Invalid argument set to restore data from zipled file.\n\nExample of a valid command:\nziply restore -f example.mp4 -o restored -p ziplySecret2 -m gpu\n\nFor detailed information on the available options, try running 'ziply --help'.",
                                "par-ex3");
            }
            else
            {
                throw new Error("Invalid argument '" + command + "' provided.\n\nFor detailed information on the available options, try running 'ziply --help'.", "par-ex4");
            }
        }
    }

    for (int i = 0; i < argc; i++)
    {
        this->params.push_back(std::string(argv[i]));
    }
    this->validateArguments(isRestoring, fileName, outputFileName, password, processingMode, frameHeight, frameWidth, compressionPrevention);
}

void Parser::validateArguments(bool *const isRestoring, std::string *const fileName, std::string *const outputFileName, std::string *const password, std::string *const processingMode, int *const frameHeight, int *const frameWidth, float *const compressionPrevention)
{
    if (params[1] != "create" && params[1] != "restore")
    {
        displayEnteredArguments(this);
        markErrorPart(1, this);
        throw new Error("Invalid argument '" + params[1] + "'. Expected 'create' or 'restore', instead got '" + params[1] + "'.\n\nFor detailed information on the available options, try running 'ziply --help'.", "par-ex5");
    }
    else
    {
        if (params[1] == "convert")
        {
            *isRestoring = false;
        }
        else
        {
            *isRestoring = true;
        }
    }
}

void Parser::displayHelpTexts()
{
    std::cout << "\n=== Ziply Command Options ===\n";

    std::cout << "\n** Creating a Ziply Video **\n";
    std::cout << "Command: ziply create [options]\n";

    std::cout << "\nMandatory Option:\n";
    std::cout << "  -f  <file_path>   Path or name of the file to convert.\n";

    std::cout << "\nOptional Options:\n";
    std::cout << "  -o  <output_path> Path or name for the output file. (File extension is ignored if provided.)\n";
    std::cout << "  -r  <resolution>  Output video resolution. Options: 480p, 720p, 1080p [default], 1440p, 4k.\n";
    std::cout << "  -p  <secret_key>  Secret key for encryption. [default: 'ziplySecret']\n";
    std::cout << "  -m  <mode>        Processing mode. Options:\n"
              << "                     - 'cpu-single': Single CPU core.\n"
              << "                     - 'cpu-multi': All CPU cores.\n"
              << "                     - 'gpu': Use GPU with CPU fallback. [default]\n"
              << "                     - 'gpu-cpu': Use GPU and all CPU cores.\n";
    std::cout << "  -c  <ratio>       Compression prevention (pixel-to-bit ratio). Options: 1[default], 2, 4, 8, 16.\n"
              << "                    Higher values increase the file size, suitable for compression-based platforms.\n";

    std::cout << "\nExamples:\n";
    std::cout << "  ziply create -f example.zip\n"
              << "    Converts 'example.zip' into 'example.mp4' with default settings.\n";
    std::cout << "  ziply create -f example.png -o zipled -r 1080p -p mySecret -m gpu -c 8\n"
              << "    Converts 'example.png' into 'zipled.mp4' at 1080p resolution, encrypted with 'mySecret', using GPU, with a pixel-to-bit ratio of 8.\n";

    std::cout << "\n** Restoring Original Data **\n";
    std::cout << "Command: ziply restore [options]\n";

    std::cout << "\nMandatory Option:\n";
    std::cout << "  -f  <file_path>   Path or name of the video file to restore.\n";

    std::cout << "\nOptional Options:\n";
    std::cout << "  -o  <output_path> Path or name for the restored file. [default: current directory]\n";
    std::cout << "  -p  <secret_key>  Secret key for decryption.[default: 'ziplySecret']\n";
    std::cout << "  -m  <mode>        Processing mode. Options:\n"
              << "                     - 'cpu-single': Single CPU core.\n"
              << "                     - 'cpu-multi': All CPU cores.\n"
              << "                     - 'gpu': Use GPU with CPU fallback. [default]\n"
              << "                     - 'gpu-cpu': Use GPU and all CPU cores.\n";

    std::cout << "\nExamples:\n";
    std::cout << "  ziply restore -f example.mp4\n"
              << "    Restores the original data from 'example.mp4' using default settings.\n";
    std::cout << "  ziply restore -f example.mp4 -o zipled -p mySecret -m gpu\n"
              << "    Restores original data from 'example.mp4' to 'zipled', using GPU and the secret key 'mySecret'.\n";

    exit(0);
}

void displayEnteredArguments(Parser *parser)
{
    int length = parser->params.size();
    for (int i = 0; i < length; i++)
    {
        std::cout << parser->params[i] << " ";
    }
    std::cout << "\n";
}

void markErrorPart(int index, Parser *parser)
{
    int totalLength = 0;
    for (const std::string &str : parser->params)
    {
        totalLength += str.length();
    }

    totalLength += (parser->params.size() - 1);
    int startingIndex = 0;
    int i;
    for (i = 0; i < index; i++)
    {
        startingIndex += parser->params[i].length();
    }
    startingIndex += i;
    int endIndex = startingIndex + parser->params[index].length();
    for (int j = 0; j < totalLength; j++)
    {
        if (j < startingIndex || j >= endIndex)
        {
            std::cout << " ";
        }
        else
        {
            std::cout << "^";
        }
    }
}