#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <ctype.h>
#include "read_elf_fonctions.h"

#define N_ligne 16

void initialiser_elf(Elf32_info *elf,FILE *fsource){
	size_t res = fread(&elf->header,sizeof(elf->header),1,fsource);		//lire header
	if(res==0)
		exit(1);
	if(elf->header.e_ident[EI_DATA]==2)	
		setup_little_endian(elf);

	lire_strtab(elf,fsource);
	lire_table_section(elf,fsource);					//lire table section	
}




void lire_strtab(Elf32_info *elf,FILE *fsource){

	int shoff=elf->header.e_shoff;			//section setoff
	int shstrndx=elf->header.e_shstrndx;	//indice des chaines des sections
	int shentsize=elf->header.e_shentsize;	//taille de section
	Elf32_Shdr strtab;

	fseek(fsource, shoff + shstrndx*shentsize, SEEK_SET);
	size_t res=fread(&strtab, sizeof(char), sizeof(Elf32_Shdr), fsource);//get the string table header  
	if(res==0)
		exit(1);
	fseek(fsource, swap_uint32(strtab.sh_offset), SEEK_SET);
	elf->strtable = (unsigned char *)malloc(sizeof(unsigned char)*swap_uint32(strtab.sh_size));
	res=fread(elf->strtable, sizeof(char), swap_uint32(strtab.sh_size), fsource);	
	if(res==0)
		exit(1);
}

void lire_table_section(Elf32_info *elf,FILE *fsource){

	int sec_num = elf->header.e_shnum;
	elf->section = malloc(sec_num*(sizeof(Elf32_Shdr)));
	fseek(fsource,elf->header.e_shoff,SEEK_SET);
	for(int i=0;i<sec_num;i++){
		size_t res = fread(&elf->section[i],sizeof(char),sizeof(Elf32_Shdr),fsource);
		if(res==0)
			exit(1);
		if(elf->header.e_ident[EI_DATA]==2){	
			elf->section[i].sh_name = swap_uint32(elf->section[i].sh_name);
			elf->section[i].sh_type = swap_uint32(elf->section[i].sh_type);
			elf->section[i].sh_addr = swap_uint32(elf->section[i].sh_addr);
			elf->section[i].sh_offset = swap_uint32(elf->section[i].sh_offset);
			elf->section[i].sh_size = swap_uint32(elf->section[i].sh_size);
			elf->section[i].sh_entsize = swap_uint32(elf->section[i].sh_entsize);
			elf->section[i].sh_flags = swap_uint32(elf->section[i].sh_flags);
			elf->section[i].sh_link = swap_uint32(elf->section[i].sh_link);
			elf->section[i].sh_info = swap_uint32(elf->section[i].sh_info);
			elf->section[i].sh_addralign = swap_uint32(elf->section[i].sh_addralign);
		}
	}
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
		printf("La section « %s » n'a pas de données à vidanger.\n",(char*)elf.strtable+elf.section[numero].sh_name);
		exit(1);
	}
	printf("Vidange hexadécimale de la section« %s »:\n",(char*)elf.strtable+elf.section[numero].sh_name);
	
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


void afficher_table_section(Elf32_info elf){
	printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage %#x\n\n:",elf.header.e_shnum,elf.header.e_shoff);
	printf("En-têtes de section :\n");
	printf("[Nr]\tNom\t\t    Type           Adr      Decal  Taille ES Fan\tLN\tInf\tAl\n");
	for(int i=0;i<elf.header.e_shnum;i++){
		printf("[%d]\t",i);
		printf("%-20.20s",elf.strtable+elf.section[i].sh_name);
		char* type="";						//recupérer type
		switch(elf.section[i].sh_type){
			case SHT_NULL    :type="NULL          ";	break;
			case SHT_PROGBITS:type="PROGBITS      ";	break;
			case SHT_SYMTAB  :type="SYMTAB        ";	break;
			case SHT_STRTAB  :type="STRTAB        ";	break;
			case SHT_RELA    :type="RELA          ";	break;
			case SHT_HASH    :type="HASH          ";	break;
			case SHT_DYNAMIC :type="DYNAMIC       ";	break;
			case SHT_NOTE    :type="NOTE          ";	break;
			case SHT_NOBITS  :type="NOBITS        ";	break;
			case SHT_REL     :type="REL           ";	break;
			case SHT_SHLIB   :type="SHLIB         ";	break;
			case SHT_DYNSYM  :type="DYNSYM        ";	break;
			case SHT_NUM     :type="NUM           ";	break;
			case SHT_ARM_ATTRIBUTES  :type="ARM_ATTRIBUTES";break;
		}
		printf("%s ",type);
		printf("%08x ",elf.section[i].sh_addr);
		printf("%06x ",elf.section[i].sh_offset);
		printf("%06x ",elf.section[i].sh_size);
		printf("%02x ",elf.section[i].sh_entsize);
		int flag_int = elf.section[i].sh_flags;			//recupérer flag
		char* flag=malloc(sizeof(char)*4);
		if(flag_int & SHF_MASKPROC)
			strcat(flag,"M");
		if(flag_int & SHF_INFO_LINK)
			strcat(flag,"I");
		if(flag_int & SHF_EXECINSTR)
			strcat(flag,"X");
		if(flag_int & SHF_ALLOC)
			strcat(flag,"A");
		if(flag_int & SHF_WRITE)
			strcat(flag,"W");
		printf("%s\t\t",flag);	free(flag);
		printf("%d\t",elf.section[i].sh_link);
		printf("%d\t",elf.section[i].sh_info);
		printf("%d\n",elf.section[i].sh_addralign);
	}
	printf("Clé des fanions :\n W (écriture), A (allocation), X (exécution), I (info), M (fusion)\n");
}


