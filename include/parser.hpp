#ifndef PARSER_CPP
#define PARSER_CPP

#include <string>
#include <unordered_set>
#include <vector>

#include "error.hpp"

// The Parser class is responsible for parsing command line arguments
// and validating them for the Ziply application.
class Parser {
public:
  // Parses the command line arguments and sets the appropriate parameters.
  // Parameters:
  // - argc: The number of command line arguments.
  // - argv: The array of command line arguments.
  // - isRestoring: Reference to a boolean indicating if the operation is a restore.
  // - fileName: Reference to the input file name.
  // - outputFileName: Reference to the output file name.
  // - password: Reference to the password for encryption/decryption.
  // - frameHeight: Reference to the height of the video frames.
  // - frameWidth: Reference to the width of the video frames.
  void parse(int argc, char *argv[], bool &isRestoring, std::string &fileName, std::string &outputFileName,
             std::string &password, int &frameHeight, int &frameWidth);

private:
  // Vector to hold the command line parameters.
  std::vector<std::string> params;

  // Validates the parsed arguments to ensure they meet the required criteria.
  // Parameters:
  // - isRestoring: Reference to a boolean indicating if the operation is a restore.
  // - fileName: Reference to the input file name.
  // - outputFileName: Reference to the output file name.
  // - password: Reference to the password for encryption/decryption.
  // - frameHeight: Reference to the height of the video frames.
  // - frameWidth: Reference to the width of the video frames.
  void validateArguments(bool &isRestoring, std::string &fileName, std::string &outputFileName, std::string &password,
                         int &frameHeight, int &frameWidth);

  // Prepares the arguments for processing after validation.
  // Parameters:
  // - isRestoring: Reference to a boolean indicating if the operation is a restore.
  // - fileName: Reference to the input file name.
  // - outputFileName: Reference to the output file name.
  // - password: Reference to the password for encryption/decryption.
  // - frameHeight: Reference to the height of the video frames.
  // - frameWidth: Reference to the width of the video frames.
  void prepareArguments(bool &isRestoring, std::string &fileName, std::string &outputFileName, std::string &password,
                        int &frameHeight, int &frameWidth);

  // Displays help text for the user, detailing available commands and options.
  void displayHelpTexts();

  // Friend function to display the arguments entered by the user.
  friend void displayEnteredArguments(Parser *parser);

  // Friend function to mark the part of the input that caused an error.
  friend void markErrorPart(int index, Parser *parser);
};

#endif