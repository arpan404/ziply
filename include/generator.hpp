#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "file.hpp" // Include the file handling functionalities
#include <future>   // Include for using futures to handle asynchronous tasks
#include <iostream> // Include for input/output stream operations
#include <string>   // Include for using string data type
#include <vector>   // Include for using vector container

// The Generator class is responsible for generating video frames from input files
// and restoring them from Ziply fragments. It handles the necessary parameters
// for frame generation and restoration processes.
class Generator {
public:
  // Constructor to initialize the Generator object with necessary parameters.
  // Parameters:
  // - fileName: The name of the input file to be processed.
  // - outputFileName: The name of the output file after processing.
  // - password: The password for encrypting/decrypting the video file.
  // - frameWidth: The width of the video frames.
  // - frameHeight: The height of the video frames.
  // - bitPixelRatio: The ratio of bits per pixel for encoding.
  Generator(const std::string &fileName, const std::string &outputFileName, const std::string &password,
            const int frameWidth, const int frameHeight, const int bitPixelRatio);

  // Method to generate video frames from the input file.
  void generate();

  // Method to restore video frames from Ziply fragments.
  void restore();

private:
  std::string inputFileName;  // The name of the input file
  std::string outputFileName; // The name of the output file
  std::string password;       // The password for encryption/decryption
  int frameWidth;             // The width of the video frames
  int frameHeight;            // The height of the video frames
  int bitPixelRatio;          // The ratio of bits per pixel for encoding

  // Asynchronously converts the input buffer into video frames.
  // Parameters:
  // - buffer: The data buffer containing the input file data.
  // - bytes_read: The number of bytes read from the input file.
  // - frameName: The name of the frame to be created.
  std::future<void> convertToFrames(const std::vector<char> buffer, std::streamsize bytes_read,
                                    const std::string frameName);

  // Restores frame data from a given frame path.
  // Parameters:
  // - framePath: The path to the frame file to be restored.
  std::vector<char> restoreFrameData(const std::string framePath);

  // Asynchronously writes frames to a Ziply fragment.
  // Parameters:
  // - framePath: The path to the frame to be written.
  // - ziplyFragPath: The path to the Ziply fragment where the frame will be stored.
  std::future<void> writeFramesToZiplyFrag(const std::string framePath, const fs::path ziplyFragPath);
};

#endif