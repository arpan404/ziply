#include "error.hpp"

void Error::handle()
{
    std::cerr << "\nZiply exited with error code '" << this->code << "'.\n\n";
    std::cerr << this->message << "\n";
    delete this;
    exit(1);
}