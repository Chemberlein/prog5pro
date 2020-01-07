#include <stdlib.h>
#include "header.h"

void affiche_Magic(Elf32_info elf){
	for(int i=0;i<16;i++){
		printf("%02x ",elf.header.e_ident[i]);
    }
}

void affiche_Classe(Elf32_info elf){
	for(int i=EI_MAG1;i<=EI_MAG3;i++){
		printf("%c",elf.header.e_ident[i]);

	}
	switch(elf.header.e_ident[EI_CLASS]){
		case ELFCLASSNONE:
			printf("Invalid");
			break;
		case ELFCLASS32:
			printf("32");
			break;
		case ELFCLASS64:
			printf("64");
			break;
	}

}

void affiche_DATA(Elf32_info elf){
	switch(elf.header.e_ident[EI_DATA]){
		case ELFDATANONE: printf("Invalid");
			break;
		case ELFDATA2LSB: printf("2's complement, little endian");
			break;
		case ELFDATA2MSB: printf("2's complement, big endian");
			break;
	}
}

void affiche_Version(Elf32_info elf){
	if(elf.header.e_ident[EI_VERSION]==EV_CURRENT)
		printf("%d(current)\n",elf.header.e_ident[EI_VERSION]);
	printf("  OS/ABI:\t\t\t");
	switch(elf.header.e_ident[EI_OSABI]){
		case ELFOSABI_NONE:printf("UNIX System V ABI\n");
			break;
		case ELFOSABI_LINUX:printf("linux\n");
			break;
	}
	printf("  Version ABI:\t\t\t");
	printf("%d\n",elf.header.e_ident[8]);

}

void affiche_Type(Elf32_info elf){
	switch(elf.header.e_type){
		case ET_NONE: printf("No file");
				break;
		case ET_REL: printf("REL");
				break;
		case ET_EXEC: printf("EXEC");
				break;
		case ET_DYN: printf("DYN");
				break;
		case ET_CORE: printf("CORE");
				break;
		case ET_LOPROC: printf("LOPROC");
				break;
		case ET_HIPROC: printf("HIPROC");
				break;
	}
	//valeur de type : 
	//printf("%02x\n",elf.header.e_type);
}

void affiche_Machine(Elf32_info elf){
	switch(elf.header.e_machine){
		case EM_NONE: printf("No machine");
			break;
		case EM_M32: printf("AT&T WE 32100");
			break;
		case EM_SPARC: printf("SUN SPARC");
			break;
		case EM_386: printf("Intel 80386");
			break;
		case EM_68K: printf("Motorola m68k family ");
			break;
		case EM_88K: printf("Motorola m88k family");
			break;
		case EM_860: printf("Intel 80860");
			break;
		case EM_ARM :printf("ARM");
			break;
		default:printf("unknown");
			break;
	}
	printf("\n");

}

void setto_little_endian(Elf32_info *elf){

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