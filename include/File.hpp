#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

class File
{
public:
    File(std::string fileName);
    
private:
    std::string fileName;
    std::string filePath;
    bool pathExists(const std::string &path);
    fs::path convertToPath(const std::string &fileName);

};

#endif