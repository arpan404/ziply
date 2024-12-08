#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <string>

class Generator
{
public:
    Generator(std::string inputFileName, std::string outputFileName, int frameWidth, int frameHeight);
    ~Generator();
    void generate();

private:
    std::string inputFileName;
    std::string outputFileName;
    int frameWidth;
    int frameHeight;
};

#endif