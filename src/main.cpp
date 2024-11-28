#include <iostream>
#include <File.hpp>

int main()
{
    std::cout<<"calling"<<std::flush;
    File file = File("ff.png", 200, 200);
    file.convertToFrames();
}