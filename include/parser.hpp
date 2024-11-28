#ifndef PARSER_CPP
#define PARSER_CPP

#include <string>

class Parser
{
public:
    void parse(int argc, char *argv[], std::string *const fileName, std::string *const outputFileName, std::string *const password, std::string *const processingMode, int *const frameHeight, int *const frameWidth, float *const compressionPrevention);
}

#endif