#include "file.hpp"  // Include the file handling functionalities
#include "error.hpp" // Include the custom error handling class
#include <vector>    // Include the vector library for using dynamic arrays

// Checks if a given path exists in the filesystem.
// Parameters:
// - path: The path to check for existence.
// Returns: true if the path exists, false otherwise.
bool file::pathExists(const std::string &path) { return fs::exists(path); }

// Retrieves the absolute path of a given relative path.
// Parameters:
// - path: The relative path to convert to an absolute path.
// Returns: The absolute path as a fs::path object.
fs::path file::getAbsolutePath(const std::string &path) { return fs::absolute(path); }

// Generates a list of file paths for conversion based on the input and output file names.
// Parameters:
// - inputFileName: The name of the input file to be converted.
// - outputFileName: The name of the output file after conversion.
// Returns: A vector of fs::path objects representing the paths of files to be converted.
std::vector<fs::path> file::getConvertFilePath(const std::string &inputFileName, const std::string &outputFileName) {
  std::vector<fs::path> paths;                                   // Vector to hold the file paths
  fs::path inputFilePath = file::getAbsolutePath(inputFileName); // Get the absolute path of the input file

  // Check if the input file exists
  if (file::pathExists(inputFilePath)) {
    paths.push_back(inputFilePath); // Add the input file path to the vector
  } else {
    throw Error("Input file '" + inputFileName + "' does not exist.",
                "file-e-z1"); // Throw an error if the file does not exist
  }

  fs::path outputFilePath = file::getAbsolutePath(outputFileName); // Get the absolute path of the output file

  // If the output file name is empty, use the input file name
  if (outputFileName.empty()) {
    outputFilePath = inputFilePath;
  }
  outputFilePath.replace_extension(".mp4"); // Change the output file extension to .mp4

  // Check if the output file already exists
  if (file::pathExists(outputFilePath)) {
    throw Error("Output file '" + outputFileName + "' already exists.",
                "file-e-z2"); // Throw an error if the file already exists
  }
  paths.push_back(outputFilePath); // Add the output file path to the vector

  return paths; // Return the vector of file paths
}

// Generates a list of file paths for restoration based on the input and output file names.
// Parameters:
// - inputFileName: The name of the input file to be restored.
// - outputFileName: The name of the output file after restoration.
// Returns: A vector of fs::path objects representing the paths of files to be restored.
std::vector<fs::path> file::getRestoreFilePath(const std::string &inputFileName, const std::string &outputFileName) {
  std::vector<fs::path> paths;                                   // Vector to hold the file paths
  fs::path inputFilePath = file::getAbsolutePath(inputFileName); // Get the absolute path of the input file

  // Check if the input file has the correct extension
  if (inputFilePath.extension() != ".mp4") {
    throw Error("Input file is not a video", "file-not-video"); // Throw an error if the file is not a video
  }

  // Check if the input file exists
  if (file::pathExists(inputFilePath)) {
    paths.push_back(inputFilePath); // Add the input file path to the vector
  } else {
    throw Error("Input file '" + inputFileName + "' does not exist.",
                "file-e-z1"); // Throw an error if the file does not exist
  }

  fs::path outputFilePath = file::getAbsolutePath(outputFileName); // Get the absolute path of the output file

  // If the output file name is empty, use the input file name
  if (outputFileName.empty()) {
    outputFilePath = inputFilePath;
  }
  paths.push_back(outputFilePath); // Add the output file path to the vector

  return paths; // Return the vector of file paths
}
