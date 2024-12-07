#include "file.hpp"

bool file::pathExists(const std::string &path)
{
    return fs::exists(path);
}

fs::path file::getAbsolutePath(const std::string &path)
{
    return fs::absolute(path);
}

vector<fs::path> file::getConvertFilePath(const std::string &inputFileName, const std::string &outputFileName)
{
    vector<fs::path> paths;
    fs::path inputFilePath = file::getAbsolutePath(inputFileName);

    if (file::pathExists(inputFilePath))
    {
        paths.push_back(inputFilePath);
    }
    else
    {
        throw Error("Input file '" + inputFileName + "' does not exist.", "file-e-z1");
    }

    fs::path outputFilePath = file::getAbsolutePath(outputFileName);
    if (file::pathExists(outputFilePath))
    {
        throw Error("Output file '" + outputFileName + "' already exists.", "file-e-z2");
    }

    outputFilePath.replace_extension(".mp4");
    if (file::pathExists(outputFilePath))
    {
        throw Error("Output file '" + outputFileName + "' already exists.", "file-e-z2");
    }
    paths.push_back(outputFilePath);

    return paths;
}