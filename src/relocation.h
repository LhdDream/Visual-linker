#ifndef _RELOCATION_H
#define _RELOCATION_H

#include <vector>
#include <string>
#include <unordered_map>
#include "tabulate/table.hpp"

using namespace tabulate;
using namespace std;
// relocation 信息
// load 内容 cat ..map | grep LOAD
// text data bss 起始地址
//cat 7YPOD3lzOGH-9QZoQCnh06P.map  | grep .text | head -1
//
// objdump 进行汇编代码的展示  objdump -S ab | grep -A 1 swap | head -2
class Relocation {
    public:
            Relocation(vector<string> & files) : m_files(files) {}
            void parse();
            vector<Table> get_table();
            vector<string> get_note();
            void loadlib(const string & mapname);
            void start_address(const string & mapname);
            void obj_parse(std::string & objname);
    private:
        vector<string> m_files;
        vector<Table>  m_tables;
        vector<string> m_parse_name;
        vector<string> m_notes;
};
#endif