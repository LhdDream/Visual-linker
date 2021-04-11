#ifndef SYMTAB_H_
#define SYMTAB_H_

#include <stdbool.h>
#include <sys/types.h>
#include <string>
#include <vector>
typedef struct symtab_s		symtab_t;

symtab_t *	symtab_alloc(size_t nsyms);
void		symtab_free(symtab_t* s);

void		symtab_sort(symtab_t* s);
void		symtab_dump(symtab_t* st,std::string & result,std::vector<std::string> & parse_func);
void		symtab_print_legend();

size_t		symtab_add_sym(symtab_t* symtab, size_t offset, int type, const char* sym_name);
void		symtab_add_reloc(symtab_t* symtab, size_t offset, const char* sym_name, bool is_func, int64_t addend);


#endif
