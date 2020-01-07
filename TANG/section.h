#ifndef __SECTION_H__
#define __SECTION_H__
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "elf.h"
#include "swap.h"
#include "baseElf32.h"

// Stocke les informations relatives aux sections dans la structure elf.
void lire_Section_table(Elf32_info *elf,FILE *file); 

// Alloue la mémoire pour stocker la table contenant les noms des sections.
void init_strtable(Elf32_info *elf,FILE *file);

// Liste de type de section
char * getSectionType(Elf32_Shdr sh);

// Liste de flags de section
char * get_elf_section_flags(Elf32_Word sh_flags);

// si coder en little endian, transformer en big endian
void sectionto_little_endian(Elf32_info *elf,int i);

//etape 2 Affichage de la table des sections
void afficheTableSection(Elf32_info elf,FILE *file);

//etape 3 Affichage du contenu d’une section
void afficher_contenu_section(Elf32_info elf,FILE* fsource);

#endif