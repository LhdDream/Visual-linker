#ifndef _SECTIONS_H
#define _SECTIONS_H

#include <iostream>
#include <inttypes.h>
#include <vector>
#include <string>
#include "../src/elf_parser.h"

using namespace std;
//只关注text,data,bss 段
class Section {
    public:
    Section(vector<string> &files) : m_files(files) {}
    void parse();
    private:
    vector<string> m_files;
};
#endif