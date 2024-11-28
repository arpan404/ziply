#include <iostream>
#include <File.hpp>

int main()
{
    File file = File("ff.png", 480, 480);
    file.convertToFrames();
}