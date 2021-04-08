#include "depinput.h"
#include "relocation.h"
#include "symtab.h"
vector<Table> Relocation::parse() {
    vector<Table> tables;
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
            symtab_dump(st,result);
            symtab_free(st);
        }
        input_close(in);
        tmp.add_row({result});
        tables.push_back(tmp);
    }
    return tables;
}

