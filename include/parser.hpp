#ifndef PARSER_CPP
#define PARSER_CPP

#include <vector>
#include <string>
#include <unordered_set>

#include "error.hpp"

class Parser
{
public:
    void parse(int argc, char *argv[], bool *const isRestoring, std::string *const fileName, std::string *const outputFileName, std::string *const password, std::string *const processingMode, int *const frameHeight, int *const frameWidth, float *const compressionPrevention);

private:
    std::vector<std::string> params;
    void validateArguments(bool *const isRestoring, std::string *const fileName, std::string *const outputFileName, std::string *const password, std::string *const processingMode, int *const frameHeight, int *const frameWidth, float *const compressionPrevention);
    void displayHelpTexts();
    friend void displayEnteredArguments(Parser *parser);
    friend void markErrorPart(int index, Parser *parser);
};

#endif