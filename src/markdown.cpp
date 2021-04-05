#include "markdown.h"
#include "../utils/strings.cpp"
void Markdown::writefile(const std::string &filepath){
    WriteFile(filepath,content);
}