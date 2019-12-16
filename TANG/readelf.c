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

int main(int argc, char *argv[]){
	int opt;
	struct option longopts[] = {
		{ "affichage ", required_argument, NULL, 'h' },
		
	};
	
	Elf32_info elf;
	FILE *file;
	
	file = fopen(argv[1],"r");
	if(file==NULL){
		printf("Erreur, entrer un fichier elf en argument.\n");
		exit(1);
	}else{
		while((opt = getopt_long(argc, argv, "h", longopts, NULL)) != -1){
			switch(opt){
			case 'h':
				initElf(&elf,file);
				//etape 1
				afficheHeader(elf);
				break;
			}

		}

	}

	fclose(file);



	return 0;
}
