#include <iostream>
#include <File.hpp>

int main()
{
    std::cout<<"calling"<<std::flush;
    File file = File("ff.png", 4000, 4000);
    file.convertToFrames();
}