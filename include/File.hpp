#ifndef FILE_HPP
#define FILE_HPP

#include <filesystem>
#include <vector>
namespace fs = std::filesystem;
namespace file
{
    std::vector<fs::path> getFilePath(const std::string &inputFileName, const std::string &outputFileName);
}

#endif