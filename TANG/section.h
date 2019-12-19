#ifndef __SECTION_H__
#define __SECTION_H__
#include <stdio.h>
#include "elf.h"
#include "swap.h"
#include "baseElf32.h"


void lire_Section_table(Elf32_info *elf,FILE *file);
void init_strtable(Elf32_info *elf,FILE *file);
char * getSectionType(Elf32_Shdr sh);
char * get_elf_section_flags(Elf32_Word sh_flags);
void sectionto_little_endian(Elf32_info *elf,int i);
#endif