#include "depinput.h"
#include "relocation.h"
#include "symtab.h"
#include "elf_parser.h"
#include "../utils/command.cpp"
#include "../utils/strings.cpp"
void Relocation::parse() {
    for(auto& files : m_files){
        Table tmp;
        tmp.add_row({files});
        input_t in;
        input_open(in,files);
        struct reader_funcs rdr = input_read_elf_header(&in);
        elf_sections_t * sec = rdr.find_sections(&in);
	    symtab_t *st = rdr.read_symtab(&in, sec);
        std::string result;
        if(st){
            rdr.process_relocations(&in,sec,st);
            symtab_dump(st,result,m_parse_name);
            symtab_free(st);
        }
        input_close(in);
        if(result.empty()){
            continue;
        }
        tmp.add_row({result});
        m_notes.emplace_back("重定位表");
        m_tables.push_back(tmp);
    }

}

vector<Table> Relocation::get_table(){
    return m_tables;
}
vector<string> Relocation::get_note() {
    return m_notes;
}

void Relocation::loadlib(const std::string & mapname){
    std::string cmd = "python  ../script/analyze_map.py  " + mapname + ".map";
    std::string result = command(cmd);
    Table table;
    table.add_row({"Load files","Size","Section Size"});
    std::vector<std::string> results = Split(result,"\n");
    std::vector<std::vector<std::string>> resultss;
    for(auto i :results){
        auto tmp = Split(i,"\t");
        resultss.push_back(tmp);
    }
    for(auto i : resultss){
        if(!i[0].empty())
            table.add_row({i[0],i[1],i[2]});
    }
    m_notes.emplace_back("加载的文件");
    m_tables.push_back(table);
}

void Relocation::start_address(const std::string & mapname){
    Table table;
    std::string cmd = "cat "+ mapname + ".map  | grep .text | head -1 | awk '{print $1\" \"$2\" \"$3}' ";
    std::string result = command(cmd);
    std::vector<std::string> results;
    results = Split(result, " ");
    table.add_row({"Section","Address","Size"});
    table.add_row({results[0],results[1],results[2]});
    cmd = "cat "+ mapname + ".map  | grep .data | head -1 | awk '{print $1\" \"$2\" \"$3}' ";
    result = command(cmd);
    results = Split(result, " ");
    table.add_row({results[0],results[1],results[2]});
    cmd = "cat "+ mapname + ".map  | grep .bss | head -1 | awk '{print $1\" \"$2\" \"$3}' ";
    result = command(cmd);
    results = Split(result, " ");
    table.add_row({results[0],results[1],results[2]});
     m_notes.emplace_back("段起始地址");
    m_tables.push_back(table);
}

void Relocation::obj_parse(std::string & objname) {
    Table table;
    table.add_row({"Relocation Name","Relocation Value"});
    for(auto name :m_parse_name){
        if(!name.empty()){
            std::string cmd =  "objdump -S " + objname + " | grep -A 1 " + name + " | head -2";
            std::string result = command(cmd);
            std::vector<std::string> results = Split(result,"\n");
            for(auto i : results){
                if(!i.empty()){
                    table.add_row({name,i});
                }
            }

        }
    }
    m_notes.emplace_back("重定位表的汇编结果");
    m_tables.push_back(table);
    Table global;
    global.add_row({"File","Value","Size","Type","Bind","Vis","Ndx","Name"});
    Elf_parser elf_parser(objname);
    std::vector<symbol_t> symbols = elf_parser.get_symbols();
    for(auto sym :symbols){
        for(auto parse_name : m_parse_name){
            if(!parse_name.empty() && sym.symbol_name == parse_name) {
                std::string name = sym.symbol_name;
                name += "(" + sym.symbol_section + ")";
                global.add_row({objname,int_to_hex(sym.symbol_value),int_to_hex(sym.symbol_size),
                sym.symbol_type,sym.symbol_bind,sym.symbol_visibility,sym.symbol_index,
                name});
            }
        }
    }
    m_notes.emplace_back("重定位的表项结果");
    m_tables.push_back(global);
}