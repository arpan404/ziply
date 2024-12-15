#include "generator.hpp"
#include "ende.hpp"
#include "file.hpp"
#include "threadpool.hpp"
#include "stb_image_write.h"

Generator::Generator(const std::string &fileName, const std::string &outputFileName, const std::string &password, const int frameWidth, const int frameHeight, const int bitPixelRatio)
{
    this->inputFileName = fileName;
    this->outputFileName = outputFileName;
    this->password = password;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    this->bitPixelRatio = bitPixelRatio;
}

void Generator::generate()
{
    std::vector<fs::path> filePaths = file::getConvertFilePath(this->inputFileName, this->outputFileName);
    fs::path outputFilePath = filePaths[1].replace_extension(".ziply");
    Ende::compressAndEncrypt(filePaths[0], outputFilePath, this->password, 9);
    std::cout << "Compression and encryption completed successfully" << std::endl;

    std::cout << "Output file path: " << this->frameHeight << " " << this->frameWidth << std::endl;

    // chunk_size = ((this->frameHeight * this->frameWidth) / 8);
    // std::ifstream file(outputFilePath, std::ios::binary);
    // ThreadPool pool(std::thread::hardware_concurrency());
    // std::cout << "Chunk size: " << chunk_size << std::endl;
    // if (!file)
    // {
    //     throw Error("Could not open the file", "efx1");
    // }

    // std::vector<char> buffer(chunk_size);
    // std::cout << "Buffer size: " << buffer.size() << std::endl;
    // int currentFrame = 0;
    // while (file)
    // {
    //     file.read(buffer.data(), buffer.size());
    //     std::streamsize bytes_read = file.gcount();
    //     if (bytes_read > 0)
    //     {
    //         std::string frameName = std::to_string(currentFrame++) + ".png";
    //         pool.enqueue([this, buffer = std::vector<char>(buffer), bytes_read, frameName]()
    //                      { convertToFrames(buffer, bytes_read, frameName); });
    //     }
    // }
}

void Generator::convertToFrames(std::vector<char> buffer, std::streamsize bytes_read, std::string frameName)
{
    unsigned char *image = new unsigned char[this->frameWidth * this->frameHeight * 3];
    size_t currentXaxis = 0, currentYaxis = 0, currentPixelIndex = 0;
    size_t totalPixelsToWrite = bytes_read * 8;

    for (size_t i = 0; i < bytes_read; ++i)
    {
        std::bitset<8> currentByte(buffer[i]);
        for (int j = 0; j < 8; ++j)
        {
            image[currentPixelIndex] = currentByte[j];
            currentPixelIndex++;
        }
    }

    if (currentPixelIndex >= totalPixelsToWrite)
    {
        int x = currentXaxis;
        for (int y = currentYaxis; y < this->frameHeight; ++y)
        {
            for (; x < this->frameWidth; ++x)
            {
                image[(y * this->frameWidth + x) * 3 + 0] = 255; // Red channel
                image[(y * this->frameWidth + x) * 3 + 1] = 0;   // Green channel
                image[(y * this->frameWidth + x) * 3 + 2] = 0;   // Blue channel
            }

            x = 0;
        }
    }
    // std::async(std::launch::async, [this, frameName, image]()
    //            {
    stbi_write_png(frameName.c_str(), this->frameWidth, this->frameHeight, 3, image, this->frameWidth * 3);
    // delete[] image; });
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

    try
    {
        // Add debug output before decompression
        std::cout << "Starting decompression and decryption..." << std::endl;
        Ende::decompressAndDecrypt(inputFilePath, outputFilePath, this->password);
        std::cout << "Decompression and decryption completed successfully" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Detailed error during decompression: " << e.what() << std::endl;
        throw;
    }
}