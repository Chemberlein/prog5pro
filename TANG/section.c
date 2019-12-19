#include <stdlib.h>
#include "section.h"

void lire_Section_table(Elf32_info *elf,FILE *file){
		int i;
		int sechnum=elf->header.e_shnum;
		fseek(file,elf->header.e_shoff,SEEK_SET);
		for(i=0;i<sechnum;i++){			
			if(fread(&elf->section[i],sizeof(Elf32_Shdr),1,file)){
				if(elf->header.e_ident[EI_DATA]==2){	
					sectionto_little_endian(elf,i);
				}
			}			
			
		}
} 
void init_strtable(Elf32_info *elf,FILE *file){

	int shoff=elf->header.e_shoff;
	int shstrndx=elf->header.e_shstrndx;
	int shentsize=elf->header.e_shentsize;
	printf("shstrndx %d %d\n",shstrndx,shentsize );
	Elf32_Shdr strtab;

	fseek(file, shoff + shstrndx*shentsize, SEEK_SET);//shoff:  décaler à section header
		//puis + indexe de secton * taille de chaque section = offset global
   	if(fread(&strtab, sizeof(Elf32_Shdr),1 , file)){//get the string table header

   		fseek(file, swap_uint32(strtab.sh_offset), SEEK_SET);
		elf->strtable = (unsigned char *)malloc(sizeof(unsigned char)*swap_uint32(strtab.sh_size));
    	if(fread(elf->strtable, swap_uint32(strtab.sh_size),1, file)){
    	}
    }
}

char * getSectionType(Elf32_Shdr sh){
	char *str = "";
	
	switch(sh.sh_type){
				case SHT_NULL    :str="NULL          ";
					break;
				case SHT_PROGBITS:str="PROGBITS      ";
					break;
				case SHT_SYMTAB  :str="SYMTAB        ";
					break;
				case SHT_STRTAB  :str="STRTAB        ";
					break;
				case SHT_RELA    :str="RELA          ";
					break;
				case SHT_HASH    :str="HASH          ";
					break;
				case SHT_DYNAMIC :str="DYNAMIC       ";
					break;
				case SHT_NOTE    :str="NOTE          ";
					break;
				case SHT_NOBITS  :str="NOBITS        ";
					break;
				case SHT_REL     :str="REL           ";
					break;
				case SHT_SHLIB   :str="SHLIB         ";
					break;
				case SHT_DYNSYM  :str="DYNSYM        ";
					break;
				case SHT_NUM     :str="NUM           ";
					break;
				case SHT_ARM_ATTRIBUTES  :str="ARM_ATTRIBUTES";
					break;
			}			

			return str;

}

char * get_elf_section_flags(Elf32_Word sh_flags){
	  char *flagbuf = malloc(sizeof(char)*20);
      char *cp = flagbuf;
      if (sh_flags & SHF_WRITE)
		*cp++ = 'W';
      if (sh_flags & SHF_ALLOC)
		*cp++ = 'A';
      if (sh_flags & SHF_EXECINSTR)
		*cp++ = 'X';
      if (sh_flags & SHF_MERGE)
		*cp++ = 'M';
      if (sh_flags & SHF_STRINGS)
		*cp++ = 'S';
      if (sh_flags & SHF_INFO_LINK)
		*cp++ = 'I';
      if (sh_flags & SHF_LINK_ORDER)
		*cp++ = 'L';
      if (sh_flags & SHF_OS_NONCONFORMING)
		*cp++ = 'N';
      if (sh_flags & SHF_GROUP)
		*cp++ = 'G';
      if (sh_flags & SHF_TLS)
		*cp++ = 'T';
      if (sh_flags & SHF_ORDERED)
		*cp++ = 'O';
      if (sh_flags & SHF_EXCLUDE)
		*cp++ = 'E';
      *cp = '\0';
      return flagbuf;
}

void sectionto_little_endian(Elf32_info *elf,int i){
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

