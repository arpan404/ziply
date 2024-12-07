#include "file.hpp"

vector<fs::path> file::getFilePath(const std::string &inputFileName, const std::string &outputFileName)
{
    vector<fs::path> paths;
    fs::path inputFilePath = fs::absolute(inputFileName);

    if (fs::exists(inputFilePath))
    {
        paths.push_back(inputFilePath);
    }
    else
    {
        throw Error("Input file does not exist.", "file-e-z1");
    }

    fs::path outputFilePath = fs::absolute(outputFileName);
    if (fs::exists(outputFilePath))
    {
        throw Error("Output file already exists.", "file-e-z2");
    }

    outputFilePath.replace_extension(".mp4");
    paths.push_back(outputFilePath);

    return paths;
}