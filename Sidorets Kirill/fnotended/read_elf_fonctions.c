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
	lire_table_section(elf,fsource);
	read_progbits(elf,fsource);
	read_Sym(elf,fsource);
	read_SymTable(elf,fsource);
	read_Rel(elf,fsource);
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
	elf->strtable = (unsigned char *)malloc(sizeof(unsigned char)*swap_uint32(strtab.sh_size));//????this will work only if  we need do swap why you donbt use elf->section[elf->heder.e_shstrndx]????fix this??
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



void read_Sym(Elf32_info *elf,FILE *file){
	int nbSym=0;
	int tableSize=0;
	uint32_t  max =0;
	for(int i=0;i<elf->header.e_shnum;i++){
		if(elf->section[i].sh_type==SHT_SYMTAB){
			nbSym=(elf->section[i].sh_size/elf->section[i].sh_entsize)+nbSym;
		}
	}
	elf->symtab=malloc(sizeof(Elf32_Sym)*nbSym);
	for(int i = 0;i<elf->header.e_shnum;i++){
		if(elf->section[i].sh_type==SHT_SYMTAB){
			nbSym=elf->section[i].sh_size/elf->section[i].sh_entsize;
			for (int j = 0 ; j< nbSym;j++){
				fseek(file,elf->section[i].sh_offset + j*sizeof(Elf32_Sym),SEEK_SET);
				size_t b = fread(&elf->symtab[j],sizeof(char),sizeof(Elf32_Sym),file);
				if(b==0){printf("Error\n");
									exit(1);
				}
				tableSize++;
				if(max<elf->symtab[j].st_size){
					max=elf->symtab[j].st_size;
				}
				if(elf->header.e_ident[EI_DATA]==2){
					elf->symtab[j].st_name=swap_uint32(elf->symtab[j].st_name);
					elf->symtab[j].st_value=swap_uint32(elf->symtab[j].st_value);
					elf->symtab[j].st_size=swap_uint32(elf->symtab[j].st_size);
					elf->symtab[j].st_shndx=swap_uint16(elf->symtab[j].st_shndx);
				}
			}
		}
	}
	elf-> symtable =malloc (tableSize * sizeof (unsigned char *));
}

void read_SymTable(Elf32_info *elf,FILE *file){
	Elf32_Shdr SymShdr;
	for(int i = 0;i<elf->header.e_shnum;i++){
		if (elf->section[i].sh_type==SHT_SYMTAB){
              fseek(file,elf->header.e_shoff+elf->section[i].sh_link*elf->header.e_shentsize, SEEK_SET);
              char* SymNames = NULL;
              const char* nameSym = "";
              fread(&SymShdr,1,elf->header.e_shentsize,file);
              SymNames = malloc(__bswap_32(SymShdr.sh_size));
              fseek(file,__bswap_32(SymShdr.sh_offset), SEEK_SET);
              elf-> symtable[i] = (char *)malloc (__bswap_32(SymShdr.sh_size) * sizeof(unsigned char));
              fread(elf->symtable[i], 1, __bswap_32(SymShdr.sh_size), file);
		}
	}
}

void read_progbits(Elf32_info *elf,FILE *file){
	int nb=0;
	int tableSize=0;
	for(int i=0;i<elf->header.e_shnum;i++){
		if(elf->section[i].sh_type==SHT_SYMTAB){
			nb=(elf->section[i].sh_size/elf->section[i].sh_entsize)+nb;
		}
	}
	for (int i = 0; i < elf->header.e_shnum; ++i){
		if ((elf->section[i].sh_type==SHT_PROGBITS)||(elf->section[i].sh_type==SHT_NULL)||(elf->section[i].sh_type==SHT_STRTAB)||(elf->section[i].sh_type==SHT_ARM_ATTRIBUTES)||(elf->section[i].sh_type==SHT_REL)){
			for (int j = 0;j<nb;j++)tableSize++;
		}
	}
	elf->progbits=malloc(tableSize * sizeof (unsigned char *));
	for(int i = 0;i<elf->header.e_shnum;i++){
		if ((elf->section[i].sh_type==SHT_PROGBITS)||(elf->section[i].sh_type==SHT_NULL)||(elf->section[i].sh_type==SHT_STRTAB)||(elf->section[i].sh_type==SHT_ARM_ATTRIBUTES)||(elf->section[i].sh_type==SHT_REL)){
              fseek(file,elf->section[i].sh_offset, SEEK_SET);
              elf->progbits[i] = malloc(elf->section[i].sh_size * sizeof(unsigned char));
              fread(elf->progbits[i], 1,elf->section[i].sh_size, file);
		}
	}
}

