#include "markdown.h"
#include "../utils/strings.cpp"
void Markdown::writefile(const std::string &filepath){
    for(auto tmp :content)
     WriteFile(filepath,tmp);
}