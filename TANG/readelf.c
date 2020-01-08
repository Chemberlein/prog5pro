/*
ELF Loader - chargeur/implanteur d'exécutables au format ELF à but pédagogique
Copyright (C) 2012 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "elf.h"
#include "all_elf_func.h"
#include "baseElf32.h"


void usage(char *name) {
	fprintf(stderr, "Usage:\n"
		"%s [ -debug file ] [ -Option ] \n\n"
		
		, name);
}

int main(int argc, char *argv[]){
	int opt;
	struct option longopts[] = {
		{ "affichageHeader ", optional_argument, NULL, 'h' },
		{ "affichageSection ", optional_argument, NULL, 'S' },
		{ "affichageContenuSection ", optional_argument, NULL, 'x'},
		{ "affichage de la table des symboles ", optional_argument, NULL, 's'},
		{ "affichageRelocation ", optional_argument, NULL, 'r'},
		{ "getNombreDeSection", no_argument,NULL,'T'},
		
	};
	
	Elf32_info elf;	
	FILE *file;
	
	file = fopen(argv[1],"r");
	if(file==NULL){
		printf("Erreur, entrer un fichier elf en argument.\n");
		exit(1);
	}else{
		while((opt = getopt_long(argc, argv, "hSxsrT", longopts, NULL)) != -1){
			initElf(&elf,file);
			//printf("optarg = %s\n", optarg);
			
			switch(opt){
			case 'h':				
				//etape 1
				afficheHeader(elf);		
				break;
			case 'S':
				//etape 2
				afficheTableSection(elf,file);				
				break;
			case 'x':
				afficher_contenu_section(elf,file);				
				break;
			case 's':				
				aff_s(elf,file);				
				break;
			case 'r':					
				aff_r(elf,file);				
				break;
			case 'T':
				getNbSection(elf);
				break;
			default:
				fprintf(stderr, "Unrecognized option %c\n", opt);
				usage(argv[0]);
				exit(1);

			}

		}

	}

	fclose(file);
	liberer(&elf);


	return 0;
}
