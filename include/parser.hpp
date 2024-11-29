#ifndef PARSER_CPP
#define PARSER_CPP

#include <vector>
#include <string>
#include <unordered_set>

#include "error.hpp"

class Parser
{
public:
    void parse(int argc, char *argv[], std::string *const fileName, std::string *const outputFileName, std::string *const password, std::string *const processingMode, int *const frameHeight, int *const frameWidth, float *const compressionPrevention);

private:
    std::vector<std::string> params;
    void displayHelpTexts();
};

#endif