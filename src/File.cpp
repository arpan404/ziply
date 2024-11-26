#include "File.hpp"

File::File(std::string fileName){
    this->fileName = fileName;
}

bool File::pathExists(const std:: string &path){
    return fs::exists(path);
}

fs::path File::convertToPath(const std::string &fileName){
    fs::path currentDirectory = fs::current_path();
    return currentDirectory / fileName;
}
