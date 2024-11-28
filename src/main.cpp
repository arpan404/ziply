#include <iostream>
#include <File.hpp>

int main()
{
    File file = File("test.png", 1000, 1000);
    file.convertToFrames();
}