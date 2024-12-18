#include "ende.hpp"   // Include the Ende class for encryption and compression functionalities
#include "error.hpp"  // Include the Error class for handling error messages
#include "file.hpp"   // Include file handling functionalities
#include "parser.hpp" // Include the Parser class for command line argument parsing
#include "ziply.hpp"  // Include the Ziply class for video processing
#include <iostream>   // Include the iostream library for input and output operations

int main(int argc, char *argv[]) {
  try {
    std::string fileName;                 // Variable to hold the input file name
    std::string outputFileName;           // Variable to hold the output file name
    std::string password = "ziplySecret"; // Default password for encryption/decryption
    int frameHeight, frameWidth;          // Variables to hold the dimensions of the video frames
    int bitPixelRatio = 1;                // Variable to hold the bits per pixel ratio for encoding
    bool isRestoring;                     // Flag to indicate if the operation is a restoration

    Parser parser; // Create a Parser object to handle command line arguments
    parser.parse(argc, argv, isRestoring, fileName, outputFileName, password, frameHeight, frameWidth);
    // Parse the command line arguments and populate the relevant variables

    Ziply ziply(fileName, outputFileName, password, frameWidth, frameHeight, bitPixelRatio);
    // Create a Ziply object with the provided parameters for processing

    if (isRestoring)   // Check if the operation is a restoration
      ziply.restore(); // Call the restore method to restore video frames
    else
      ziply.convert();                     // Otherwise, call the convert method to generate video frames
  } catch (const Error &e) { e.handle(); } // Catch any errors and handle them appropriately
}