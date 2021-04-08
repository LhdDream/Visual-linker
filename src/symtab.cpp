#include "symtab.h"
#include "../utils/easylogging++.h"
#include "../utils/strings.cpp"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <elf.h>


typedef struct reloc
{
	const char *	sym_name;	// refers this symbol name
	bool		is_func;	// symbol is function?
	size_t		offset;		// from the patched sym
	int64_t		addend;		// relocation's addend, if rela
	struct reloc *	next;		// linked list of relocations
} reloc;

/**
 * Describes the properties of an ELF symbol that are of interest.
 */
typedef struct sym
{
	size_t		offset;	// address of the sym
	int 		type;	// type of the sym
	const char *	name;	// symbol's name
	struct reloc *	relocs;	// relocs that reference this symbol
} sym;

/**
 * Describes a collection of ELF symbols (symtab).
 */
typedef struct symtab_s
{
	sym *	syms;		// array of nsyms (or more) elements
	size_t	nsyms;		// number of actual symbols in the syms array
	size_t 	free_idx;	// index of the next "free" slot in the syms array
} symtab_s;

static int	sym_compare(const void* s1, const void* s2)
{
	const sym *sym1 = (const sym *)s1;
	const sym *sym2 = (const sym *)s2;

	return  (sym1->offset > sym2->offset)
		? 1
		: (sym1->offset == sym2->offset ? 0 : -1);
}

/**
 * Sorts the given symbol table based on symbols offset.
 */
extern void	symtab_sort(symtab_t* st)
{
	assert(st);
	assert(st->free_idx > 0);

	qsort(st->syms, st->free_idx, sizeof(sym), sym_compare);
}

/**
 * Allocates new symbol table capable of holding up to nsyms entries.
 * The allocated resources must be released with symtab_free().
 */
extern symtab_t*	symtab_alloc(size_t nsyms)
{
	sym *syms = (sym*)calloc(nsyms, sizeof(sym));
	if (!syms)
	{
		LOG(INFO) << "Not enough memory";
	}

	symtab_s *st = (symtab_s *)malloc(sizeof(symtab_s));
	if (!st)
	{
		LOG(INFO) <<  "Not enough memory";
	}

	st->syms = syms;
	st->nsyms = nsyms;
	st->free_idx = 0;

	return st;
}

/**
 * Releases the resources allocated for the symbol table (see symtab_alloc()).
 */
extern void		symtab_free(symtab_t* s)
{
	assert(s);

	for (size_t i = 0; i < s->free_idx; ++i)
	{
		reloc *relocs = s->syms[i].relocs;
		while (relocs)
		{
			reloc *next_reloc = relocs->next;
			free(relocs);
			relocs = next_reloc;
		}
		s->syms[i].relocs = NULL;
	}

	free(s->syms);
	free(s);
}

/**
 * Returns symbol, which offset is not less than the given offset and is nearest
 * to it. If not found, returns NULL.
 */
static sym*	locate_sym(symtab_t* st, size_t offset)
{
	sym *ret = NULL;

	size_t upper = st->nsyms;
	size_t lower = 0;
	size_t prev_i = upper;
	size_t i = 0;

	while (i != prev_i)
	{
		assert(upper > lower);
		prev_i = i;
		i = (upper - lower) / 2 + lower;

		ret = &st->syms[i];
		if (offset < ret->offset)
		{
			upper = i;
		}
		else if (offset > ret->offset)
		{
			lower = i;
		}
		else
		{
			break; // exact match
		}
	}

	return ret;
}

/**
 * Adds a symbol with the given properties to the given symbol table.
 */
extern size_t		symtab_add_sym(symtab_t* symtab, size_t offset, int type, const char* sym_name)
{
	assert(symtab);
	assert(symtab->syms);
	assert(symtab->free_idx < symtab->nsyms);

	symtab->syms[symtab->free_idx].offset = offset;
	symtab->syms[symtab->free_idx].type = type;
	symtab->syms[symtab->free_idx].name = sym_name;
	symtab->syms[symtab->free_idx].relocs = NULL;

	return ++symtab->free_idx;
}

/**
 * Adds relocation information to the appropriate symbol (determined by the offset) in the given symbol table.
 */
extern void		symtab_add_reloc(symtab_t* st, size_t offset, const char* sym_name, bool is_func, int64_t addend)
{
	assert(st);

	sym *s = locate_sym(st, offset);
	if (s)
	{
		// We're supposed to have found a relocation pointing into the symbol
		// 's', not before it
		assert(offset >= s->offset);

		reloc *new_r = (reloc *)malloc(sizeof(reloc));
		if (!new_r)
		{
			LOG(INFO) <<  "Out of memory";
		}
		new_r->sym_name = sym_name;
		new_r->is_func = is_func;
		new_r->offset = offset - s->offset;
		new_r->addend = addend;
		new_r->next = NULL;

		// Find where to place this new relocation
		if (!s->relocs)
		{
			s->relocs = new_r;
		}
		else
		{
			struct reloc *r = s->relocs;
			struct reloc *last_r = NULL;
			for (; r && r->offset < new_r->offset; r = r->next)
			{
				last_r = r;
			}

			if (last_r)
			{
				last_r->next = new_r;
			}
			else
			{
				s->relocs = new_r; // add to the head
			}
			new_r->next = r;
		}
	}
	else
	{
		LOG(INFO) <<  "unable to locate sym corresponding to offset ";
	}
}



static void	dump_reloc(reloc *r,std::string & result)
{

	//fprintf(stdout, "\t(+0x%04lx)-> ", r->offset);
	result += "\t";
	result += "(";
	result += int_to_hex(r->offset);
	result += ")-> ";

	if (r->sym_name)
	{
		//fprintf(stdout, "%s", r->sym_name);
		result += r->sym_name;
		if (r->is_func)
			//fprintf(stdout, "()");
			result += "()";
	}

	// Not interested in seeing zero addend; but if it's
	// all there is, print it anyway
	const bool show_addend = (r->addend != 0) || !r->sym_name;
	if (show_addend)
	{
	//	fprintf(stdout, "%+ld", r->addend);
		result += int_to_hex(r->addend);
	}

	//fprintf(stdout, "\n");
	result += "\n";
}

static void	dump_sym(sym* s,std::string & result)
{
	//fprintf(stdout, "%s (addr 0x%08lx)\n", s->name, s->offset);
	result += s->name ;
	result += " (addr ";
	result += int_to_hex(s->offset) +")\n";
	for (reloc *r = s->relocs; r; r = r->next)
	{
		dump_reloc(r,result);
	}
}


#define STT_OBJECT	1
#define STT_FUNC	2

extern bool		args_sym_is_interesting(const char *name, int type)
{
	if (type != STT_FUNC && type != STT_OBJECT)
		return false;


	return true;
}

/**
 * Prints out the contents of the symbol table, excluding symbols that are not of interest to the user based on the options given.
 * See args_sym_is_interesting().
 */
extern void	symtab_dump(symtab_t* st,std::string & result)
{
	assert(st);

	bool empty_output = true;
	for (size_t i = 0; i < st->free_idx; ++i)
	{
		sym *s = &st->syms[i];

		if (args_sym_is_interesting(s->name, s->type))
		{
			if (s->relocs)
			{
				dump_sym(s,result);
				empty_output = false;
			}
		}
	}
}
