#include "depinput.h"
#include "../utils/easylogging++.h"
#include "symtab.h"



#include <stdbool.h>
#include <elf.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>
#include <string.h>

INITIALIZE_EASYLOGGINGPP

typedef struct	Elf32
{
	Elf32_Shdr *	sections;	// array of all sections
	uint16_t	shnum;		// number of elements in that array

	Elf32_Shdr *	shstrtab;	// string table for section names

	// symtab
	Elf32_Shdr *	symtab;		// the .symtab section
	int		symtab_idx;	// the section's index
	Elf32_Shdr *	strtab;		// corresponding string section for symbol names

	// dynsym
	Elf32_Shdr *	dsymtab;	// the .dynsym section
	int		dsymtab_idx;	// the section's index
	Elf32_Shdr *	dstrtab;	// corresponding string section for symbol names
} Elf32;

typedef struct	Elf64
{
	Elf64_Shdr *	sections;
	uint16_t	shnum;

	Elf64_Shdr *	shstrtab;

	// symtab
	Elf64_Shdr *	symtab;
	int		symtab_idx;
	Elf64_Shdr *	strtab;

	// dynsym
	Elf64_Shdr *	dsymtab;
	int		dsymtab_idx;
	Elf64_Shdr *	dstrtab;
} Elf64;

typedef struct	elf_sections_s
{
	union
	{
		Elf32	elf32;
		Elf64	elf64;
	};
} elf_sections_s;

static const char*	get_sh_str_64(input_t* in, elf_sections_s* descr, uint32_t i)
{
	assert( descr->elf64.shstrtab );

	// The following is not out of range (we checked already)
	const char* sh_strings = &input_get_mem_map(in)[descr->elf64.shstrtab->sh_offset];
	if ( i >= descr->elf64.shstrtab->sh_size )
	{
		LOG(INFO) <<"Section header string table index %d out of range";
	}

	// our resulting string is always null-terminated (we checked already)
	const char *s = sh_strings+i;
	return s;
}

static const char*	get_str_64(input_t* in, void* sec, uint32_t i)
{
	assert( sec );

	Elf64_Shdr* strtab = (Elf64_Shdr *)sec;
	// The following is not out of range (we checked already)
	const char* strings = &input_get_mem_map(in)[strtab->sh_offset];
	if ( i >= strtab->sh_size )
	{
		LOG(INFO) << "String table index %d out of range ";
	}

	// our resulting string is always null-terminated (we checked already)
	const char* s = strings + i;
	return s;
}

static void	check_sec_size(input_t* in, elf_sections_s* descr, Elf64_Shdr* sec)
{
	if ( (uint64_t)sec->sh_offset + sec->sh_size > input_get_file_size(in) )
	{
		const char* sec_name = get_sh_str_64(in, descr, sec->sh_name);
		LOG(INFO) <<  "section  goes past end of file (corrupted ELF header?)";
	}
}

static void	check_str_sec(input_t* in, elf_sections_s* descr, Elf64_Shdr* sec)
{
	check_sec_size(in, descr, sec);

	if ( input_get_mem_map(in)[sec->sh_offset + sec->sh_size - 1] != 0 )
	{
		const char* s = get_sh_str_64(in, descr, sec->sh_name); // could be no null terminator here

		// makes sure sec_name is null-terminated
		static char sec_name[64];
		strncpy(sec_name, s, sizeof(sec_name));
		sec_name[63] = 0;

		LOG(INFO) <<  "String table section  not null-terminated (corrupted ELF file?)";
	}
}

/**
 * Reads an ELF program header that has different endianness than us and
 * replaces its every data member so that is has the same endianness.
 */
static void	make_ehdr_native_endian_64(Elf64_Ehdr* ehdr)
{
	assert( ehdr );

	ehdr->e_type = get_uint16(&ehdr->e_type);
	ehdr->e_machine = get_uint16(&ehdr->e_machine);
	ehdr->e_version = get_uint32(&ehdr->e_version);
	ehdr->e_entry = get_uint64(&ehdr->e_entry);
	ehdr->e_phoff = get_uint64(&ehdr->e_phoff);
	ehdr->e_shoff = get_uint64(&ehdr->e_shoff);
	ehdr->e_flags = get_uint32(&ehdr->e_flags);
	ehdr->e_ehsize = get_uint16(&ehdr->e_ehsize);
	ehdr->e_phentsize = get_uint16(&ehdr->e_phentsize);
	ehdr->e_phnum = get_uint16(&ehdr->e_phnum);
	ehdr->e_shentsize = get_uint16(&ehdr->e_shentsize);
	ehdr->e_shnum = get_uint16(&ehdr->e_shnum);
	ehdr->e_shstrndx = get_uint16(&ehdr->e_shstrndx);
}

