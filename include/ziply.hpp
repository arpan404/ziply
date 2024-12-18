#ifndef ZIPLY_HPP
#define ZIPLY_HPP

#include <string>

// The Ziply class is responsible for handling the creation and restoration of video files.
// It manages file input/output, resolution settings, and password protection.
class Ziply {
private:
  std::string fileName;       // The name of the input file to be processed
  std::string outputFileName; // The name of the output file after processing
  int frameHeight;            // The height of the video frames
  int frameWidth;             // The width of the video frames
  float bitPixelRatio;        // The ratio of bits per pixel for encoding
  std::string password;       // The password for encrypting/decrypting the video file

public:
  // Constructor to initialize the Ziply object with necessary parameters.
  // Parameters include input file name, output file name, password, frame dimensions, and bit pixel ratio.
  Ziply(const std::string &fileName, const std::string &outputFileName, const std::string &password,
        const int frameWidth, const int frameHeight, int bitPixelRatio);

  // Method to convert the input file into a video format.
  void convert();

  // Method to restore a video file from a previously created Ziply file.
  void restore();
};

#endif