#include "cmdline.h"
#include "utils/strings.cpp"
#include "src/section.h"
#include "src/markdown.h"
#include "src/relocation.h"
#include "src/symbol.h"
#include "src/sole.hpp"
#include "src/cmd.h"
#include <cassert>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

using namespace tabulate;
//展示链接信息
//参数
//   -S      展示输入重定位文件中的段表信息
//   -s      展示符号决议的过程
//   -R      展示重定位的过程
//   -p      链接信息以markdown格式写入文件
//   -f      需要分析可重定位文件/ELF文件(以,分隔)
//   -T      指定链接脚本(辅助分析详细的过程,需要指定)
//   -o      指令链接脚本生成的可执行文件

//判断文件是不是可执行的文件

bool isexec(const std::vector<std::string> & files) {
    for(auto & file  : files){
        if(access(file.c_str(),X_OK)!=-1)
        {
            return true;
        }
    }
    return false;
}

bool isexit(const std::string & files) {
    return access(files.c_str(),F_OK) != -1;
}

int main(int argc, char * argv []) {
    cmdline::parser parse; // 创建命令行解析器
    parse.add<string>("file",'f',"需要分析可重定位文件/ELF文件(以,分隔)",false,"");
    parse.add<string>("print",'p',"保留分析信息的markdown文件名称(默认为visual.md)",false,"visual.md");
    parse.add<string>("parse", 'T',"指定链接脚本(辅助分析详细的过程,需要指定)",false,"");
    parse.add<string>("obj",'o',"指令链接脚本生成的可执行文件",false ,"a.out");
    parse.add("S",'\0',"展示段表信息");
    parse.add("s",'\0',"展示符号决议的过程");
    parse.add("R",'\0',"展示重定位过程");
    parse.parse_check(argc,argv);

    auto file = parse.get<string>("file");  //可重定位文件
    auto filename = parse.get<string>("print");// 指定的markdown 文件名称
    auto ldsname = parse.get<string>("parse"); // 指定的链接脚本
    auto objname = parse.get<string>("obj");
    if(objname.empty()){
        objname = "a.out";
    }
    vector<string> files ;
    files = Split(file,",");//获取所有可重定位文件
    if(isexec(files)){
       std::cout << "存在可执行文件无法进行连接分析"  << std::endl;
       return -1;
    }
    sole::uuid u0 = sole::uuid0();
    std::string uuid = u0.base62();
    if(!ldsname.empty()) {
        //执行一个连接
        std::string result = exec(files,uuid,ldsname,objname);
        if(!isexit(objname)){
            std::cout << "链接失败,错误原因:" << result << std::endl;
            return -1;
        }
    }
    if(parse.exist("S")){
        //展示段表信息
        Section section(files);
        section.parse();
        if(!ldsname.empty()){
            section.parsemap(uuid);
            vector<string> tmp;
            tmp.push_back(objname);
            section.swap(tmp);
            section.parse();
        }
        auto table = section.get_tables();
        if(!filename.empty()){
            Markdown md(table);
            md.writefile(filename);
        }
    }
    if(parse.exist("s")){
        //展示符号决议的过程
        if(!filename.empty()){
            Symbol symbol(files,filename);
            symbol.parse();
            if(!ldsname.empty()){
                symbol.parseobj(objname);
            }
        }
    }
    if(parse.exist("R")){
        //展示重定位的过程
        Relocation relocat(files);
        if(!ldsname.empty()){
           relocat.start_address(uuid);
           relocat.loadlib(uuid);
        }
        relocat.parse();
        if(!ldsname.empty()){
            relocat.obj_parse(objname);
        }
        auto table = relocat.get_table();
        if(!filename.empty()){
            Markdown md(table);
            md.writefile(filename);
        }
    }
    return 0;
}