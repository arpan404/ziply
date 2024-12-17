#include "error.hpp" // Include the Error class header for error handling functionalities

// Method to handle the error by displaying the error message and code
void Error::handle() const {
  // Output the error code to standard error stream
  std::cerr << "\nZiply exited with error code '" << this->code << "'.\n\n";
  // Output the associated error message to standard error stream
  std::cerr << this->message << "\n";
  // Terminate the program with an exit status of 1 indicating an error
  exit(1);
}