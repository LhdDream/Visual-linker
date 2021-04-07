#ifndef _SECTIONS_H
#define _SECTIONS_H

#include <iostream>
#include <inttypes.h>
#include <vector>
#include <string>
#include "elf_parser.h"
#include "tabulate/table.hpp"
using namespace std;
using namespace tabulate;
//只关注text,data,bss 段
class Section {
    public:
    Section(vector<string> &files) : m_files(files) {}
    Table parse();
    private:
    vector<string> m_files;
};
#endif