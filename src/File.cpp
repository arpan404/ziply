#include "file.hpp"

bool file::pathExists(const std::string &path)
{
    return fs::exists(path);
}

vector<fs::path> file::getConvertFilePath(const std::string &inputFileName, const std::string &outputFileName)
{
    vector<fs::path> paths;
    fs::path inputFilePath = fs::absolute(inputFileName);

    if (file::pathExists(inputFilePath))
    {
        paths.push_back(inputFilePath);
    }
    else
    {
        throw Error("Input file does not exist.", "file-e-z1");
    }

    fs::path outputFilePath = fs::absolute(outputFileName);
    if (file::pathExists(outputFilePath))
    {
        throw Error("Output file already exists.", "file-e-z2");
    }

    outputFilePath.replace_extension(".mp4");
    paths.push_back(outputFilePath);

    return paths;
}