void read_Rel(Elf32_info *elf,FILE *file){
	int size=0;
	int nb=0;
	int nr_valeurs;
	elf->reltab=malloc(elf->header.e_shnum*sizeof(Elf32_Rel *));
  for (int i = 0; i < elf->header.e_shnum; i++){
      if (elf->section[i].sh_type==SHT_REL){
        nr_valeurs = elf->section[i].sh_size / elf->section[i].sh_entsize;
		elf->reltab[i]=malloc(nr_valeurs*sizeof(Elf32_Rel));
		for (int j=0 ; j<nr_valeurs; j++){
		  fseek(file, elf->section[i].sh_offset + j * sizeof(Elf32_Rel), SEEK_SET);
          size_t b=fread(&elf->reltab[i][j],sizeof(char), sizeof(Elf32_Rel), file);
          if(b==0){
          	printf("ERROR\n");
          	exit(1);
          }if(elf->header.e_ident[EI_DATA]==2){
          	elf->reltab[i][j].r_offset=swap_uint32(elf->reltab[i][j].r_offset);
          	elf->reltab[i][j].r_info=swap_uint32(elf->reltab[i][j].r_info);
          }
      }
    }
    }
}

void aff_s(Elf32_info elf,FILE *file) {
  int nr_valeurs;
  for (int i = 0; i < elf.header.e_shnum; i++){
    if (elf.section[i].sh_type==SHT_SYMTAB){
        nr_valeurs = elf.section[i].sh_size / elf.section[i].sh_entsize;
        printf("La table de symboles < %s > contient %d entrées: \n",(char*)elf.strtable+elf.section[i].sh_name,nr_valeurs);
        printf("Num: Valeur      Tail  Type     Lien   Vis         Ndx  Nom   \n");
            for (int j=0; j < nr_valeurs; j++){
            	printf("%-5d",j  );
              	printf("0x%08x  ", elf.symtab[j].st_value );
              printf("%-5d ",elf.symtab[j].st_size);
              switch(ELF32_ST_TYPE(elf.symtab[j].st_info)){
                case STT_SECTION:
                  printf("SECTION   ");
                  break;

                case STT_NOTYPE:
                  printf("NOTYPE    ");
                  break;

                default:

                  printf("ERROR   ");
                  break;}

              switch(ELF32_ST_TYPE(ELF32_ST_BIND(elf.symtab[j].st_info))){
                case STB_LOCAL:
                  printf("LOCAL  ");
                  break;

                case STB_GLOBAL:
                  printf("GLOBAL ");
                  break;

                default:
                  printf("ERROR   ");
                  break;}

              if (elf.symtab[j].st_other==0)
                printf("DEFAULT     ");
              else
                printf("NOT DEFAULT ");

              printf("%d ", elf.symtab[j].st_shndx);
              if(elf.symtab[j].st_name)
              	printf("%s",(char*)elf.symtable[i]+elf.symtab[j].st_name);
          	  printf("\n");
          }
      }
  }
}





uint16_t print_val_sym(Elf32_info elf,FILE *file,int k) {
  for (int i = 0; i <elf.header.e_shnum; i++){
    if (elf.section[i].sh_type==SHT_SYMTAB){
              printf("%d-0x%08x  ", k,elf.symtab[k].st_value );
                if (elf.symtab[k].st_name){
                  printf("%s",(char*)elf.symtable[i]+elf.symtab[k].st_name);
                  return 0;
                }
                 return elf.symtab[k].st_shndx;
                   }
}
return 0;}


