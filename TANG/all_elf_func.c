#include <string.h> 
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include "all_elf_func.h"

#define N_ligne 16

void initElf(Elf32_info *elf,FILE *file){
	//lire le contenu de l'en-tête
	if(fread(&elf->header,sizeof(elf->header),1,file)){
			//printf("réussir à lire header\n");
			//if big endian then swap to little endian
			if(elf->header.e_ident[EI_DATA]==2){		
				setto_little_endian(elf);
			}
	}
	if(elf->header.e_shnum == 0){
		printf("\nIl n'y a pas de sections dans ce fichier.\n");
		
	}else{
		elf->section = malloc(sizeof(Elf32_Shdr) * swap_uint16(elf->header.e_shnum)); //allocation de table de section

		lire_Section_table(elf,file);  // lire le contenu de table des sections
		init_strtable(elf,file);		//Alloue la mémoire pour stocker la table contenant les noms des symboles.
	}
	read_Sym(elf,file); //Stocke les informations relatives à la table des symboles dans la structure elf.
	read_SymTable(elf,file); //Alloue la mémoire pour stocker la table contenant les noms des symboles.

	read_Rel(elf,file); // Stocke les informations relatives à la table des relocations dans la structure elf.

	

}

void read_progbits(Elf32_info *elf,FILE *file){
	int tableSize=0;
	for (int i = 0; i < elf->header.e_shnum; ++i){
		if (elf->section[i].sh_type==SHT_PROGBITS){
			tableSize++;
		}
	}
	elf->progbits=malloc(tableSize * sizeof (unsigned char *));
	for(int i = 0;i<elf->header.e_shnum;i++){
		if (elf->section[i].sh_type==SHT_PROGBITS){
              fseek(file,elf->section[i].sh_offset, SEEK_SET);
              elf-> progbits[i] = malloc(elf->section[i].sh_size * sizeof(unsigned char));
              if(fread(elf->progbits[i], 1,elf->section[i].sh_size, file)){

              }      
		}
	}
	if(elf->header.e_ident[EI_DATA]==2){

	}

}
