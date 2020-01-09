#include <stdlib.h>
#include "section.h"

#define N_ligne 16

void lire_Section_table(Elf32_info *elf,FILE *file){ 
		int i;
		int sechnum=elf->header.e_shnum;
		fseek(file,elf->header.e_shoff,SEEK_SET);
		for(i=0;i<sechnum;i++){			
			if(fread(&elf->section[i],sizeof(Elf32_Shdr),1,file)){  // stocke les informations relatives aux sections
				if(elf->header.e_ident[EI_DATA]==2){	// si en big endian, transformer en little endian
					sectionto_little_endian(elf,i);
				}
			}			
			
		}
} 
void init_strtable(Elf32_info *elf,FILE *file){ 

	Elf32_Shdr strtab = elf->section[elf->header.e_shstrndx];
	fseek(file, strtab.sh_offset, SEEK_SET);
	elf->strtable = (unsigned char *)malloc(sizeof(unsigned char)*strtab.sh_size);
	if(fread(elf->strtable, strtab.sh_size,1, file)){}
 
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


void afficheTableSection(Elf32_info elf,FILE *file){
	printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage  0x%lx:\n", elf.header.e_shnum,(unsigned long )elf.header.e_shoff);
	printf("\n");
	printf("En-tetes de section :\n");

    printf("  [Nr] Nom               Type            Adr      Décala.Taille ES Fan LN Inf Al\n");
    fseek(file,elf.header.e_shoff,SEEK_SET);
    int i;
    char *strFlags;
    for(i=0;i<elf.header.e_shnum;i++){
    	printf("  [ %d] ",i);
		printf("%-16.19s", elf.section[i].sh_name+elf.strtable);
		printf("  %s",getSectionType(elf.section[i]));
		printf("%08x      ",elf.section[i].sh_addr);
		printf("%06x ",elf.section[i].sh_offset);
		printf("%06x ",elf.section[i].sh_size);
		printf("%02x ",elf.section[i].sh_entsize);
		strFlags = get_elf_section_flags(elf.section[i].sh_flags);
		printf("%s",strFlags);
		free(strFlags);
		printf("%d ",elf.section[i].sh_link);
		printf("%d ",elf.section[i].sh_info);
		printf("%d\n",elf.section[i].sh_addralign);		
    }
 	printf("Clé des fanions :\n W (écriture), A (allocation), X (exécution), I (info), M (fusion),  S (chaînes), I (info), \n");
      printf("L (ordre des liens), O (traitement supplémentaire par l'OS requis), G (groupe), \n");
       printf("y (purecode), p (processor specific)\n");

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