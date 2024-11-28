#include "File.hpp"

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

    std::vector<std::thread> threads;

    int currentFrame = 0;

    if (!file)
    {
        throw Error("Could not open the file", "input-file-error");
    }

    std::vector<char> buffer(chunk_size);

    while (file)
    {
        file.read(buffer.data(), buffer.size());

        std::streamsize bytes_read = file.gcount();

        if (bytes_read > 0)
        {
            std::string currentFrameName = std::to_string(currentFrame) + ".png";
            threads.push_back(std::thread([this, buffer, bytes_read, currentFrameName]()
                                          { generateFrames(buffer, bytes_read, currentFrameName); }));
        }
        if (bytes_read < chunk_size)
        {
            break;
        }
    }
    for (auto &t : threads)
    {
        t.join();
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
                if (currentByte[j] == 0)
                {
                    image.pixelColor(currentXaxis, currentYaxis, Magick::Color("white"));
                }
                else
                {
                    image.pixelColor(currentXaxis, currentYaxis, Magick::Color("black"));
                }
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

        image.write(frameName);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error generating frames: " << e.what() << std::endl;
    }
}
