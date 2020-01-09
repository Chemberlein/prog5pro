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
		"Available option:  \n "
 		"-h, : AffichageHeader \n"
 		"-S, : AffichageSection \n"
 		"-x, : AffichageContenuSection \n"
 		"-s, : Affichage de la table des symboles \n"
 		"-r, : AffichageRelocation  \n" 		
 		"-T, : Obtenir le nombre de Section de fichier elf indiqué  \n"	
 		"default : Display this help and exit  \n"	
		, name);
}

int main(int argc, char *argv[]){
	if(argc == 3){  //partie 1 les affichage
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
		liberer(elf);
	}
	if(argc == 4){
		if(strcmp(argv[3],"-f")==0){
		/* vlad version
			FILE *fsource1;
		    FILE *fsource2;
		    FILE *fsource3;

		    Elf32_info elf1;
		    Elf32_info elf2;
		    Elf32_info elf3;

		    int ch;

		    fsource1 = fopen(argv[1],"r");
		    if (fsource1 == NULL) {
		      printf("error d'ouverture\n" );
		      exit (1);}

		    fsource2 = fopen(argv[2],"r");
		    if (fsource2 == NULL) {
		      printf("error d'ouverture\n" );
		      exit (1);}

		    fsource3 = fopen("concatenation.o","w+");
		    if (fsource1 == NULL) {
		      printf("error d'ouverture\n" );
		      exit (1);}


		    while( ( ch = fgetc(fsource1) ) != EOF ){ // on fait une copie du premier ELF, dans laquelle on va faire la concatenation
		      fputc(ch, fsource3);}

		    fseek(fsource1, 0, SEEK_SET);
		    fseek(fsource3, 0, SEEK_SET);


		  	initElf(&elf1,fsource1);

		    initElf(&elf2,fsource2);

		    initElf(&elf3,fsource3);

		    concatener(&elf1, &elf2, &elf3, fsource3, fsource2);

		  //  afficher_table_section(elf1);
		  //  afficher_table_section(elf2);

		    afficheTableSection(elf3,fsource3);


			fclose(fsource1);
		    fclose(fsource2);
		    fclose(fsource3);
		    liberer(elf1);
		    liberer(elf2);
		    liberer(elf3);*/

		    //version de kirill
		    FILE *f1;
			FILE *f2;
			f1=fopen(argv[1],"r");
			f2=fopen(argv[2],"r");
			Elf32_info elf1;
			Elf32_info elf2;
			initElf(&elf1,f1);
			initElf(&elf2,f2);
			/*if (elf1.header.e_shnum>elf2.header.e_shnum){
				p2e6e8(elf1,elf2);
			}else{
				p2e6e8(elf2,elf1);
			}
			if (elf1.header.e_shnum>elf2.header.e_shnum){
				p2e7(elf1,elf2);
			}else{
				p2e7(elf2,elf1);
			}*/
			if (elf1.header.e_shnum>elf2.header.e_shnum){
				p2(elf1,elf2);
			}else{
				p2(elf2,elf1);
			}
			fclose(f1);
			fclose(f2);
			liberer(elf1);
		    liberer(elf2);
		}

	}


	return 0;
}
