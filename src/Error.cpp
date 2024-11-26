#include <Error.hpp>
Error::Error(const std::string message, std::string code)
{
    this->message = message;
    this->code = code;
    this->handle();
}

void Error::handle()
{
    std::cout << "Error occured. For more info, see the error log below" << "\n";
    std::cout << "Error message: " << this->message << "\n";
    std::cout << "Error code:  " << this->code << "\n";
    exit(1);
}