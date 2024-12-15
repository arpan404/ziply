#include "parser.hpp"

void Parser::parse(int argc, char *argv[], bool &isRestoring, std::string &fileName, std::string &outputFileName,
                   std::string &password, int &frameHeight, int &frameWidth) { // Removed bitPixelRatio
  if (argc < 2) {
    throw Error("No arguments passed.\nIf you need help running ziply, run 'ziply --help'", "par-ex1");
  }
  if (argc == 2) {
    std::string command = std::string(argv[1]);

    if (std::string(argv[1]) == "--help") {
      this->displayHelpTexts();
    } else {
      if (command == "create") {
        throw Error("Invalid argument set to create a zipled file.\n\nExample of a valid command:\nziply "
                    "create -f example.png -o zipled -r 1080p -p ziplySecret2\n\nFor detailed information "
                    "on the available options, try running 'ziply --help'.",
                    "par-ex2");
      }

      else if (command == "restore") {
        throw Error("Invalid argument set to restore data from zipled file.\n\nExample of a valid "
                    "command:\nziply restore -f example.mp4 -o restored -p ziplySecret2\n\nFor detailed "
                    "information on the available options, try running 'ziply --help'.",
                    "par-ex3");
      } else {
        throw Error("Invalid argument '" + command +
                        "' provided.\n\nFor detailed information on the available options, try running 'ziply --help'.",
                    "par-ex4");
      }
    }
  }

  for (int i = 0; i < argc; i++) { this->params.push_back(std::string(argv[i])); }
  this->validateArguments(isRestoring, fileName, outputFileName, password, frameHeight,
                          frameWidth); // Removed bitPixelRatio
}

void Parser::validateArguments(bool &isRestoring, std::string &fileName, std::string &outputFileName,
                               std::string &password, int &frameHeight, int &frameWidth) { // Removed bitPixelRatio
  if (params[1] != "create" && params[1] != "restore") {
    displayEnteredArguments(this);
    markErrorPart(1, this);
    throw Error("Invalid argument '" + params[1] + "'. Expected 'create' or 'restore', instead got '" + params[1] +
                    "'.\n\nFor detailed information on the available options, try running 'ziply --help'.",
                "par-ex5");
  } else {
    if (params[1] == "create") {
      isRestoring = false;
    } else {
      isRestoring = true;
    }
    prepareArguments(isRestoring, fileName, outputFileName, password, frameHeight, frameWidth); // Removed bitPixelRatio
  }
}

void Parser::prepareArguments(bool &isRestoring, std::string &fileName, std::string &outputFileName,
                              std::string &password, int &frameHeight, int &frameWidth) { // Removed bitPixelRatio
  int argumentsListLength = params.size();
  bool isInputFileProvided = false;
  bool isResolutionProvided = false;
  if (isRestoring) {
    std::unordered_set<std::string> availableArguments = {"-f", "-o", "-p"};
    int i = 2;
    while (i < argumentsListLength) {
      if (params[i][0] == '-') {
        if (availableArguments.find(params[i]) != availableArguments.end()) {
          if (i + 1 < argumentsListLength) {
            if (availableArguments.find(params[i + 1]) == availableArguments.end() && params[i + 1][0] == '-') {
              displayEnteredArguments(this);
              markErrorPart(i, this);
              throw Error("Expected a value after '" + params[i] +
                              "', but got none.\n\nFor detailed information on the available options, "
                              "try running 'ziply --help'.",
                          "par-ey1");
            } else {
              if (params[i] == "-f") {
                fileName = params[i + 1];
                isInputFileProvided = true;
              }

              if (params[i] == "-o") {
                outputFileName = params[i + 1];
              }

              if (params[i] == "-p") {
                password = params[i + 1];
              }
              i += 2;
            }
          } else {
            displayEnteredArguments(this);
            markErrorPart(i, this);
            throw Error("Expected a value after '" + params[i] +
                            "', but got none.\n\nFor detailed information on the available options, try "
                            "running 'ziply --help'.",
                        "par-ey3");
          }
        } else {
          displayEnteredArguments(this);
          markErrorPart(i, this);
          throw Error("Expected a value after '" + params[i] +
                          "', but got none.\n\nFor detailed information on the available options, try "
                          "running 'ziply --help'.",
                      "par-ey4");
        }
      } else {
        displayEnteredArguments(this);
        markErrorPart(i, this);
        throw Error("Invalid argument '" + params[i] +
                        "' provided.\n\nFor detailed information on the available options, try running 'ziply --help'.",
                    "par-ey5");
      }
    }

    if (!isInputFileProvided) {
      throw Error("Expected an input file, but got none.\n\nFor detailed information on the available options, "
                  "try running 'ziply --help'.",
                  "par-ey6");
    }
  } else {
    std::unordered_set<std::string> availableArguments = {"-f", "-o", "-p", "-r"}; // Removed "-c"
    int i = 2;
    while (i < argumentsListLength) {
      if (params[i][0] == '-') {
        if (availableArguments.find(params[i]) != availableArguments.end()) {
          if (i + 1 < argumentsListLength) {
            if (availableArguments.find(params[i + 1]) == availableArguments.end() && params[i + 1][0] == '-') {
              displayEnteredArguments(this);
              markErrorPart(i, this);
              throw Error("Expected a value after '" + params[i] +
                              "', but got none.\n\nFor detailed information on the available options, "
                              "try running 'ziply --help'.",
                          "par-ez1");
            } else {
              if (params[i] == "-f") {
                fileName = params[i + 1];
                isInputFileProvided = true;
              }

              if (params[i] == "-o") {
                outputFileName = params[i + 1];
              }

              if (params[i] == "-p") {
                password = params[i + 1];
              }

              if (params[i] == "-r") {
                if (params[i + 1] != "1080p" && params[i + 1] != "360p" && params[i + 1] != "480p" &&
                    params[i + 1] != "720p" && params[i + 1] != "1440p" && params[i + 1] != "4k") {
                  displayEnteredArguments(this);
                  markErrorPart(i + 1, this);
                  throw Error("Expected value: '360p' or '480p' or '720p' or '1080p' or '1440p' or "
                              "'4k' but got '" +
                                  params[i] +
                                  "'.\n\nFor detailed information on the available options, try "
                                  "running 'ziply --help'.",
                              "par-ez3");
                }
                std::string res = params[i + 1];
                if (res == "360p") {
                  frameWidth = 640;
                  frameHeight = 360;
                  isResolutionProvided = true;
                } else if (res == "480p") {
                  frameWidth = 854;
                  frameHeight = 480;
                  isResolutionProvided = true;
                } else if (res == "720p") {
                  frameWidth = 1280;
                  frameHeight = 720;
                  isResolutionProvided = true;
                } else if (res == "1080p") {
                  frameWidth = 1920;
                  frameHeight = 1080;
                  isResolutionProvided = true;
                } else if (res == "1440p") {
                  frameWidth = 2560;
                  frameHeight = 1440;
                  isResolutionProvided = true;
                } else if (res == "4k") {
                  frameWidth = 3840;
                  frameHeight = 2160;
                  isResolutionProvided = true;
                } else {
                  frameWidth = 1920;
                  frameHeight = 1080;
                  isResolutionProvided = true;
                }
              }

              i += 2;
            }
          } else {
            displayEnteredArguments(this);
            markErrorPart(i, this);
            throw Error("Expected a value after '" + params[i] +
                            "', but got none.\n\nFor detailed information on the available options, try "
                            "running 'ziply --help'.",
                        "par-ez5");
          }
        } else {
          displayEnteredArguments(this);
          markErrorPart(i, this);
          throw Error("Invalid argument '" + params[i] +
                          "' provided.\n\nFor detailed information on the available options, try running "
                          "'ziply --help'.",
                      "par-ez6");
        }
      }

      if (!isResolutionProvided) {
        frameHeight = 1080;
        frameWidth = 1920;
      }

      if (!isInputFileProvided) {
        throw Error("Expected an input file, but got none.\n\nFor detailed information on the available "
                    "options, try running 'ziply --help'.",
                    "par-ez6");
      }
    }
  }
}

