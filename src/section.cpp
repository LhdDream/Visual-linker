#include "section.h"
#include "tabulate/table.hpp"
#include "../utils/strings.cpp"
#include "../utils/command.cpp"

using namespace tabulate;
vector<Table> Section::get_tables() {
    return m_result;
}

void Section::parse(){
    for(auto file : m_files){
        //文件内容
        Table format;
        format.add_row({"File","Name","Type","Address","Size"});
        Elf_parser elf_parser(file);
        std::vector<section_t> sec = elf_parser.get_sections();
        for(auto & i :sec){
            if(!i.section_name.empty())
            {
                if(i.section_name.find("data") != std::string::npos || i.section_name.find("text") != std::string::npos || i.section_name.find("bss") != std::string::npos)
                     format.add_row({file,i.section_name,i.section_type,int_to_hex(i.section_addr),to_string(i.section_size)});
            }
        }
        m_result.push_back(format);
    }
}

void Section::swap(vector<string> & files){
    m_files.swap(files);
}

void Section::parsemap(const string & filename) {
    std::string cmd ;
    std::vector<std::vector<string>> results;
    for(auto file : m_files)
    {
        cmd = "cat " + filename + ".map" + " | sed '/LOAD/d' | ";
        cmd += "grep " + file +" | sed '$d' | awk '{print $1\" \"$2\" \"$3\" \"$4}' ";
        std::string result = command(cmd);
        std::vector<std::string> tmp = Split(result,"\n");
        for(auto i : tmp){
            if(!i.empty())
                results.push_back(Split(i," "));
        }
    }
    sort(results.begin(),results.end(),[]( const std::vector<std::string> &a, const std::vector<std::string> &b){return a[1] < b[1];});
    Table table;
    table.add_row({"File","Section","Address","Size"});
    for(auto i : results){
        if(i[0].find("data") != std::string::npos || i[0].find("text") != std::string::npos || i[0].find("bss") != std::string::npos){
            unsigned int x;
            std::stringstream ss;
            ss << std::hex << i[2];
            ss >> x;
            table.add_row({i[3],i[0],i[1],to_string(x)});
        }
    }
    m_result.push_back(table);
}




