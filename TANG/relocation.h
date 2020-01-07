#ifndef __RELOCATION_H__
#define __RELOCATION_H__
#include <stdio.h>
#include "elf.h"
#include "swap.h"
#include "baseElf32.h"

//Stocke les informations relatives à la table des relocations dans la structure elf.
void read_Rel(Elf32_info *elf,FILE *file);

//etape 5 Affichage des tables de réimplantation et des d ́etails relatifs à chaque entré.
void aff_r(Elf32_info elf,FILE *file) ;

#endif