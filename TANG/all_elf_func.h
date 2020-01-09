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




//libérer la mémoire qui est alloué par la fonction initElf
void liberer(Elf32_info elf);


//Écrire le nombre de section dans un fichier temporaire
void getNbSection(Elf32_info elf);




void insert_bit(FILE *f3, Elf32_info *elf2, int dec_from_be, int size_of_fo, int size2 ,int nr_sec);   // la funtion pour concatener les bits

void insert_bit_end(FILE *f3, Elf32_info *elf2, int nr_sec, int size);   //la function pour ecrir les bits a la fin du fichier

void concatener(Elf32_info *elf1, Elf32_info *elf2, Elf32_info *elf3 ,FILE *f3, FILE *f2) ;   //la functions qui parcours les deux ELF files et les concatenne


//partie 2 

void read_progbits(Elf32_info *elf,FILE *file);

int funForFindIndex(Elf32_info elfo,Elf32_info elft,int ind1);

void p2(Elf32_info elfo,Elf32_info elft);

