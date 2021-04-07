//符号决议的过程以及符号重定位
#ifndef _SYMOBOL_H__
#define _SYMOBOL_H__

#include "tabulate/table.hpp"
#include "elf_parser.h"
#include <vector>
#include <string>
#include <unordered_set>
using namespace tabulate;



class Symbol {
    public:

    private:
    // //三个集合
    // //所有的可重定位程序
    // std::vector<std::string> m_files;
    // //未定义的符号集合
    // std::unordered_set<symbol_t> m_noexit; //未定义的
    // //已经定义的符号集合
    // std::unordered_set<symbol_t> m_exit; // 已经定义的
    // //重定义的符号集合
    // std::unordered_set<symbol_t> m_mults; //重复定义的
};
#endif