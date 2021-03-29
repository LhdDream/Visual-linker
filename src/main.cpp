#include "cmdline.h"

using namespace std;
//展示链接信息
//参数
//   -S      空间与地址分配的过程
//   -s      展示符号决议的过程
//   -R      展示重定位的过程
//   -M      展示 详细的链接库信息 
//   -T      指定 链接脚本
//   -e      指定 lds 中的 entry 程序入口
//   -o      指定 生成可执行文件的名称
//   -p      链接信息以markdown格式写入文件
//   -f      指定要链接的可重定位文件
int main(int argc, char * argv []) {
    cmdline::parser parse; // 创建命令行解析器
    parse.add<string>("file",'f',"指定要链接的可重定位文件(以,分隔)",true,"");
    parse.add<string>("name",'o',"指定 生成可执行文件的名称", false ,"a.out");
    parse.add<string>("map", 'm', "展示详细的链接信息", true, "");
    parse.add<string>("assign",'T', "指定链接脚本",true, "");
    parse.add<string>("entry",'e',"指定程序入口",true, "");
    parse.add<string>("print",'p',"链接信息以markdown格式写入文件",true,"");
    parse.add("S",'\0',"空间与地址分配的过程");
    parse.add("s",'\0',"展示符号决议的过程");
    parse.add("R",'\0',"展示重定位过程");
    parse.parse_check(argc,argv);
    
    string command =  "ld";
    //去进行相应函数的处理
    if(parse.exist("S")){
        //空间与地址分配的过程
    }
    if(parse.exist("s")){
        //展示符号决议的过程
    }
    if(parse.exist("R")){
        //展示重定位的过程
    }
    return 0;
}