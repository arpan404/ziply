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

#include <Magick++.h>
#include <bitset>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;

public:
    explicit ThreadPool(size_t threads)
    {
        for (size_t i = 0; i < threads; ++i)
        {
            workers.emplace_back([this]()
                                 {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty())
                            return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                } });
        }
    }

    template <class F>
    void enqueue(F &&task)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(std::forward<F>(task));
        }
        condition.notify_one();
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers)
            worker.join();
    }
};

class File
{
    std::string filePath;
    int frameWidth, frameHeight;
    ThreadPool pool;

public:
    File(const std::string &path, int width, int height, size_t thread_count)
        : filePath(path), frameWidth(width), frameHeight(height), pool(thread_count) {}

    void convertToFrames()
    {
        size_t chunk_size = ((this->frameHeight * this->frameWidth) / 8);
        std::ifstream file(this->filePath, std::ios::binary);
        Magick::InitializeMagick(nullptr);

        if (!file)
        {
            throw std::runtime_error("Could not open the file");
        }

        std::vector<char> buffer(chunk_size);
        int currentFrame = 0;

        while (file.read(buffer.data(), buffer.size()))
        {
            std::streamsize bytes_read = file.gcount();
            if (bytes_read > 0)
            {
                std::string frameName = std::to_string(currentFrame++) + ".png";
                pool.enqueue([this, buffer = std::vector<char>(buffer.begin(), buffer.begin() + bytes_read), frameName]()
                             { generateFrames(buffer, frameName); });
            }
        }
    }

    void generateFrames(const std::vector<char> &buffer, const std::string &frameName)
    {
        try
        {
            Magick::Image image(Magick::Geometry(this->frameWidth, this->frameHeight), "red");

            Magick::PixelPacket *pixels = image.getPixels(0, 0, frameWidth, frameHeight);

            size_t currentXaxis = 0, currentYaxis = 0;
            size_t pixel_index = 0;

            for (char byte : buffer)
            {
                std::bitset<8> bits(byte);

                for (int j = 0; j < 8; ++j)
                {
                    if (currentYaxis >= frameHeight)
                        break;

                    pixels[pixel_index++] = bits[j] ? Magick::Color("black") : Magick::Color("white");

                    if (++currentXaxis >= frameWidth)
                    {
                        currentXaxis = 0;
                        ++currentYaxis;
                    }
                }
                if (currentYaxis >= frameHeight)
                    break;
            }

            image.syncPixels();
            image.write(frameName);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error generating frames: " << e.what() << std::endl;
        }
    }
};
