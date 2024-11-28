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

    // Fill the image with black and white pixels
    for (int y = 0; y < this->frameHeight; ++y)
    {
        for (int x = 0; x < this->frameWidth; ++x)
        {
            unsigned char color = ((x + y) % 2 == 0) ? 255 : 0; // Alternate black and white
            image[(y * this->frameWidth + x) * 3 + 0] = color;  // Red channel
            image[(y * this->frameWidth + x) * 3 + 1] = color;  // Green channel
            image[(y * this->frameWidth + x) * 3 + 2] = color;  // Blue channel
        }
    }

    // Write the image to a PNG file
    if (stbi_write_png("output.png", this->frameWidth, this->frameHeight, 3, image, this->frameWidth * 3))
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
// void File::generateFrames(std::vector<char> buffer, std::streamsize bytes_read, std::string frameName)
// {
//     try
//     {
//         Magick::Image image(Magick::Geometry(this->frameWidth, this->frameHeight), "red");

//         std::cout << frameName << std::flush;
//         // Pre-calculate the number of pixels in the image (frameWidth * frameHeight)
//         const size_t totalPixels = this->frameWidth * this->frameHeight;

//         // Buffer the pixels in a temporary array instead of calling `pixelColor` for each one
//         std::vector<std::string> pixelBuffer(totalPixels);

//         size_t currentPixelIndex = 0;

//         for (size_t i = 0; i < bytes_read; ++i)
//         {
//             std::bitset<8> currentByte(buffer[i]);

//             for (int j = 0; j < 8; ++j)
//             {
//                 // Fill the pixel buffer with 'black' or 'white' for each bit
//                 pixelBuffer[currentPixelIndex] = currentByte[j] ? "black" : "white";
//                 ++currentPixelIndex;

//                 // Check if we've reached the end of the image
//                 if (currentPixelIndex >= totalPixels)
//                 {
//                     break;
//                 }
//             }
//             if (currentPixelIndex >= totalPixels)
//             {
//                 break;
//             }
//         }

//         // Now apply the buffered pixel data to the image all at once
//         size_t currentXaxis = 0;
//         size_t currentYaxis = 0;

//         for (size_t i = 0; i < totalPixels; ++i)
//         {
//             image.pixelColor(currentXaxis, currentYaxis, pixelBuffer[i]);

//             ++currentXaxis;
//             if (currentXaxis >= this->frameWidth)
//             {
//                 currentXaxis = 0;
//                 ++currentYaxis;
//             }
//         }

//         image.write(frameName);
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Error generating frames: " << e.what() << std::endl;
//     }
// }