/**
 * Reads an ELF section header that has different endianness than us and
 * replaces its every data member so that is has the same endianness.
 */
static void	make_shdr_native_endian_64(Elf64_Shdr* shdr)
{
	assert( shdr );

	shdr->sh_name = get_uint32(&shdr->sh_name);
	shdr->sh_type = get_uint32(&shdr->sh_type);
	shdr->sh_flags = get_uint64(&shdr->sh_flags);
	shdr->sh_addr = get_uint64(&shdr->sh_addr);
	shdr->sh_offset = get_uint64(&shdr->sh_offset);
	shdr->sh_size = get_uint64(&shdr->sh_size);
	shdr->sh_link = get_uint32(&shdr->sh_link);
	shdr->sh_info = get_uint32(&shdr->sh_info);
	shdr->sh_addralign = get_uint64(&shdr->sh_addralign);
	shdr->sh_entsize = get_uint64(&shdr->sh_entsize);
}

static void	find_sym_sec(input_t* in, elf_sections_s* descr)
{
	Elf64_Shdr* sections = descr->elf64.sections;
	int shnum = descr->elf64.shnum;

	for (int i = 0; i < shnum; ++i)
	{
		Elf64_Shdr* sec = &sections[i];
		if ( sec->sh_type == SHT_SYMTAB || sec->sh_type == SHT_DYNSYM )
		{

			Elf64_Shdr* strsec = &sections[sec->sh_link];

			if( sec->sh_type == SHT_SYMTAB )
			{
				descr->elf64.symtab = sec;
				descr->elf64.strtab = strsec;
				descr->elf64.symtab_idx = i;
			}
			else
			{
				descr->elf64.dsymtab = sec;
				descr->elf64.dstrtab = strsec;
				descr->elf64.dsymtab_idx = i;
			}
		}
	}

	if ( !descr->elf64.symtab && !descr->elf64.dsymtab )
	{
		LOG(INFO) <<  "No .symtab or .dynsym section ";
	}

	// Check the symtab/strtab sections
	if ( descr->elf64.symtab )
	{
		assert(descr->elf64.strtab); // they always go in pairs
		check_sec_size(in, descr, descr->elf64.symtab);
		check_str_sec(in, descr, descr->elf64.strtab);

		// We're going to be reading symtab sequentially real soon
		madvise(&input_get_mem_map(in)[descr->elf64.symtab->sh_offset], descr->elf64.symtab->sh_size, MADV_WILLNEED);
		madvise(&input_get_mem_map(in)[descr->elf64.strtab->sh_offset], descr->elf64.strtab->sh_size, MADV_RANDOM);
	}

	if ( descr->elf64.dsymtab )
	{
		assert(descr->elf64.dstrtab); // they always go in pairs
		check_sec_size(in, descr, descr->elf64.dsymtab);
		check_str_sec(in, descr, descr->elf64.dstrtab);

		// We're going to be reading symtab sequentially real soon
		madvise(&input_get_mem_map(in)[descr->elf64.dsymtab->sh_offset], descr->elf64.dsymtab->sh_size, MADV_WILLNEED);
		madvise(&input_get_mem_map(in)[descr->elf64.dstrtab->sh_offset], descr->elf64.dstrtab->sh_size, MADV_RANDOM);
	}
}

/**
 * Locates all SYMTAB and their corresponding STRTAB sections and stores
 * pointers to them in input_t. Also converts the section headers to
 * the same endianness as us, if necessary.
 */
