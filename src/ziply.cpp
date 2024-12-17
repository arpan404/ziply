#include "ziply.hpp"     // Ziply class header
#include "generator.hpp" // Generator class header

// Constructor for the Ziply class
// Initializes the Ziply object with file and frame parameters.
Ziply::Ziply(const std::string &fileName, const std::string &outputFileName, const std::string &password,
             const int frameWidth, const int frameHeight, int bitPixelRatio)
    : fileName(fileName), outputFileName(outputFileName), password(password), frameWidth(frameWidth),
      frameHeight(frameHeight), bitPixelRatio(bitPixelRatio) {}

// Converts the specified file into a video format using the Generator class.
void Ziply::convert() {
  Generator generator(this->fileName, this->outputFileName, this->password, this->frameWidth, this->frameHeight,
                      this->bitPixelRatio);
  generator.generate(); // Generate the video
}

// Restores the original data from the converted video file using the Generator class.
void Ziply::restore() {
  Generator generator(this->fileName, this->outputFileName, this->password, this->frameWidth, this->frameHeight,
                      this->bitPixelRatio);
  generator.restore(); // Restore the original data
}