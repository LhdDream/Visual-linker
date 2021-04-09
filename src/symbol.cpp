#include "symbol.h"
#include "../utils/strings.cpp"
#include "tabulate/markdown_exporter.hpp"


void Symbol::files() {
    Table format;
    format.add_row({"files gather"});
    for(auto i : m_files){
        format.add_row({i});
    }
    MarkdownExporter exporter;
    m_content += exporter.dump(format) + "\n";
}

void Symbol::parse() {
    files();
    for(auto i : m_files){
        Elf_parser elf_parser(i);
        std::vector<symbol_t> symbols = elf_parser.get_symbols();
        m_symols.push_back({i,symbols});
    }
    for(auto symbol  : m_symols){
        //遍历已定义的符号
        m_content += symbol.first + "\n Define Symbol Table \n";
        Table tmp_exit;
        tmp_exit.add_row({"Value","Size","Type","Bind","Vis","Ndx","Name"});
        for(auto sym : symbol.second){
            if(sym.symbol_index == "UND"){
                continue;
            }
            std::string name = sym.symbol_name;
            name += "(" + sym.symbol_section + ")";
            tmp_exit.add_row({int_to_hex(sym.symbol_value),int_to_hex(sym.symbol_size),
            sym.symbol_type,sym.symbol_bind,sym.symbol_visibility,sym.symbol_index,
            name});
        }
        MarkdownExporter tmp_export;
        m_content += tmp_export.dump(tmp_exit) + "\n";
        m_content += "\n";
        // //遍历未定义的符号
        m_content += symbol.first + "\n Undefine Symbol Table \n";
        Table tmp_noexit;
        tmp_noexit.add_row({"Value","Size","Type","Bind","Vis","Ndx","Name"});
        for(auto sym : symbol.second){
            if(sym.symbol_index == "UND" && sym.symbol_bind == "GLOBAL"){
                std::string name = sym.symbol_name;
                name += "(" + sym.symbol_section + ")";
                tmp_noexit.add_row({int_to_hex(sym.symbol_value),int_to_hex(sym.symbol_size),
                sym.symbol_type,sym.symbol_bind,sym.symbol_visibility,sym.symbol_index,
                name});
            }
        }
        MarkdownExporter tmp_noexport;
        m_content += tmp_noexport.dump(tmp_noexit) + "\n";
        m_content += "\n";
    }
    WriteFile(m_file,m_content);
}