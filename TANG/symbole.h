#ifndef __SYMBOLE_H__
#define __SYMBOLE_H__
#include <stdio.h>
#include "elf.h"
#include "swap.h"
#include "baseElf32.h"

//Stocke les informations relatives à la table des symboles dans la structure elf.
void read_Sym(Elf32_info *elf,FILE *file);  

//Alloue la mémoire pour stocker la table contenant les noms des symboles.
void read_SymTable(Elf32_info *elf,FILE *file);

//Affichage la valeur de symbole
uint16_t print_val_sym(Elf32_info elf,FILE *file,int k);


//etape 4 Affichage de la table des symboles et des détails relatifs à chaque symbole
void aff_s(Elf32_info elf,FILE *file);


#endif