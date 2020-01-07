#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "myelf.h"
#include "read_elf_fonctions.h"


int main(int argc, char** argv){

	//int opt;
	if(argc<2){
		printf("Nombre d'arguments incorrect \n");
		exit(1);
	}
	else if(argc>=2){
		FILE *fsource;
		fsource = fopen(argv[1],"r");
		if(fsource==NULL){
			fsource = fopen(argv[2],"r");
		}
		Elf32_info elf;
		
		//initialiser info_elf
		initialiser_elf(&elf,fsource);
		/*
		struct option longopts[] = {
			{ "header", required_argument, NULL, 'h' },
			{ "tableSection", required_argument, NULL, 'S' },
			{ "contenuSection", required_argument, NULL, 'x' },
		};
		*/
		//while ((opt = getopt_long(argc, argv, "h:x:S", longopts, NULL)) != -1){

			switch(argv[2][1]){
				case 'h':
					//affichage de l'en_tête
					afficher_header(elf);
						break;
				case 'S':
					//affichage de la table des sections
					afficher_table_section(elf);
						break;
				case 'x':
					//affichage du contenu d'une section
					afficher_contenu_section(elf,fsource);
						break;
				case 's':
					//affichage du contenu d'une section
					aff_s(elf,fsource);
						break;
				case 'r':
					//affichage du contenu d'une section
					aff_r(elf,fsource);
						break;
			}
			
			
		//}



		
		fclose(fsource)	;
	}

return 0;
}





	
