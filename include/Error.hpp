#ifndef ERROR_HPP
#define ERROR_HPP

#include <cstdlib>
#include <iostream>
#include <string>

class Error {
  public:
    Error(const std::string &message, const std::string &code) : message(message), code(code) {}
    void handle() const;

  private:
    std::string message;
    std::string code;
};

#endif