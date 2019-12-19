#include <string.h> 
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include "all_elf_func.h"

#define N_ligne 16

void initElf(Elf32_info *elf,FILE *file){
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

		lire_Section_table(elf,file);
		init_strtable(elf,file);
	}

	

}



void afficheHeader(Elf32_info elf){
	printf("ELF Header:\n");
 	/*e_ident[]*/
	printf("  Magic number:\t\t");
	//Magic number
    affiche_Magic(elf);
    printf("\n");
    //Classe
    printf("  Classe:\t\t\t");
    affiche_Classe(elf);
    printf("\n");
    //Données
    printf("  Donnees:\t\t");
    affiche_DATA(elf);
    printf("\n");
    //Version
    printf("  Version:\t\t\t");
    affiche_Version(elf);
    /*e_type*/
	printf("  Type:\t\t\t");
    affiche_Type(elf);
    printf("\n");
    /* e_machine */
	printf("  Machine:\t\t\t");
    affiche_Machine(elf);
    int W=0; // Debug purposes
	int ait=W; // Debug purposes
	ait+=ait;

    printf("  Objet file version:            \t\t\t0x%x\n",elf.header.e_version);
	printf("  Adresse du point d'entree:     \t\t\t0x%x\n",elf.header.e_entry);
	printf("  Debut des en-tetes de programme:\t\t%d(octets)\n",elf.header.e_phoff);
	printf("  Debut des en-tetes de section: \t\t\t%d(octets)\n",elf.header.e_shoff);
	printf("  Flags:                       \t\t\t%#02x\n",elf.header.e_flags);
	printf("  Taille de cet en-tete:         \t\t\t%d(octets)\n",elf.header.e_ehsize);
	printf("  Taille de l'en-tete du programme:\t\t\t%d(octets)\n",elf.header.e_phentsize);
	printf("  Nombre d'en-tete du programme:  \t\t\t%d\n",elf.header.e_phnum);
	printf("  Taille des en-tetes de section:\t\t\t%d(octets)\n",elf.header.e_shentsize);
	printf("  Nombre d'en-tetes de section:  \t\t\t%d\n",elf.header.e_shnum);
	printf("  Table d'indexes des chaines d'en-tete de section:\t%d\n",elf.header.e_shstrndx);

}


void afficheTableSection(Elf32_info elf,FILE *file){
	printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage : 0x%lx\n", elf.header.e_shnum,(unsigned long )elf.header.e_shoff);
	printf("\n");
	printf("En-tetes de section :\n");

    printf("[Nr]\tNom\t\t    Type           Adr      Decal  Taille ES Fan\tLN\tInf\tAl\n");
    fseek(file,elf.header.e_shoff,SEEK_SET);
    int i;
    char *strFlags;
    for(i=0;i<elf.header.e_shnum;i++){
    	printf("[%d]\t",i);
		printf("%-20.20s", elf.section[i].sh_name+elf.strtable);
		printf("%s ",getSectionType(elf.section[i]));
		printf("%08x ",elf.section[i].sh_addr);
		printf("%06x ",elf.section[i].sh_offset);
		printf("%06x ",elf.section[i].sh_size);
		printf("%02x ",elf.section[i].sh_entsize);
		strFlags = get_elf_section_flags(elf.section[i].sh_flags);
		printf("%s\t\t",strFlags);
		free(strFlags);
		printf("%d\t",elf.section[i].sh_link);
		printf("%d\t",elf.section[i].sh_info);
		printf("%d\n",elf.section[i].sh_addralign);		
    }
 	printf("Clé des fanions :\n W (écriture), A (allocation), X (exécution), I (info), M (fusion)\n");
}

void afficher_contenu_section(Elf32_info elf,FILE* fsource){
	int i=0;
	char nom[32];
	int numero=-1;
	printf("Entrez un numéro ou un nom de section pour afficher le contenu:\n");		
	if(scanf("%s",nom)==0)
		exit(1);
	if(nom[0]>='0' && nom[0]<='9' ){
		numero = atoi(nom);
	}
	else{											//recupérer le numero à l'aide du nom
		while((numero==-1)&&(i<elf.header.e_shnum)){
			if(!strcmp(nom,(char*)elf.strtable+elf.section[i].sh_name)){
				numero = i;
			}
			i++;
		}
	}
	if(numero<0 || numero>=elf.header.e_shnum){
		printf("La section saisie n'existe pas!\n");	exit(1);
	}	
	int offset = elf.section[numero].sh_offset;		//dacalage de début de section[numero]
	int size = elf.section[numero].sh_size;			//taille de section[numero]
	int adresse = elf.section[numero].sh_addr;		//adresse de section[numero]
	if(size==0){
		printf("La section « %s » n'a pas de données à vidanger.\n",(char*)elf.strtable+elf.section[numero].sh_name);
		exit(1);
	}
	printf("Vidange hexadécimale de la section« %s »:\n",(char*)elf.strtable+elf.section[numero].sh_name);
	
	fseek(fsource, offset , SEEK_SET);
	unsigned char buf[N_ligne];
	int count_fread;
	while(size>0){							//pour chaque ligne faire:
		printf(" 0x%08x  ",adresse);				//debut d'adresse de chaque ligne
		count_fread = fread(buf,1,sizeof(buf),fsource);  	
		if(count_fread==0)
			exit(1);
		size-=N_ligne;	adresse+=N_ligne;
		for(i=0;i<sizeof(buf);i++){
            if(i<count_fread){
       		 printf("%02x",buf[i]);
            }
            else{
              printf("   ");
            }
			if((i+1)%4==0)
				printf(" ");
	    	
   		 }

		//printf("  %s",buf);
		for(i=0;i<sizeof(buf);i++){
		        if(i<count_fread)
		 			 printf("%c",isprint(buf[i])?buf[i]:'.');
		        else
		        	printf(" ");
	 
		}
		printf("\n");
	}
}	