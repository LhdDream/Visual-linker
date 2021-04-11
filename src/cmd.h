#ifndef _CMD_H_
#define _CMD_H_
#include "../utils/command.cpp"
#include <string>
#include <vector>
static std::string exec(const std::vector<std::string> &files,const std::string& id,const std::string & lds, const std::string & objname){
    std::string cmd ;
    cmd += "ld ";
    for(auto file  : files){
        cmd +=  file + " ";
    }
    if(!lds.empty())
        cmd += "-T " + lds;
    cmd += " -Map=" + id +".map";
    cmd += " -o " + objname;
    return command(cmd);
}
#endif