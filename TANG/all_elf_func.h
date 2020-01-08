#include <stdio.h>
#include "elf.h"
#include "header.h"
#include "section.h"
#include "symbole.h"
#include "relocation.h"
#include "swap.h"
#include "baseElf32.h"
/*
	+ Initialise une structure Elf32_info contenant les information relative au fichier ELF ( header, section, table des symboles 
	et leurs noms ainsi que la table des relocations ).
	+ Appelle toutes les fonctions nécessaires qui récupèrent les données.
	+ Alloue la mémoire nécéssaire pour les informations du header ainsi que les sections.
	+ Convertit les valeurs en little endian si elles sont en big endian.
*/
void initElf(Elf32_info *elf,FILE *file);

//
void read_progbits(Elf32_info *elf,FILE *file);


//libérer la mémoire qui est alloué par la fonction initElf
void liberer(Elf32_info *elf);


//Écrire le nombre de section dans un fichier temporaire
void getNbSection(Elf32_info elf);





