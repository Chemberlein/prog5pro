#ifndef __BASEELF32_H__
#define __BASEELF32_H__
//structure utilisé
typedef struct{
  	Elf32_Ehdr header;
	Elf32_Shdr *section;
	unsigned char *strtable;
	unsigned char **progbits;
	Elf32_Sym  *symtab;
	unsigned char **symtable;
	Elf32_Rel  **reltab;	
}Elf32_info;

#endif