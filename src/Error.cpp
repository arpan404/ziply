#include "error.hpp"

void Error::handle() const {
  std::cerr << "\nZiply exited with error code '" << this->code << "'.\n\n";
  std::cerr << this->message << "\n";
  exit(1);
}