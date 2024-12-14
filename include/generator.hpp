#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <string>
#include <future> 
#include <vector>
#include <string>
#include <fstream>
#include "file.hpp"

class Generator
{
private:
    std::string inputFileName;
    std::string outputFileName;
    std::string password;
    int frameWidth;
    int frameHeight;
    float bitPixelRatio;
    
    // Private helper method for reading chunks
    std::vector<char> readFileChunk(std::ifstream& file, size_t offset, size_t chunkSize);
    std::vector<std::future<std::vector<char>>> readFileInChunks(const fs::path& filePath, size_t chunkSize);

public:
    Generator(const std::string &fileName, const std::string &outputFileName, const std::string &password, const int frameWidth, const int frameHeight, const float bitPixelRatio);
    void generate();
    void restore();
};

#endif