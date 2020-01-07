#ifndef __HEADER_H__
#define __HEADER_H__
#include <stdio.h>
#include "elf.h"
#include "swap.h"
#include "baseElf32.h"

//affichage de l'en -tête
void affiche_Magic(Elf32_info elf);
void affiche_Classe(Elf32_info elf);
void affiche_Type(Elf32_info elf);
void affiche_DATA(Elf32_info elf);
void affiche_Version(Elf32_info elf);
void affiche_Machine(Elf32_info elf);
// si coder en big endian, transformer en little endian pour l'affichage correcte
void setto_little_endian(Elf32_info *elf);


//etape 1 Affichage de l'en - tête 
void afficheHeader(Elf32_info elf);


#endif