void afficher_header(Elf32_info elf){
	int i;
	printf("\tMagique: ");								//magic
		for(int i=0;i<16;i++){
			printf("%02x ",elf.header.e_ident[i]);
		}
	printf("\n");
	printf("\tClasse:\t\t\t");							//class
	for(i=EI_MAG1;i<=EI_MAG3;i++){
		printf("%c",elf.header.e_ident[i]);
	}
	switch(elf.header.e_ident[EI_CLASS]){				
		case ELFCLASSNONE:printf("classe invalide\n");break;
		case ELFCLASS32:printf("32-BIT\n");break;
		case ELFCLASS64:printf("64-BIT\n");break;
		case ELFCLASSNUM:printf("NUM\n");break;
	}
	printf("\tDonnées:\t\t");							//data
	switch(elf.header.e_ident[EI_DATA]){				
		case 0:printf("données codées invalides\n");break;
		case 1:printf("little endian\n");break;
		case 2:printf("big endian\n");break;
	}
	printf("\tVersion:\t\t%d",elf.header.e_ident[EI_VERSION]);	//version
	switch(elf.header.e_ident[EI_VERSION]){
		case 0:printf("(NONE)\n");break;
		case 1:printf("(CURRENT)\n");break;
		case 2:printf("(NUM)\n");break;
	}
	printf("\tOS/ABI:\t\t\t");							//OS
	switch(elf.header.e_ident[EI_OSABI]){
		case ELFOSABI_NONE:printf("UNIX - System V\n");break;
		case ELFOSABI_LINUX:printf("LINUX - System\n");break;
	}
	printf("\tVersion ABI\t\t%d\n",elf.header.e_ident[8]);	//ABI version
	printf("\tType:\t\t\t");							//type
	switch(elf.header.e_type){
		case ET_NONE: printf("NONE\n");break;
		case ET_REL: printf("REL\n");break;
		case ET_EXEC: printf("EXEC\n");break;
		case ET_DYN: printf("DYN\n");break;
		case ET_CORE: printf("CORE\n");break;
		case ET_LOPROC: printf("LOPROC\n");break;
		case ET_HIPROC: printf("HIPROC\n");break;
	}
	printf("\tMachine:\t\t");							//machine
	switch(elf.header.e_machine){
		case EM_NONE: printf("No machine");break;
		case EM_M32: printf("AT&T WE 32100");break;
		case EM_SPARC: printf("SUN SPARC");break;
		case EM_386: printf("Intel 80386");break;
		case EM_68K: printf("Motorola m68k family ");break;
		case EM_88K: printf("Motorola m88k family");break;
		case EM_860: printf("Intel 80860");break;
		case EM_ARM :printf("ARM");break;
		default:printf("unknown");break;
	}
	printf("\n");
	printf("\tVersion:\t\t%#02x\n",elf.header.e_version);		//e_version
	printf("\tAdresse du point d'entrée:\t\t%#02x\n",elf.header.e_entry);
	printf("\tDébut des en-têtes de programme:\t%d (octets dans le fichier)\n",elf.header.e_phoff);
	printf("\tDébut des en-têtes de section:\t\t%d (octets dans le fichier)\n",elf.header.e_shoff);
	printf("\tFanions:\t\t\t\t%#08x\n",elf.header.e_flags);
	printf("\tTaille de cet en-tête:\t\t\t%d(octets)\n",elf.header.e_ehsize);
	printf("\tTaille de l'en-tête du programme:\t%d (octets)\n",elf.header.e_phentsize);
	printf("\tNombre d'en-tête du programme:\t\t%d\n",elf.header.e_phnum);
	printf("\tTaille de l'en-tête de section:\t\t%d (octets)\n",elf.header.e_shentsize);
	printf("\tNombre d'en-tête de section:\t\t%d\n",elf.header.e_shnum);
	printf("\tTable d'indexes des chaînes d'en-tête de section: %d\n",elf.header.e_shstrndx);
	
}





































void setup_little_endian(Elf32_info *elf){

	elf->header.e_type = swap_uint16(elf->header.e_type);
	elf->header.e_machine = swap_uint16(elf->header.e_machine);
	elf->header.e_version = swap_uint32(elf->header.e_version);
	elf->header.e_entry = swap_uint32(elf->header.e_entry);
	elf->header.e_phoff = swap_uint32(elf->header.e_phoff);
	elf->header.e_shoff = swap_uint32(elf->header.e_shoff);
	elf->header.e_flags = swap_int32(elf->header.e_flags);
	elf->header.e_ehsize = swap_uint16(elf->header.e_ehsize);
	elf->header.e_phentsize = swap_uint16(elf->header.e_phentsize);
	elf->header.e_phnum = swap_uint16(elf->header.e_phnum);
	elf->header.e_shentsize = swap_uint16(elf->header.e_shentsize);
	elf->header.e_shnum = swap_uint16(elf->header.e_shnum);
	elf->header.e_shstrndx = swap_uint16(elf->header.e_shstrndx);
}
//! Byte swap unsigned short
uint16_t swap_uint16( uint16_t val ) 
{
    return (val << 8) | (val >> 8 );
}

//! Byte swap short
int16_t swap_int16( int16_t val ) 
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

//! Byte swap unsigned int
uint32_t swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
}

//! Byte swap int
int32_t swap_int32( int32_t val )
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | ((val >> 16) & 0xFFFF);
}


