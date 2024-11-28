#include <iostream>
#include <File.hpp>

int main()
{
    File file = File("ff.png", 1920, 1080);
    file.convertToFrames();
}