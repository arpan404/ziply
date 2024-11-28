#include "File.hpp"
#include "ThreadPool.hpp"

File::File(std::string fileName, int frameWidth, int frameHeight)
{
    this->fileName = fileName;
    this->frameHeight = frameHeight;
    this->frameWidth = frameWidth;
    this->filePath = "test.png";
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
    Magick::InitializeMagick(nullptr);
    ThreadPool pool(std::thread::hardware_concurrency()); // Use the number of available cores

    if (!file)
    {
        throw Error("Could not open the file", "efx1");
    }

    std::vector<char> buffer(chunk_size);
    int currentFrame = 0;
    while (file)
    {
        std::cout << "Loading: " << currentFrame;
        file.read(buffer.data(), buffer.size());
        std::streamsize bytes_read = file.gcount();
        if (bytes_read > 0)
        {
            std::string frameName = std::to_string(currentFrame++) + ".png";
            pool.enqueue([this, buffer, bytes_read, frameName]()
                         { generateFrames(buffer, bytes_read, frameName); });
        }
    }
}

void File::generateFrames(std::vector<char> buffer, std::streamsize bytes_read, std::string frameName)
{
    try
    {
        Magick::Image image(Magick::Geometry(this->frameWidth, this->frameHeight), "red");

        int currentXaxis = 0, currentYaxis = 0;

        for (size_t i = 0; i < bytes_read; ++i)
        {
            std::bitset<8> currentByte(buffer[i]);

            for (int j = 0; j < 8; ++j)
            {
                image.pixelColor(currentXaxis, currentYaxis, currentByte[j] ? "black" : "white");

                ++currentXaxis;
                if (currentXaxis >= this->frameWidth)
                {
                    currentXaxis = 0;
                    ++currentYaxis;

                    if (currentYaxis >= this->frameHeight)
                    {
                        break;
                    }
                }
            }
            if (currentYaxis >= this->frameHeight)
            {
                break;
            }
        }

#pragma omp parallel
        {
            image.write(frameName);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error generating frames: " << e.what() << std::endl;
    }
}
