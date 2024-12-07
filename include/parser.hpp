#ifndef PARSER_CPP
#define PARSER_CPP

#include <vector>
#include <string>
#include <unordered_set>

#include "error.hpp"

class Parser
{
public:
    void parse(int argc, char *argv[], bool &isRestoring, std::string &fileName, std::string &outputFileName, std::string &password, std::string &processingMode, int &frameHeight, int &frameWidth, float &bitPixelRatio);

private:
    std::vector<std::string> params;
    void validateArguments(bool &isRestoring, std::string &fileName, std::string &outputFileName, std::string &password, std::string &processingMode, int &frameHeight, int &frameWidth, float &bitPixelRatio);
    void prepareArguments(bool &isRestoring, std::string &fileName, std::string &outputFileName, std::string &password, std::string &processingMode, int &frameHeight, int &frameWidth, float &bitPixelRatio);
    void displayHelpTexts();
    friend void displayEnteredArguments(Parser *parser);
    friend void markErrorPart(int index, Parser *parser);
};

#endif