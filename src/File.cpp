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
            std::cout << std::bitset<8>(buffer[0]) << std::endl;
        }
        if (bytes_read < chunk_size)
        {
            break;
        }
    }
}

void File::generateFrames()
{
}