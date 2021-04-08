#include "section.h"
#include "tabulate/table.hpp"
#include "../utils/strings.cpp"

using namespace tabulate;
vector<Table> Section::parse(){

    vector<Table> result;
    for(auto file : m_files){
        //文件内容
        Table format;
        format.add_row({"File","Name","Type","Address","Offset","Size","EntSize","Align"});
        Elf_parser elf_parser(file);
        std::vector<section_t> sec = elf_parser.get_sections();
        for(auto & i :sec){
            if(i.section_name.find("text") != std::string::npos || i.section_name.find("data") != std::string::npos || i.section_name.find("bss") != std::string::npos)
            {
                format.add_row({file,i.section_name,i.section_type,int_to_hex(i.section_addr),int_to_hex(i.section_offset),int_to_hex(i.section_size),int_to_hex(i.section_ent_size),to_string(i.section_addr_align)});
            }
        }
        result.push_back(format);
    }
    return result;
}


