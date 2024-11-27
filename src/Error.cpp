#include <Error.hpp>
Error::Error(const std::string message, std::string code)
{
    this->message = message;
    this->code = code;
    this->handle();
}

void Error::handle()
{
    std::cerr << "Error occured. For more info, see the error log below" << "\n";
    std::cerr << "Error message: " << this->message << "\n";
    std::cerr << "Error code:  " << this->code << "\n";
    exit(1);
}