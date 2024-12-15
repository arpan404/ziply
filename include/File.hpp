#ifndef FILE_HPP
#define FILE_HPP

#include <filesystem>
#include <vector>
namespace fs = std::filesystem;
namespace file {
bool pathExists(const std::string &path);
fs::path getAbsolutePath(const std::string &path);

std::vector<fs::path> getConvertFilePath(const std::string &inputFileName, const std::string &outputFileName);
} // namespace file

#endif