#include <iostream>
#include <File.hpp>

int main()
{
    std::cout << "Hello World" << "\n";
    File file = File("test.png", 1080, 1080);
    file.convertToFrames();
}