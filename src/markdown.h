// 编写markdown 写入文件
//已完成
#include "tabulate/markdown_exporter.hpp"  // 注意这个头文件
#include "tabulate/table.hpp"
#include <vector>
using namespace std;

#ifndef _MARKDOWN_H_
#define _MARKDOWN_H_

using namespace tabulate;
class Markdown{
    public:
    Markdown( const vector<Table> & table) {
        for(auto i : table)
            content +=  exporter.dump(i) +"\n";
    }
    void writefile(const std::string &filepath);
    private:
    MarkdownExporter exporter;
    std::string content;
};


#endif