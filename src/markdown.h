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
    Markdown( const vector<Table> & table, const vector<string> &notes) {
        int j = 0;
        for(auto i : table)
        {
            std::string tmp ;
            if(!notes.empty() && j < notes.size()){
                tmp += notes[j++] + "\n\n";
            }
            tmp += exporter.dump(i) +"\n";
            content.push_back(tmp);
        }
    }
    void writefile(const std::string &filepath);
    private:
    MarkdownExporter exporter;
    std::vector<std::string> content;
};


#endif