extern elf_sections_s*	find_sections_64(input_t* in)
{
	static elf_sections_s 	elf_sec = {0};

	elf_sections_t * descr = &elf_sec;

	Elf64_Ehdr* ehdr = (Elf64_Ehdr*)input_get_mem_map(in);
	if ( ! input_get_is_same_endian(in) )
	{
		// Need to modify the program headers in-place in order for us to be able
		// simply read it even though it is of a different endianness
		make_ehdr_native_endian_64(ehdr);
	}

	if ( ehdr->e_shoff == 0 )
	{
		LOG(INFO) <<  "No section info ";
	}

	if ( ehdr->e_shoff + (Elf64_Off)ehdr->e_shentsize*ehdr->e_shnum > input_get_file_size(in) )
	{
		LOG(INFO) <<  "Section header table goes past end of file (corrupted ELF header?)";
	}

	descr->elf64.sections = (Elf64_Shdr*)&input_get_mem_map(in)[ehdr->e_shoff];
	descr->elf64.shnum = ehdr->e_shnum;

	// Find out about the .shstrtab section:
	if ( ehdr->e_shstrndx == SHN_UNDEF )
	{
		LOG(INFO) <<  "No .strtab section ";
	}

	Elf64_Shdr* sections = descr->elf64.sections;
	assert(sections);

	if ( ! input_get_is_same_endian(in) )
	{
		// Need to modify section headers in-place in order for us to be able
		// simply read them even though they are of a different endianness
		for (int i = 0; i < descr->elf64.shnum; ++i)
		{
			make_shdr_native_endian_64(&sections[i]);
		}
	}

	if ( ehdr->e_shstrndx >= SHN_LORESERVE )
	{
		if ( ehdr->e_shstrndx != SHN_XINDEX )
		{
			LOG(INFO) <<  "Bad .shstrtab section index";
		}
		// actual index is in sh_link field of the first entry
		if ( sections[0].sh_link >= ehdr->e_shnum )
		{
			LOG(INFO) <<  ".shstrtab section index  out of range ";
		}
		descr->elf64.shstrtab = &sections[sections[0].sh_link];


	}
	else if ( ehdr->e_shstrndx >= ehdr->e_shnum )
	{
		LOG(INFO) <<  "Out of range .shstrtab section index (corrupted ELF header?)";
	}
	else
	{
		descr->elf64.shstrtab = &sections[ehdr->e_shstrndx];

	}

	// Check the .shstrtab section
	check_str_sec(in, descr, descr->elf64.shstrtab);

	// Start reading the section table
	if ( ehdr->e_shentsize != sizeof(Elf64_Shdr) )
	{
		LOG(INFO) <<  "Bad section header ";
	}

	find_sym_sec(in, descr);

	return descr;
}

static void	make_sym_same_endian_64(Elf64_Sym* s)
{
	s->st_name = get_uint32(&s->st_name);
	s->st_value = get_uint64(&s->st_value);
	s->st_size = get_uint64(&s->st_size);
	s->st_shndx = get_uint16(&s->st_shndx);
}

static size_t	read_symtab_sec(input_t* in,
				Elf64_Shdr* symtab,
				Elf64_Shdr* strtab,
				symtab_t* syms)
{
	const Elf64_Off symsoff = symtab->sh_offset;
	const size_t symtab_nelem = symtab->sh_size / symtab->sh_entsize;
	size_t syms_idx = 0;
	for( size_t i = 0; i < symtab_nelem; ++i)
	{
		size_t symoff = symsoff + i*symtab->sh_entsize;
		Elf64_Sym* s = (Elf64_Sym*)&input_get_mem_map(in)[symoff];
		if ( !input_get_is_same_endian(in) )
		{
			make_sym_same_endian_64(s);
		}

		int symtype = ELF64_ST_TYPE(s->st_info);
		size_t symval = s->st_value;
		const char * symname = get_str_64(in, strtab, s->st_name);
		syms_idx = symtab_add_sym(syms, symval, symtype, symname);

	}

	return syms_idx;
}

/**
 * Reads in the input ELF file and returns a pointer to the file's symbol table as symtab_t (see).
 * The returned object must be deallocated with symtab_free().
 */
extern symtab_t*	read_in_symtab_64(input_t* in, elf_sections_s* descr)
{
	size_t nsyms = 0;
	if ( descr->elf64.symtab )
	{
		nsyms += descr->elf64.symtab->sh_size / descr->elf64.symtab->sh_entsize;
	}
	if ( descr->elf64.dsymtab )
	{
		nsyms += descr->elf64.dsymtab->sh_size / descr->elf64.dsymtab->sh_entsize;
	}



	symtab_t* symtab = symtab_alloc(nsyms);
	assert(symtab);

	size_t syms_read = 0;
	if ( descr->elf64.symtab )
	{
		syms_read = read_symtab_sec(in, descr->elf64.symtab, descr->elf64.strtab, symtab);
		assert(syms_read <= nsyms);
	}

	if ( descr->elf64.dsymtab )
	{
		syms_read = read_symtab_sec(in, descr->elf64.dsymtab, descr->elf64.dstrtab, symtab);
		assert(syms_read <= nsyms);
	}

	if ( syms_read == 0 )
	{
		//report(NORM, "No symbols found in .symtab and .dynsym; nothing to do.");
		return NULL;
	}

	symtab_sort(symtab);

	return symtab;
}

