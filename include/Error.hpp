#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include <iostream>
#include <cstdlib>

class Error
{
public:
    Error(const std::string &message, const std::string &code) : message(message), code(code) {}
    void handle();

private:
    std::string message;
    std::string code;
};

#endif