void Parser::displayHelpTexts() {
  std::cout << "\n=== Ziply Command Options ===\n";

  std::cout << "\n** Creating a Ziply Video **\n";
  std::cout << "Command: ziply create [options]\n";

  std::cout << "\nMandatory Option:\n";
  std::cout << "  -f  <file_path>   Path or name of the file to convert.\n";

  std::cout << "\nOptional Options:\n";
  std::cout << "  -o  <output_path> Path or name for the output file. (File extension is ignored if provided.)\n";
  std::cout << "  -r  <resolution>  Output video resolution. Options: 360p, 480p, 720p, 1080p [default], 1440p, 4k.\n";
  std::cout << "  -p  <secret_key>  Secret key for encryption. [default: 'ziplySecret']\n";
  // Removed compression ratio help text

  std::cout << "\nExamples:\n";
  std::cout << "  ziply create -f example.zip\n"
            << "    Converts 'example.zip' into 'example.mp4' with default settings.\n";
  std::cout << "  ziply create -f example.png -o zipled -r 1080p -p mySecret\n"
            << "    Converts 'example.png' into 'zipled.mp4' at 1080p resolution, encrypted with 'mySecret'.\n";

  std::cout << "\n** Restoring Original Data **\n";
  std::cout << "Command: ziply restore [options]\n";

  std::cout << "\nMandatory Option:\n";
  std::cout << "  -f  <file_path>   Path or name of the video file to restore.\n";

  std::cout << "\nOptional Options:\n";
  std::cout << "  -o  <output_path> Path or name for the restored file. [default: current directory]\n";
  std::cout << "  -p  <secret_key>  Secret key for decryption.[default: 'ziplySecret']\n";

  std::cout << "\nExamples:\n";
  std::cout << "  ziply restore -f example.mp4\n"
            << "    Restores the original data from 'example.mp4' using default settings.\n";
  std::cout << "  ziply restore -f example.mp4 -o zipled -p mySecret\n"
            << "    Restores original data from 'example.mp4' to 'zipled', using the secret key 'mySecret'.\n";

  exit(0);
}

void displayEnteredArguments(Parser *parser) {
  int length = parser->params.size();
  for (int i = 0; i < length; i++) { std::cout << parser->params[i] << " "; }
  std::cout << "\n";
}

void markErrorPart(int index, Parser *parser) {
  int totalLength = 0;
  for (const std::string &str : parser->params) { totalLength += str.length(); }

  totalLength += (parser->params.size() - 1);
  int startingIndex = 0;
  int i;
  for (i = 0; i < index; i++) { startingIndex += parser->params[i].length(); }
  startingIndex += i;
  int endIndex = startingIndex + parser->params[index].length();
  for (int j = 0; j < totalLength; j++) {
    if (j < startingIndex || j >= endIndex) {
      std::cout << " ";
    } else {
      std::cout << "^";
    }
  }
}