static const char*	get_sym_name_64(input_t* in, elf_sections_s* descr, int symtab_sec_idx, size_t sym_idx, bool *is_func)
{
	// We expect symtab_sec_idx to point to either symtab or dynsym:
	Elf64_Shdr* symtab = descr->elf64.symtab;
	Elf64_Shdr* strtab = descr->elf64.strtab;
	if ( symtab_sec_idx == descr->elf64.dsymtab_idx )
	{
		symtab = descr->elf64.dsymtab;
		strtab = descr->elf64.dstrtab;
	}
	else if ( symtab_sec_idx != descr->elf64.symtab_idx )
	{
	//	error("relocation section references unknown symtab (section index %d)", symtab_sec_idx);
		return NULL;
	}

	if ( !symtab || !strtab)
	{
		// May happen when relocation doesn't reference symbols and  only has
		// addends
		return NULL;
	}

	size_t symoff = sym_idx*symtab->sh_entsize;
	if ( symoff >= symtab->sh_size )
	{
		const char* sec_name = get_sh_str_64(in, descr, symtab->sh_name);
		//error("offset %d into '%s' of symbol index %d is out of range (%d)",
		//	symoff, sec_name, sym_idx, symtab->sh_size);
	}
	Elf64_Sym* s = (Elf64_Sym*)&input_get_mem_map(in)[symtab->sh_offset + symoff];
	*is_func = (ELF64_ST_TYPE(s->st_info) == STT_FUNC);
	return get_str_64(in, strtab, s->st_name);
}

static void	make_rel_same_endian_64(Elf64_Rel* r)
{
	r->r_offset = get_uint64(&r->r_offset);
	r->r_info   = get_uint64(&r->r_info);
}

static void	make_rela_same_endian_64(Elf64_Rela* r)
{
	r->r_offset = get_uint64(&r->r_offset);
	r->r_info   = get_uint64(&r->r_info);
	r->r_addend = (int64_t)get_uint64(&r->r_addend);
}

/**
 * Processes relocation records in the given input ELF file, adding information to the given symbol table.
 */
extern void process_relocations_64(input_t* in, elf_sections_s* descr, symtab_t* symtab)
{
	for (int i = 0; i < descr->elf64.shnum; ++i)
	{
		Elf64_Shdr* sec = &descr->elf64.sections[i];
		uint32_t typ = sec->sh_type;
		if ( typ == SHT_RELA || typ == SHT_REL )
		{
			madvise(&input_get_mem_map(in)[sec->sh_offset], sec->sh_size, MADV_WILLNEED);
		//	report(DBG, "Processing rel[a] section \"%s\" at index %d", get_sh_str_64(in, descr, sec->sh_name), i);

			uint32_t symtab_sec_idx = sec->sh_link; // this relocation section uses this symtab
			size_t nelem = sec->sh_size / sec->sh_entsize;

			if ( typ == SHT_REL ) // .rel section
			{
				check_sec_size(in, descr, sec);
				Elf64_Rel* relocs = (Elf64_Rel*)&input_get_mem_map(in)[sec->sh_offset];

				for(size_t j = 0; j < nelem; ++j)
				{
					if ( !input_get_is_same_endian(in) )
					{
						make_rel_same_endian_64(&relocs[j]);
					}
					size_t sym_idx = ELF64_R_SYM(relocs[j].r_info);
					bool is_func = false;
					const char* sym_name = get_sym_name_64(in, descr, (int)symtab_sec_idx, sym_idx, &is_func);
					symtab_add_reloc(symtab, relocs[j].r_offset, sym_name, is_func, 0);
				}
			}
			else  // .rela section
			{
				check_sec_size(in, descr, sec);
				Elf64_Rela* relocs = (Elf64_Rela*)&input_get_mem_map(in)[sec->sh_offset];

				for(size_t j = 0; j < nelem; ++j)
				{
					if ( !input_get_is_same_endian(in) )
					{
						make_rela_same_endian_64(&relocs[j]);
					}
					size_t sym_idx = ELF64_R_SYM(relocs[j].r_info);
					bool is_func = false;
					const char* sym_name = get_sym_name_64(in, descr, (int)symtab_sec_idx, sym_idx, &is_func);
					symtab_add_reloc(symtab, relocs[j].r_offset, sym_name, is_func, relocs[j].r_addend);
				}
			}
		}
	}
}

/**
 * Returns the size of the input file in bytes.
 */
unsigned long long	input_get_file_size(input_t* in)
{
	assert(in);
	return in->fsize;
}

/**
 * Returns the pointer to memory-mapped image of the input file.
 */
char *			input_get_mem_map(input_t* in)
{
	assert(in);
	assert(in->map);
	return in->map;
}

/**
 * Returns true if we are the same endianness as the input ELF file.
 */
