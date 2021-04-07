#include "cmdline.h"
#include "utils/strings.cpp"
#include "src/section.h"
#include "src/markdown.h"
#include <cassert>
using namespace std;

using namespace tabulate;
//展示链接信息
//参数
//   -S      展示输入重定位文件中的段表信息
//   -s      展示符号决议的过程
//   -R      展示重定位的过程
//   -T      指定 链接脚本
//   -p      链接信息以markdown格式写入文件
//   -f      指定要链接的可重定位文件
//   -o      可重定位名称
int main(int argc, char * argv []) {
    cmdline::parser parse; // 创建命令行解析器
    parse.add<string>("file",'f',"指定要链接的可重定位文件(以,分隔)",false,"");
    parse.add<string>("assign",'T', "指定链接脚本进行链接",false, "");
    parse.add<string>("print",'p',"保留链接信息的markdown文件名称(默认为visual.md)",false,"visual.md");
    parse.add<string>("name",'o' , "文件名称",false, "a.out");
    parse.add("S",'\0',"展示段表信息");
    parse.add("s",'\0',"展示符号决议的过程");
    parse.add("R",'\0',"展示重定位过程");
    parse.parse_check(argc,argv);

    auto file = parse.get<string>("file");  //可重定位文件
    auto scriptname = parse.get<string>("assign");//指定的链接脚本名称
    auto filename = parse.get<string>("print");// 指定的markdown 文件名称
    auto name = parse.get<string>("name");//名称

    vector<string> files ;
    files = Split(file,",");//获取所有可重定位文件

    if(parse.exist("S")){
        //展示段表信息
        Section section(files);
        auto table = section.parse();
        if(!filename.empty()){
            Markdown md(table);
            md.writefile(filename);
        }
    }
    if(parse.exist("s")){
        //展示符号决议的过程
    }
    if(parse.exist("R")){
        //展示重定位的过程
    }
    return 0;
}