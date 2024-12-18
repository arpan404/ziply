#ifndef FILE_HPP
#define FILE_HPP

#include <filesystem> // Include the filesystem library for handling file paths and directories
#include <vector>     // Include the vector library for using dynamic arrays

namespace fs = std::filesystem; // Create an alias for the filesystem namespace

namespace file {
// Checks if a given path exists in the filesystem.
// Parameters:
// - path: The path to check for existence.
// Returns: true if the path exists, false otherwise.
bool pathExists(const std::string &path);

// Retrieves the absolute path of a given relative path.
// Parameters:
// - path: The relative path to convert to an absolute path.
// Returns: The absolute path as a fs::path object.
fs::path getAbsolutePath(const std::string &path);

// Generates a list of file paths for conversion based on the input and output file names.
// Parameters:
// - inputFileName: The name of the input file to be converted.
// - outputFileName: The name of the output file after conversion.
// Returns: A vector of fs::path objects representing the paths of files to be converted.
std::vector<fs::path> getConvertFilePath(const std::string &inputFileName, const std::string &outputFileName);

// Generates a list of file paths for restoration based on the input and output file names.
// Parameters:
// - inputFileName: The name of the input file to be restored.
// - outputFileName: The name of the output file after restoration.
// Returns: A vector of fs::path objects representing the paths of files to be restored.
std::vector<fs::path> getRestoreFilePath(const std::string &inputFileName, const std::string &outputFileName);
} // namespace file

#endif
