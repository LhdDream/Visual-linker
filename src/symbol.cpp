#include "symbol.h"
#include "../utils/strings.cpp"
#include "tabulate/markdown_exporter.hpp"
#include "cmd.h"

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
        int len = 0;
        Table tmp_exit;
        tmp_exit.add_row({"File","Value","Size","Type","Bind","Vis","Ndx","Name"});
        for(auto sym : symbol.second){
            if(sym.symbol_index == "UND" || sym.symbol_bind == "LOCAL"){
                continue;
            }
            len++;
            std::string name = sym.symbol_name;
            name += "(" + sym.symbol_section + ")";
            tmp_exit.add_row({symbol.first,int_to_hex(sym.symbol_value),int_to_hex(sym.symbol_size),
            sym.symbol_type,sym.symbol_bind,sym.symbol_visibility,sym.symbol_index,
            name});
        }
        if(len > 0){
            MarkdownExporter tmp_export;
            m_content += tmp_export.dump(tmp_exit) + "\n";
            m_content += "\n";
        }
        len = 0;
        // //遍历未定义的符号
        Table tmp_noexit;
        tmp_noexit.add_row({"File","Value","Size","Type","Bind","Vis","Ndx","Name"});
        for(auto sym : symbol.second){
            if(sym.symbol_index == "UND" && sym.symbol_bind == "GLOBAL"){
                len++;
                std::string name = sym.symbol_name;
                name += "(" + sym.symbol_section + ")";
                tmp_noexit.add_row({symbol.first,int_to_hex(sym.symbol_value),int_to_hex(sym.symbol_size),
                sym.symbol_type,sym.symbol_bind,sym.symbol_visibility,sym.symbol_index,
                name});
            }
        }
        if(len > 0){
            MarkdownExporter tmp_noexport;
            m_content += tmp_noexport.dump(tmp_noexit) + "\n";
            m_content += "\n";
        }
        Table global;
        global.add_row({"Value","Size","Type","Bind","Vis","Ndx","Name"});
        for(auto sym :symbol.second){
            if(sym.symbol_bind == "GLOBAL") {
                if (sym.symbol_index != "UND")
                {
                    auto it = m_global.find(sym);
                    if(it != m_global.end() && it->symbol_index == "UND" ){
                        m_global.erase(it);
                    }
                }
                m_global.emplace(sym);
            }
        }
        for(auto sym : m_global){
            std::string name = sym.symbol_name;
            name += "(" + sym.symbol_section + ")";
            global.add_row({int_to_hex(sym.symbol_value),int_to_hex(sym.symbol_size),
            sym.symbol_type,sym.symbol_bind,sym.symbol_visibility,sym.symbol_index,
            name});
        }
        MarkdownExporter tmp_global;
        m_content += tmp_global.dump(global) + "\n";
        m_content += "\n";
    }
    WriteFile(m_file,m_content);
}

void Symbol::parseobj(std::string &objname) {

    Elf_parser elf_parser(objname);
    std::vector<symbol_t> symbols = elf_parser.get_symbols();
    Table table;
    table.add_row({"File","Value","Size","Type","Bind","Vis","Ndx","Name"});
    for(auto sym :symbols) {
        if(sym.symbol_bind != "GLOBAL") {
            continue;
        }
        std::string name = sym.symbol_name;
        name += "(" + sym.symbol_section + ")";
        table.add_row({objname,int_to_hex(sym.symbol_value),int_to_hex(sym.symbol_size),
            sym.symbol_type,sym.symbol_bind,sym.symbol_visibility,sym.symbol_index,
            name});
    }
    MarkdownExporter markdown;
    std::string content = markdown.dump(table) + "\n";
    content += "\n";
    WriteFile(m_file,content);
}

