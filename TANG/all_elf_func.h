#include "elf.h"
#include <stdio.h>

typedef struct{
	Elf32_Ehdr header;
}Elf32_info;

void initElf(Elf32_info *elf,FILE *file);
void affiche_Magic(Elf32_info elf);
void affiche_Classe(Elf32_info elf);
void affiche_Type(Elf32_info elf);
void affiche_DATA(Elf32_info elf);
void affiche_Version(Elf32_info elf);
void affiche_Machine(Elf32_info elf);


void afficheHeader(Elf32_info elf);
void setto_little_endian(Elf32_info *elf);