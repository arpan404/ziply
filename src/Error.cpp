#include "error.hpp"

Error::Error(const std::string message, std::string code)
{
    this->message = message;
    this->code = code;
    this->handle();
}

void Error::handle()
{
    std::cerr << "\nZiply exited with error code '" << this->code << "'.\n\n";
    std::cerr << this->message << "\n";
    delete this;
    exit(1);
}