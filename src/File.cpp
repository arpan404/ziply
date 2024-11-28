#include "File.hpp"
#include "ThreadPool.hpp"

File::File(std::string fileName, int frameWidth, int frameHeight)
{
    std::cout << "Called" << std::flush;
    this->fileName = fileName;
    this->frameHeight = frameHeight;
    this->frameWidth = frameWidth;
    this->filePath = "ff.png";
    std::cout << "Called" << std::flush;
}

bool File::pathExists(const std::string &path)
{
    return fs::exists(path);
}

fs::path File::convertToPath(const std::string &fileName)
{
    fs::path currentDirectory = fs::current_path();
    return currentDirectory / fileName;
}

void File::convertToFrames()
{

    size_t chunk_size = ((this->frameHeight * this->frameWidth) / 8);
    std::ifstream file(this->filePath, std::ios::binary);
    std::cout << 1;
    std::cout << 2;
    ThreadPool pool(std::thread::hardware_concurrency()); // Use the number of available cores
    std::cout << 3;

    if (!file)
    {
        throw Error("Could not open the file", "efx1");
    }

    std::vector<char> buffer(chunk_size);
    int currentFrame = 0;
    std::cout << currentFrame;
    while (file)
    {
        std::cout << "Loading: " << currentFrame << std::flush;
        file.read(buffer.data(), buffer.size());
        std::streamsize bytes_read = file.gcount();
        if (bytes_read > 0)
        {
            std::string frameName = std::to_string(currentFrame++) + ".png";
            std::cout << frameName << std::flush;
            pool.enqueue([this, buffer, bytes_read, frameName]()
                         { generateFrames(buffer, bytes_read, frameName); });
        }
    }
}

void File::generateFrames(std::vector<char> buffer, std::streamsize bytes_read, std::string frameName)
{

    unsigned char *image = new unsigned char[this->frameWidth * this->frameHeight * 3];

    size_t currentXaxis = 0, currentYaxis = 0, currentPixelIndex = 0;
    size_t totalPixelsToWrite = bytes_read * 8;

    for (size_t i = 0; i < bytes_read; ++i)
    {
        std::bitset<8> currentByte(buffer[i]);
        for (int j = 0; j < 8; ++j)
        {

            unsigned char color = currentByte[j] ? 0 : 255;
            if (currentPixelIndex >= totalPixelsToWrite)
            {
                std::cout<<"Hello";
                image[(currentYaxis * this->frameWidth + currentXaxis) * 3 + 0] = 255; // Red channel
                image[(currentYaxis * this->frameWidth + currentXaxis) * 3 + 1] = 0;   // Green channel
                image[(currentYaxis * this->frameWidth + currentXaxis) * 3 + 2] = 0;   // Blue channel
            }
            else
            {
                image[(currentYaxis * this->frameWidth + currentXaxis) * 3 + 0] = color; // Red channel
                image[(currentYaxis * this->frameWidth + currentXaxis) * 3 + 1] = color; // Green channel
                image[(currentYaxis * this->frameWidth + currentXaxis) * 3 + 2] = color; // Blue channel
            }

            ++currentXaxis;
            ++currentPixelIndex;
            if (currentXaxis >= this->frameWidth)
            {
                currentXaxis = 0;
                ++currentYaxis;
            }
            if (currentYaxis >= this->frameHeight)
            {
                break;
            }
        }
    }
    if (stbi_write_png(frameName.c_str(), this->frameWidth, this->frameHeight, 3, image, this->frameWidth * 3))
    {
        std::cout << "PNG image created successfully!" << std::endl;
    }
    else
    {
        std::cerr << "Error creating PNG image!" << std::endl;
    }

    // Clean up
    delete[] image;
}
