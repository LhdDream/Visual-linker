//符号决议的过程以及符号重定位
#ifndef _SYMOBOL_H__
#define _SYMOBOL_H__

#include "tabulate/table.hpp"
#include "elf_parser.h"
#include <vector>
#include <string>
#include <unordered_set>
#include <map>
using namespace tabulate;


typedef struct {
    std::size_t operator()(const symbol_t& _node) const {
        return  std::hash<std::string>()(_node.symbol_name) ^ std::hash<std::string>()(_node.symbol_section);
    }
}symbol_hash;


class Symbol {
    public:
        Symbol(std::vector<std::string> &files, const std::string& file) : m_files(files),m_file(file){

        }
        void files();
        void parse();
        void parseobj(std::string &objname);
        void parse_result(std::string & result);
    private:
    // //三个集合
    // //所有的可重定位程序
    std::string m_result; // 符号决议的过程
    std::string m_file; // 需要写入的文件
    std::string m_content;//需要写入的内容
    std::vector<std::string> m_files;
    std::vector<std::pair<std::string,std::vector<symbol_t>>> m_symols;//全局符号表
    std::unordered_set<symbol_t,symbol_hash> m_global;
};
#endif