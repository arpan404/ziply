#ifndef ZIPLY_HPP
#define ZIPLY_HPP

#include <string>

class Ziply
{
private:
    std::string fileName;
    std::string outputFileName;
    int frameHeight, frameWidth;
    float bitPixelRatio;
    std::string password;

public:
    Ziply(const std::string &fileName, const std::string &outputFileName, const std::string &password, const int frameWidth, const int frameHeight, float bitPixelRatio);

    void convert();
    void restore();
};

#endif