#ifndef __BASEELF32_H__
#define __BASEELF32_H__
typedef struct{
  Elf32_Ehdr header;
  Elf32_Shdr *section;
  unsigned char *strtable;
}Elf32_info;

#endif