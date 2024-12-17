#include "ziply.hpp" // Include the header file for the Ziply class
#include "generator.hpp" // Include the header file for the Generator class

// Constructor for the Ziply class
// Initializes the Ziply object with the provided parameters for file conversion and restoration.
Ziply::Ziply(const std::string &fileName, const std::string &outputFileName, const std::string &password,
             const int frameWidth, const int frameHeight, int bitPixelRatio)
    : fileName(fileName), outputFileName(outputFileName), password(password), frameWidth(frameWidth),
      frameHeight(frameHeight), bitPixelRatio(bitPixelRatio) {}

// Method to convert the specified file into a video format
// Creates a Generator object and calls its generate method to perform the conversion.
void Ziply::convert() {
  Generator generator(this->fileName, this->outputFileName, this->password, this->frameWidth, this->frameHeight,
                      this->bitPixelRatio);
  generator.generate(); // Generate the video from the input file
}

// Method to restore the original data from the converted video file
// Creates a Generator object and calls its restore method to perform the restoration.
void Ziply::restore() {
  Generator generator(this->fileName, this->outputFileName, this->password, this->frameWidth, this->frameHeight,
                      this->bitPixelRatio);
  generator.restore(); // Restore the original data from the video file
}
