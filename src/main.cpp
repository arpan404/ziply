#include <iostream>
#include <File.hpp>

int main()
{
    std::cout<<"calling"<<std::flush;
    File file = File("ff.png", 2000, 2000);
    file.convertToFrames();
}