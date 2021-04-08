
/*
  This is free and unencumbered software released into the public domain.

  Anyone is free to copy, modify, publish, use, compile, sell, or
  distribute this software, either in source code form or as a compiled
  binary, for any purpose, commercial or non-commercial, and by any
  means.

  In jurisdictions that recognize copyright laws, the author or authors
  of this software dedicate any and all copyright interest in the
  software to the public domain. We make this dedication for the benefit
  of the public at large and to the detriment of our heirs and
  successors. We intend this dedication to be an overt act of
  relinquishment in perpetuity of all present and future rights to this
  software under copyright law.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.

  For more information, please refer to <http://unlicense.org/>
*/

#ifndef DEPINPUT_H_
#define DEPINPUT_H_




#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <elf.h>
#include "depinput.h"
#include "../utils/easylogging++.h"

struct input_s {
    input_s():fd(-1),fsize(0),map(nullptr),same_endian(true),is_64(true) {

    }
    int 			fd;
	unsigned long long 	fsize;

	char * 			map;

	bool			same_endian;
	bool			is_64;
};
typedef struct input_s input_t;
typedef struct symtab_s		symtab_t;
typedef struct elf_sections_s 	elf_sections_t;




struct 	reader_funcs
{

	elf_sections_t * 	(*find_sections)(input_t*);


	symtab_t *		(*read_symtab)(input_t*, elf_sections_t* );

	void			(*process_relocations)(input_t*, elf_sections_t*, symtab_t*);
};


input_t *	input_init(void);
void	input_open(input_t& in, const std::string &filename);
void	input_close(input_t& in);
struct reader_funcs	input_read_elf_header(input_t* in);


unsigned long long	input_get_file_size(input_t* in);
char *			input_get_mem_map(input_t* in);
bool			input_get_is_same_endian(input_t* in);


uint16_t	get_uint16(void* ptr);
uint32_t	get_uint32(void* ptr);
uint64_t	get_uint64(void* ptr);

elf_sections_t*		find_sections_64(input_t* in);
symtab_t*		read_in_symtab_64(input_t* in, elf_sections_t*);
void			process_relocations_64(input_t* in, elf_sections_t*, symtab_t*);

#endif