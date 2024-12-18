#ifndef ERROR_HPP // Include guard to prevent multiple inclusions of this header file
#define ERROR_HPP

#include <cstdlib>  // Standard library for general utilities, including memory allocation
#include <iostream> // Standard library for input and output stream operations
#include <string>   // Standard library for using the string data type

// The Error class is responsible for handling error messages and codes.
// It encapsulates the error message and its associated code, providing a method to handle the error.
class Error {
public:
  // Constructor to initialize the Error object with a message and a code.
  // Parameters:
  // - message: A string containing the error message.
  // - code: A string representing the error code.
  Error(const std::string &message, const std::string &code) : message(message), code(code) {}

  // Method to handle the error, typically by logging the message and code.
  void handle() const;

private:
  std::string message; // The error message to be displayed or logged
  std::string code;    // The error code associated with the error
};

#endif // End of include guard