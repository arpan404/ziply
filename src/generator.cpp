#include "generator.hpp"
#include "ende.hpp"
#include "file.hpp"

Generator::Generator(const std::string &fileName, const std::string &outputFileName, const std::string &password, const std::string &processingMode, const int frameWidth, const int frameHeight, const float bitPixelRatio)
{
    this->inputFileName = fileName;
    this->outputFileName = outputFileName;
    this->password = password;
    this->processingMode = processingMode;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    this->bitPixelRatio = bitPixelRatio;
}

void Generator::generate()
{
    std::vector<fs::path> filePaths = file::getConvertFilePath(this->inputFileName, this->outputFileName);
    fs::path outputFilePath = filePaths[1].replace_extension(".ziply");
    Ende::compressAndEncrypt(filePaths[0], outputFilePath, this->password, 9);
}

void Generator::restore()
{
    // Get absolute paths for both input and output files
    std::vector<fs::path> filePaths = file::getConvertFilePath(this->inputFileName, this->outputFileName);
    fs::path inputFilePath = filePaths[0];
    fs::path outputFilePath = filePaths[1].replace_extension(".png");

    // Add file existence check
    if (!fs::exists(inputFilePath))
    {
        std::cerr << "Error: Input file '" << inputFilePath << "' does not exist." << std::endl;
        throw std::runtime_error("Input file not found");
    }

    // Ensure we have read permissions for input file
    if (!fs::is_regular_file(inputFilePath))
    {
        std::cerr << "Error: '" << inputFilePath << "' is not a regular file." << std::endl;
        throw std::runtime_error("Invalid input file");
    }

    std::cout << inputFilePath << std::endl;
    
    try {
        // Add debug output before decompression
        std::cout << "Starting decompression and decryption..." << std::endl;
        Ende::decompressAndDecrypt(inputFilePath, outputFilePath, this->password);
        std::cout << "Decompression and decryption completed successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Detailed error during decompression: " << e.what() << std::endl;
        throw;
    }
}