void aff_r(Elf32_info elf,FILE *file) {
  int m;
  uint16_t boo=0;
  int nr_valeurs;
  for (int i = 0; i < elf.header.e_shnum; i++){
      if (elf.section[i].sh_type==SHT_REL){
        nr_valeurs = elf.section[i].sh_size / elf.section[i].sh_entsize;

        printf("Section de readressage ' %s ' a l'adresse de decalage: 0x%x contient %d entree:\n\n",(char*)elf.strtable+elf.section[i].sh_name, elf.section[i].sh_offset,  nr_valeurs);
        printf("Decalage     Info         Type         Val.-sym       Noms-symboles\n");
        for (int j=0 ; j<nr_valeurs; j++){
          printf("0x%08x   ",elf.reltab[i][j].r_offset);
          printf("0x%08x   ",elf.reltab[i][j].r_info);
          switch (ELF32_R_TYPE(elf.reltab[i][j].r_info)) {
          case R_ARM_NONE: printf("%15.15s \t","R_ARM_NONE"); break;
          case R_ARM_PC24: printf("%15.15s \t","R_ARM_PC24"); break;
          case R_ARM_ABS32: printf("%15.15s \t","R_ARM_ABS32"); break;
          case R_ARM_REL32: printf("%15.15s \t","R_ARM_REL32"); break;
          case R_ARM_PLT32: printf("%15.15s \t","R_ARM_PLT32");break;
          case R_ARM_CALL: printf("%15.15s \t","R_ARM_CALL");break;
          case R_ARM_JUMP24: printf("%-15s \t","R_ARM_JUMP24");break;
          case R_ARM_THM_JUMP24: printf("%15.15s \t","R_ARM_THM_JUMP24");break;
          default: printf("%15.15s \t", "type non connu!"); break;
          }

          boo=print_val_sym(elf,file, ELF32_R_SYM(elf.reltab[i][j].r_info));
          if(boo!=0){
          if (elf.section[boo].sh_name){printf("%s",(char*)elf.strtable+elf.section[boo].sh_name);}

          }
printf("\n");


        }


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


int funForFindIndex(Elf32_info elfo,Elf32_info elft,int ind1){
	for (int i = 0; i < elft.header.e_shnum; i++) {
		char *name1=elfo.strtable+elfo.section[ind1].sh_name;
		char *name2=elft.strtable+elft.section[i].sh_name;
		if(strcmp(name1,name2)==0){
			return i;
		}
	}
	return -1;
}

void p2(Elf32_info elfo,Elf32_info elft){
	int ind;
	int nbsect=0;
	Elf32_Ehdr header;
	unsigned char *strtable;
	unsigned char **progbits;
	Elf32_Shdr *section;
	char *name=elfo.strtable+elfo.section[elfo.header.e_shstrndx].sh_name;
	uint16_t e_shstrndx;
	uint32_t e_shoff=0;
	//############## don't need change or same #########################3
	Elf32_Sym  *symtab=elfo.symtab;
	memcpy(header.e_ident,elfo.header.e_ident,sizeof(elfo.header.e_ident));
	header.e_type=elfo.header.e_type;
	header.e_machine=elfo.header.e_machine;
	header.e_version=elfo.header.e_version;
	header.e_entry=elfo.header.e_entry;
	header.e_phoff=elfo.header.e_phoff;
	header.e_flags=elfo.header.e_flags;
	header.e_ehsize=elfo.header.e_ehsize;
	header.e_phnum=elfo.header.e_phnum;
	header.e_phentsize=elfo.header.e_phentsize;
	header.e_shentsize=elfo.header.e_shentsize;
	//##############################################################

	for (int i = 0; i < elfo.header.e_shnum; i++)
		if((elfo.section[i].sh_type==SHT_STRTAB)||(elfo.section[i].sh_type==SHT_ARM_ATTRIBUTES)||(elfo.section[i].sh_type==SHT_PROGBITS)||(elfo.section[i].sh_type==SHT_NULL)||(elfo.section[i].sh_type==SHT_REL))nbsect++;
	for (int i = 0; i < elft.header.e_shnum; i++){
		char *name1=elft.strtable+elft.section[i].sh_name;
		if((elft.section[i].sh_type==SHT_STRTAB)||(elft.section[i].sh_type==SHT_ARM_ATTRIBUTES)||(elft.section[i].sh_type==SHT_PROGBITS)||(elft.section[i].sh_type==SHT_NULL)||(elft.section[i].sh_type==SHT_REL)){
			ind=funForFindIndex(elft,elfo,i);
			if(ind==-1)printf("[%s]",name1);nbsect++;	
		}
	}
	progbits=malloc((nbsect) * sizeof (unsigned char *));
	section= malloc(nbsect*(sizeof(Elf32_Shdr)));
	int a=0;
	for (int i = 0; i < elfo.header.e_shnum; i++){
		char *name1=elfo.strtable+elfo.section[i].sh_name;
		ind=funForFindIndex(elfo,elft,i);
		if((elfo.section[i].sh_type==SHT_PROGBITS)||(elfo.section[i].sh_type==SHT_NULL)||(elfo.section[i].sh_type==SHT_REL)){
			if(ind!=-1){
				progbits[a]=malloc((elfo.section[i].sh_size+elft.section[ind].sh_size)* sizeof(unsigned char));
				for(int k=0;k<elfo.section[i].sh_size;k++){
					progbits[a][k]=elfo.progbits[i][k];
				}
				for(int k=0;k<elft.section[ind].sh_size;k++){
					progbits[a][k+elfo.section[i].sh_size]=elft.progbits[ind][k];
				}
					e_shoff=e_shoff+sizeof(progbits[a]);
						section[a].sh_name=elfo.section[i].sh_name;
						section[a].sh_type=elfo.section[i].sh_type;
						section[a].sh_flags=elfo.section[i].sh_flags;
						section[a].sh_addr=elfo.section[i].sh_addr;
						if(a==0)section[a].sh_offset=elfo.section[i].sh_offset;else section[a].sh_offset=section[a-1].sh_offset+section[a-1].sh_size;
						section[a].sh_size=elfo.section[i].sh_size+elft.section[ind].sh_size;
						section[a].sh_link=elfo.section[i].sh_link;
						section[a].sh_info=elfo.section[i].sh_info;
						section[a].sh_addralign=elfo.section[i].sh_addralign;
						section[a].sh_entsize=elfo.section[i].sh_entsize;
				a++;
			}else{
				progbits[a]=malloc((elfo.section[i].sh_size) * sizeof(unsigned char));
				for(int k=0;k<elfo.section[i].sh_size;k++){
					progbits[a][k]=elfo.progbits[i][k];
				}
					e_shoff=e_shoff+sizeof(progbits[a]);
						section[a].sh_name=elfo.section[i].sh_name;
						section[a].sh_type=elfo.section[i].sh_type;
						section[a].sh_flags=elfo.section[i].sh_flags;
						section[a].sh_addr=elfo.section[i].sh_addr;
						if(a==0)section[a].sh_offset=elfo.section[i].sh_offset;else section[a].sh_offset=section[a-1].sh_offset+section[a-1].sh_size;
						section[a].sh_size=elfo.section[i].sh_size;
						section[a].sh_link=elfo.section[i].sh_link;
						section[a].sh_info=elfo.section[i].sh_info;
						section[a].sh_addralign=elfo.section[i].sh_addralign;
						section[a].sh_entsize=elfo.section[i].sh_entsize;
				a++;
			}
		}
	}
	for (int i = 0; i < elft.header.e_shnum; i++){
		char *name1=elft.strtable+elft.section[i].sh_name;
		ind=funForFindIndex(elft,elfo,i);
		if((elft.section[i].sh_type==SHT_PROGBITS)||(elft.section[i].sh_type==SHT_NULL)||(elft.section[i].sh_type==SHT_REL)){
			if(ind==-1){
				progbits[a]=malloc((elft.section[i].sh_size) * sizeof(unsigned char));
				for(int k=0;k<elft.section[i].sh_size;k++){
					progbits[a][k]=elft.progbits[ind][k];
				}

					e_shoff=e_shoff+sizeof(progbits[a]);
							section[a].sh_name=elft.section[i].sh_name;
							section[a].sh_type=elft.section[i].sh_type;
							section[a].sh_flags=elft.section[i].sh_flags;
							section[a].sh_addr=elft.section[i].sh_addr;
							if(a==0)section[a].sh_offset=elfo.section[i].sh_offset;else section[a].sh_offset=section[a-1].sh_offset+section[a-1].sh_size;
							section[a].sh_size=elft.section[i].sh_size;
							section[a].sh_link=elft.section[i].sh_link;
							section[a].sh_info=elft.section[i].sh_info;
							section[a].sh_addralign=elft.section[i].sh_addralign;
							section[a].sh_entsize=elft.section[i].sh_entsize;
				a++;
			}
		} 
	}
	for (int i = 0; i < elfo.header.e_shnum; i++){
		char *name1=elfo.strtable+elfo.section[i].sh_name;
		ind=funForFindIndex(elfo,elft,i);
		if((elfo.section[i].sh_type==SHT_STRTAB)||(elfo.section[i].sh_type==SHT_ARM_ATTRIBUTES)){
			if(strcmp(name,name1)==0){e_shstrndx=a;}
			if(ind!=-1){
				progbits[a]=malloc((elfo.section[i].sh_size+elft.section[ind].sh_size)* sizeof(unsigned char));
				for(int k=0;k<elfo.section[i].sh_size;k++){
					progbits[a][k]=elfo.progbits[i][k];
				}
				int index=1,skip=0,b=0,z;
				while(index+skip<elft.section[ind].sh_size){
					do{
						for(int k=0;k<elfo.section[i].sh_size;k++){
							z=index+skip;
							while(elft.progbits[ind][z]!=0){
								if(elfo.progbits[i][k+z-index-skip]==elft.progbits[ind][z]){b=1;}else{b=0;}
								z++;	
							}
							if(b)break;
						}
						if(!b){
							progbits[a][index+elfo.section[i].sh_size]=elft.progbits[ind][index+skip];
						}
						index++;
					}while(elft.progbits[ind][index+skip]!=0);
				}
					e_shoff=e_shoff+sizeof(progbits[a]);
						section[a].sh_name=elfo.section[i].sh_name;
						section[a].sh_type=elfo.section[i].sh_type;
						section[a].sh_flags=elfo.section[i].sh_flags;
						section[a].sh_addr=elfo.section[i].sh_addr;
						if(a==0)section[a].sh_offset=elfo.section[i].sh_offset;else section[a].sh_offset=section[a-1].sh_offset+section[a-1].sh_size;
						section[a].sh_size=elfo.section[i].sh_size+elft.section[ind].sh_size;
						section[a].sh_link=elfo.section[i].sh_link;
						section[a].sh_info=elfo.section[i].sh_info;
						section[a].sh_addralign=elfo.section[i].sh_addralign;
						section[a].sh_entsize=elfo.section[i].sh_entsize;
				a++;
			}else{
				progbits[a]=malloc((elfo.section[i].sh_size) * sizeof(unsigned char));
				for(int k=0;k<elfo.section[i].sh_size;k++){
					progbits[a][k]=elfo.progbits[i][k];
				}
					e_shoff=e_shoff+sizeof(progbits[a]);
						section[a].sh_name=elfo.section[i].sh_name;
						section[a].sh_type=elfo.section[i].sh_type;
						section[a].sh_flags=elfo.section[i].sh_flags;
						section[a].sh_addr=elfo.section[i].sh_addr;
						if(a==0)section[a].sh_offset=elfo.section[i].sh_offset;else section[a].sh_offset=section[a-1].sh_offset+section[a-1].sh_size;
						section[a].sh_size=elfo.section[i].sh_size;
						section[a].sh_link=elfo.section[i].sh_link;
						section[a].sh_info=elfo.section[i].sh_info;
						section[a].sh_addralign=elfo.section[i].sh_addralign;
						section[a].sh_entsize=elfo.section[i].sh_entsize;
				a++;
			}
		}
	}
	for (int i = 0; i < elfo.header.e_shnum; i++){
		char *name1=elfo.strtable+elfo.section[i].sh_name;
		ind=funForFindIndex(elfo,elft,i);
		if((elft.section[i].sh_type==SHT_STRTAB)||(elft.section[i].sh_type==SHT_ARM_ATTRIBUTES)){
			if(ind==-1){
				progbits[a]=malloc((elft.section[i].sh_size) * sizeof(unsigned char));
				for(int k=0;k<elft.section[i].sh_size;k++){
					progbits[a][k]=elft.progbits[i][k];
				}
					e_shoff=e_shoff+sizeof(progbits[a]);
							section[a].sh_name=elft.section[i].sh_name;
							section[a].sh_type=elft.section[i].sh_type;
							section[a].sh_flags=elft.section[i].sh_flags;
							section[a].sh_addr=elft.section[i].sh_addr;
							if(a==0)section[a].sh_offset=elfo.section[i].sh_offset;else section[a].sh_offset=section[a-1].sh_offset+section[a-1].sh_size;
							section[a].sh_size=elft.section[i].sh_size;
							section[a].sh_link=elft.section[i].sh_link;
							section[a].sh_info=elft.section[i].sh_info;
							section[a].sh_addralign=elft.section[i].sh_addralign;
							section[a].sh_entsize=elft.section[i].sh_entsize;
				a++;
			}
		}
	}
	header.e_ident[EI_DATA]=1;
	header.e_shnum=a;
	header.e_shstrndx=e_shstrndx;
	header.e_shoff=e_shoff;
	FILE *f=fopen("res_elf","w");
	fseek(f,0,SEEK_SET);
	fwrite(&header,1,sizeof(Elf32_Ehdr),f);
	int fsv=header.e_ehsize;
	for(int i=0;i<a;i++){
		fseek(f,fsv,SEEK_SET);
		fwrite(&progbits[i],1,sizeof(progbits[i]),f);
		fsv=fsv+sizeof(progbits[i]);
	}
	for(int i=0;i<a;i++){
		fseek(f,fsv,SEEK_SET);
		fwrite(&section[i],1,sizeof(Elf32_Shdr),f);
		fsv=fsv+sizeof(Elf32_Shdr);
	}

	
}




//p2
void p2e6e8(Elf32_info elfo,Elf32_info elft){
	int ind;
	int nbsect=0;
	unsigned char **progbits;
	Elf32_Shdr *section;
	for (int i = 0; i < elfo.header.e_shnum; i++){
			char *name1=elfo.strtable+elfo.section[i].sh_name;
			if((elfo.section[i].sh_type==SHT_PROGBITS)||(elfo.section[i].sh_type==SHT_NULL)||(elfo.section[i].sh_type==SHT_REL)){
					ind=funForFindIndex(elfo,elft,i);
					if(ind!=-1){
						printf("[%s]",name1);nbsect++;
						for(int k=0;k<elfo.section[i].sh_size;k++){
							printf("%02x",elfo.progbits[i][k]);
						if((k+1)%4==0)printf(" ");
					}
						for(int k=0;k<elft.section[ind].sh_size;k++){
							printf("%02x",elft.progbits[ind][k]);
							if((k+1)%4==0)printf(" ");
						}
						printf("\n");
					}else{
						printf("[%s]",name1);nbsect++;
						for(int k=0;k<elfo.section[i].sh_size;k++){
							printf("%02x",elfo.progbits[i][k]);
							if((k+1)%4==0)printf(" ");
						}
						printf("\n");
					}
		}
	}
	for (int i = 0; i < elft.header.e_shnum; i++){
			char *name1=elft.strtable+elft.section[i].sh_name;
			if((elft.section[i].sh_type==SHT_PROGBITS)||(elft.section[i].sh_type==SHT_NULL)||(elft.section[i].sh_type==SHT_REL)){
					ind=funForFindIndex(elft,elfo,i);
					if(ind==-1){
							printf("[%s]",name1);nbsect++;
							for(int k=0;k<elft.section[i].sh_size;k++){
								printf("%02x",elft.progbits[i][k]);
								if((k+1)%4==0)printf(" ");
							}
							printf("\n");
					}
		}
	}
	progbits=malloc((nbsect) * sizeof (unsigned char *));
	section= malloc(nbsect*(sizeof(Elf32_Shdr)));
	int a=0;
	for (int i = 0; i < elfo.header.e_shnum; i++){
			char *name1=elfo.strtable+elfo.section[i].sh_name;
			if((elfo.section[i].sh_type==SHT_PROGBITS)||(elfo.section[i].sh_type==SHT_NULL)||(elfo.section[i].sh_type==SHT_REL)){
					ind=funForFindIndex(elfo,elft,i);
					if(ind!=-1){
						progbits[a]=malloc((elfo.section[i].sh_size+elft.section[ind].sh_size)* sizeof(unsigned char));
						for(int k=0;k<elfo.section[i].sh_size;k++){
							progbits[a][k]=elfo.progbits[i][k];
					}
						for(int k=0;k<elft.section[ind].sh_size;k++){
							progbits[a][k+elfo.section[i].sh_size]=elft.progbits[ind][k];
						}
						section[a].sh_name=elfo.section[i].sh_name;
						section[a].sh_type=elfo.section[i].sh_type;
						section[a].sh_flags=elfo.section[i].sh_flags;
						section[a].sh_addr=elfo.section[i].sh_addr;
						if(a==0)section[a].sh_offset=elfo.section[i].sh_offset;else section[a].sh_offset=section[a-1].sh_offset+section[a-1].sh_size;
						section[a].sh_size=elfo.section[i].sh_size+elft.section[ind].sh_size;
						section[a].sh_link=elfo.section[i].sh_link;
						section[a].sh_info=elfo.section[i].sh_info;
						section[a].sh_addralign=elfo.section[i].sh_addralign;
						section[a].sh_entsize=elfo.section[i].sh_entsize;

						a++;
					}else{
						progbits[a]=malloc((elfo.section[i].sh_size) * sizeof(unsigned char));
						for(int k=0;k<elfo.section[i].sh_size;k++){
							progbits[a][k]=elfo.progbits[i][k];
						}
						section[a].sh_name=elfo.section[i].sh_name;
						section[a].sh_type=elfo.section[i].sh_type;
						section[a].sh_flags=elfo.section[i].sh_flags;
						section[a].sh_addr=elfo.section[i].sh_addr;
						if(a==0)section[a].sh_offset=elfo.section[i].sh_offset;else section[a].sh_offset=section[a-1].sh_offset+section[a-1].sh_size;
						section[a].sh_size=elfo.section[i].sh_size+elft.section[ind].sh_size;
						section[a].sh_link=elfo.section[i].sh_link;
						section[a].sh_info=elfo.section[i].sh_info;
						section[a].sh_addralign=elfo.section[i].sh_addralign;
						section[a].sh_entsize=elfo.section[i].sh_entsize;
						a++;
					}
		}
	}
	for (int i = 0; i < elft.header.e_shnum; i++){
			char *name1=elft.strtable+elft.section[i].sh_name;
			if((elft.section[i].sh_type==SHT_PROGBITS)||(elft.section[i].sh_type==SHT_NULL)||(elft.section[i].sh_type==SHT_REL)){
					ind=funForFindIndex(elft,elfo,i);
					if(ind==-1){
							progbits[a]=malloc((elft.section[i].sh_size) * sizeof(unsigned char));
							for(int k=0;k<elft.section[i].sh_size;k++){
								progbits[a][k]=elft.progbits[ind][k];
							}
							section[a].sh_name=elft.section[i].sh_name;
							section[a].sh_type=elft.section[i].sh_type;
							section[a].sh_flags=elft.section[i].sh_flags;
							section[a].sh_addr=elft.section[i].sh_addr;
							if(a==0)section[a].sh_offset=elfo.section[i].sh_offset;else section[a].sh_offset=section[a-1].sh_offset+section[a-1].sh_size;
							section[a].sh_size=elft.section[i].sh_size+elft.section[ind].sh_size;
							section[a].sh_link=elft.section[i].sh_link;
							section[a].sh_info=elft.section[i].sh_info;
							section[a].sh_addralign=elft.section[i].sh_addralign;
							section[a].sh_entsize=elft.section[i].sh_entsize;
							a++;
					}
		}
	}
}



void p2e7(Elf32_info elfo,Elf32_info elft){
	int ind;
	int nbsect=0;
	unsigned char **progbits;
	Elf32_Shdr *section;
	for (int i = 0; i < elfo.header.e_shnum; i++)
		if((elfo.section[i].sh_type==SHT_STRTAB)||(elfo.section[i].sh_type==SHT_ARM_ATTRIBUTES))nbsect++;
	for (int i = 0; i < elft.header.e_shnum; i++){
		char *name1=elft.strtable+elft.section[i].sh_name;
		if((elft.section[i].sh_type==SHT_STRTAB)||(elft.section[i].sh_type==SHT_ARM_ATTRIBUTES)){
			ind=funForFindIndex(elft,elfo,i);
			if(ind==-1)printf("[%s]",name1);nbsect++;	
		}
	}
	progbits=malloc((nbsect) * sizeof (unsigned char *));
	section= malloc(nbsect*(sizeof(Elf32_Shdr)));
	int a=0;
	for (int i = 0; i < elfo.header.e_shnum; i++){
		char *name1=elfo.strtable+elfo.section[i].sh_name;
		if((elfo.section[i].sh_type==SHT_STRTAB)||(elfo.section[i].sh_type==SHT_ARM_ATTRIBUTES)){
			ind=funForFindIndex(elfo,elft,i);
			printf("[%s]",name1);
			if(ind!=-1){
				progbits[a]=malloc((elfo.section[i].sh_size+elft.section[ind].sh_size)* sizeof(unsigned char));
				for(int k=0;k<elfo.section[i].sh_size;k++){
					progbits[a][k]=elfo.progbits[i][k];
					printf("%c",isprint(elfo.progbits[i][k])?elfo.progbits[i][k]:'.');
				}
				int index=1,skip=0,b=0,z;
				while(index+skip<elft.section[ind].sh_size){
						do{
							for(int k=0;k<elfo.section[i].sh_size;k++){
								z=index+skip;
								while(elft.progbits[ind][z]!=0){
									if(elfo.progbits[i][k+z-index-skip]==elft.progbits[ind][z]){b=1;}else{b=0;}
									z++;	
								}
								if(b)break;
							}
							if(!b){
								progbits[a][index+elfo.section[i].sh_size]=elft.progbits[ind][index+skip];
								printf("%c",isprint(elft.progbits[ind][index+skip])?elft.progbits[ind][index+skip]:'.');
							}
							index++;
					}while(elft.progbits[ind][index+skip]!=0);

				}
				printf("\n");
				section[a].sh_name=elfo.section[i].sh_name;
				section[a].sh_type=elfo.section[i].sh_type;
				section[a].sh_flags=elfo.section[i].sh_flags;
				section[a].sh_addr=elfo.section[i].sh_addr;
				section[a].sh_size=elfo.section[i].sh_size+elft.section[ind].sh_size;
				section[a].sh_link=elfo.section[i].sh_link;
				section[a].sh_info=elfo.section[i].sh_info;
				section[a].sh_addralign=elfo.section[i].sh_addralign;
				section[a].sh_entsize=elfo.section[i].sh_entsize;
				a++;
			}else{
				progbits[a]=malloc((elfo.section[i].sh_size) * sizeof(unsigned char));
				for(int k=0;k<elfo.section[i].sh_size;k++){
					progbits[a][k]=elfo.progbits[i][k];
					printf("%c",isprint(elfo.progbits[i][k])?elfo.progbits[i][k]:'.');
				}
				printf("\n");
				section[a].sh_name=elfo.section[i].sh_name;
				section[a].sh_type=elfo.section[i].sh_type;
				section[a].sh_flags=elfo.section[i].sh_flags;
				section[a].sh_addr=elfo.section[i].sh_addr;
				section[a].sh_size=elfo.section[i].sh_size+elft.section[ind].sh_size;
				section[a].sh_link=elfo.section[i].sh_link;
				section[a].sh_info=elfo.section[i].sh_info;
				section[a].sh_addralign=elfo.section[i].sh_addralign;
				section[a].sh_entsize=elfo.section[i].sh_entsize;
				a++;
			}
		}
	}
	for (int i = 0; i < elft.header.e_shnum; i++){
		char *name1=elft.strtable+elft.section[i].sh_name;
		if((elft.section[i].sh_type==SHT_STRTAB)||(elft.section[i].sh_type==SHT_ARM_ATTRIBUTES)){
			ind=funForFindIndex(elft,elfo,i);
				if(ind==-1){
					printf("[%s]",name1);
					progbits[a]=malloc((elft.section[i].sh_size) * sizeof(unsigned char));
					for(int k=0;k<elft.section[i].sh_size;k++){
						progbits[a][k]=elft.progbits[i][k];
						printf("%c",isprint(elft.progbits[i][k])?elft.progbits[i][k]:'.');
					}
					section[a].sh_name=elft.section[i].sh_name;
					section[a].sh_type=elft.section[i].sh_type;
					section[a].sh_flags=elft.section[i].sh_flags;
					section[a].sh_addr=elft.section[i].sh_addr;
					section[a].sh_size=elft.section[i].sh_size+elft.section[ind].sh_size;
					section[a].sh_link=elft.section[i].sh_link;
					section[a].sh_info=elft.section[i].sh_info;
					section[a].sh_addralign=elft.section[i].sh_addralign;
					section[a].sh_entsize=elft.section[i].sh_entsize;
					a++;
			}
		}
	}
}





/*




	






	int ind;
	int symtabmalloc=0;
	Elf32_Sym  *symtab;
	unsigned char **symtable;
	for(int i=0 ; i<elfo.header.e_shnum;i++){
		if(elfo.section[i].sh_type==SHT_SYMTAB){
				ind=funForFindIndex(elfo,elft,i);
				if(ind!=-1){
					char *name1=elfo.strtable+elfo.section[i].sh_name;
					printf("[%s]\n",name1);printf("%s\n",(char*)elfo.symtable[13]+elfo.symtab[4].st_name);
					symtabmalloc++;
				}else {
					char *name1=elfo.strtable+elfo.section[i].sh_name;
					printf("[%s]\n",name1);
					symtabmalloc++;
				}
			}
	}
	for (int i = 0; i< elft.header.e_shnum;i++){
		if(elft.section[i].sh_type==SHT_STRTAB){
			ind=funForFindIndex(elft,elfo,i);
			if(ind==-1){
				char *name1=elft.strtable+elft.section[i].sh_name;
				printf("[%s]\n",name1);
				symtabmalloc++;
			}
		}
	}
	symtab=malloc(sizeof(Elf32_Sym)*symtabmalloc);
}*/


















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
