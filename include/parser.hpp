#ifndef PARSER_CPP
#define PARSER_CPP

#include <string>
#include <unordered_set>
#include <vector>

#include "error.hpp"

class Parser {
public:
  void parse(int argc, char *argv[], bool &isRestoring, std::string &fileName, std::string &outputFileName,
             std::string &password, int &frameHeight, int &frameWidth);

private:
  std::vector<std::string> params;
  void validateArguments(bool &isRestoring, std::string &fileName, std::string &outputFileName, std::string &password,
                         int &frameHeight, int &frameWidth);
  void prepareArguments(bool &isRestoring, std::string &fileName, std::string &outputFileName, std::string &password,
                        int &frameHeight, int &frameWidth);
  void displayHelpTexts();
  friend void displayEnteredArguments(Parser *parser);
  friend void markErrorPart(int index, Parser *parser);
};

#endif