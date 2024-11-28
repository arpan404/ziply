#include <iostream>
#include <File.hpp>

int main()
{
    std::cout << "Hello World" << "\n";
    File file = File("test.png", 220, 220);
    file.convertToFrames();
}