bool			input_get_is_same_endian(input_t* in)
{
	assert(in);
	return in->same_endian;
}



/**
 * Returns the set of reader functions appropriate for the input given.
 */
struct reader_funcs	input_init_reader_funcs(input_t * in)
{
	struct reader_funcs rdr;
	if (in->is_64) // input is 64-bit ELF
	{
		rdr.find_sections = find_sections_64;
		rdr.process_relocations = process_relocations_64;
		rdr.read_symtab = read_in_symtab_64;
	}

	return rdr;
}

const char*	elf_describe(int type)
{
	switch (type)
	{
	case ET_NONE:
		return "none";

	case ET_REL:
		return "relocatable file";

	case ET_EXEC:
		return "executable file";

	case ET_DYN:
		return "shared object";

	case ET_CORE:
		return "core file";

	default:
		LOG(INFO) <<"unrecognized object type (corrupted ELF header?)";
		return "none";
	}
}

/**
 * For the opened input, returns the set of functions suitable for reading it.
 */
struct reader_funcs	input_read_elf_header(input_t* in)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)in->map; // map must be page-aligned, so no problem with the cast

	// e_ident is the same in either 64- or 32-bit elf; it is also unaffected by endianness
	if (ehdr->e_ident[EI_MAG0] != ELFMAG0 || ehdr->e_ident[EI_MAG1] != ELFMAG1 || ehdr->e_ident[EI_MAG2] != ELFMAG2 || ehdr->e_ident[EI_MAG3] != ELFMAG3)
	{
		LOG(INFO) << "input not ELF: magic number is different";
	}

	in->is_64 = (ehdr->e_ident[EI_CLASS] == ELFCLASS64);

	const bool input_big_endian = ehdr->e_ident[EI_DATA] == ELFDATA2MSB;
	const bool host_big_endian = (((union {unsigned int i; char c; }){1}).c) == 0;
	in->same_endian = (input_big_endian == host_big_endian);

	int typ = in->same_endian ? ehdr->e_type : get_uint16(&ehdr->e_type);
	const char *descr = elf_describe(typ);

	return input_init_reader_funcs(in);
}

////////////////////// Input file reader functions ////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**
 * Returns a 64-bit value pointed to by vp interpreting the bytes as having a different endianness than this program.
 */
uint64_t		get_uint64(void* vp)
{
	char *p = (char *)vp;
	union
	{
		char c[8];
		uint64_t i;
	} u;

	for (size_t j = 0; j < 8; ++j)
	{
		u.c[7 - j] = *(p + j);
	}

	return u.i;
}

/**
 * Returns a 32-bit value pointed to by vp interpreting the bytes as having a different endianness than this program.
 */
uint32_t		get_uint32(void* vp)
{
	char *p = (char *)vp;
	union
	{
		char c[4];
		uint32_t i;
	} u;
	u.c[3] = *(p + 0);
	u.c[2] = *(p + 1);
	u.c[1] = *(p + 2);
	u.c[0] = *(p + 3);
	return u.i;
}

/**
 * Returns a 16-bit value pointed to by vp interpreting the bytes as having a different endianness than this program.
 */
uint16_t		get_uint16(void* vp)
{
	char *p = (char *)vp;
	union
	{
		char c[2];
		uint16_t i;
	} u;
	u.c[1] = *(p + 0);
	u.c[0] = *(p + 1);
	return u.i;
}
void	input_open(input_t& in, const std::string &filename)
{
	in.fd = open(filename.c_str(), O_RDONLY);
	if (in.fd == -1)
	{
		LOG(INFO) << "Cannot open input file " ;
	}

	struct stat sb;
	if (fstat(in.fd, &sb) == -1)
	{
		LOG(INFO) <<"Cannot stat input file";
	}

	if ((sb.st_mode & S_IFMT) == S_IFDIR)
	{
		LOG(INFO) << "Input can not be a directory";
	}

	if (sb.st_size <= 0)
	{
		LOG(INFO) <<  "Input file is empty";
	}

	in.fsize = (size_t)sb.st_size;

	in.map = (char *)mmap(NULL, in.fsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, in.fd, 0);
	if (in.map == MAP_FAILED)
	{
		LOG(INFO) <<  "Cannot read in input file";
	}
}

void	input_close(input_t& in)
{


	if (munmap(in.map, in.fsize) == -1)
	{
		LOG(INFO) <<  "Cannot unmap input file";
	}

	if (in.fd != -1)
	{
		if (close(in.fd) == -1)
		{
			LOG(INFO) << "Cannot close input file";
		}
	}

	in.fd = 0;
	in.map = NULL;
}
