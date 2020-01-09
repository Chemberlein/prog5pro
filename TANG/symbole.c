#include <stdlib.h>
#include "symbole.h"

void read_Sym(Elf32_info *elf,FILE *file){
	int nbSym=0;
	int tableSize=0;
	uint32_t  max =0;
	for(int i=0;i<elf->header.e_shnum;i++){  //calculer le nombre de symbole
		if(elf->section[i].sh_type==SHT_SYMTAB){
			nbSym=(elf->section[i].sh_size/elf->section[i].sh_entsize)+nbSym;
		}
	}	
	elf->symtab=malloc(sizeof(Elf32_Sym)*nbSym); //allouer la mémoire pour les infos issus de symboles
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
	elf-> symtable =malloc (tableSize * sizeof (unsigned char *));  //allouer une table  de chaîne de caractères pour afficher le nom de la table des symboles
}



void read_SymTable(Elf32_info *elf,FILE *file){
  Elf32_Shdr SymShdr;
  int indice=0;
  for(int i=0;i<elf->header.e_shnum;i++){ //obtenir la valeur de section de type STRTAB
    if(elf->section[i].sh_type == SHT_STRTAB && i!=elf->header.e_shstrndx)
      indice=i;
  }
  SymShdr = elf->section[indice];
  for(int i = 0;i<elf->header.e_shnum;i++){  //récupérer le nom de symboles
    if (elf->section[i].sh_type==SHT_SYMTAB){
              char* SymNames = NULL;
              SymNames = malloc(SymShdr.sh_size);
              if(SymNames == NULL)
                printf("impossible à lire la table de symboles\n");
              fseek(file,SymShdr.sh_offset, SEEK_SET);
              elf->symtable[i] = (unsigned char *) malloc (SymShdr.sh_size * sizeof(unsigned char)); //allouer la mémoire puis stocker le nom dans symtable
              if(fread(elf->symtable[i], 1, SymShdr.sh_size, file)){}        
    }
  }
}



uint16_t print_val_sym(Elf32_info elf,FILE *file,int k) { 
  for (int i = 0; i <elf.header.e_shnum; i++){
    if (elf.section[i].sh_type==SHT_SYMTAB){  //si le type est SYMTAB ,afficher l'information qui a été enregistré dans elf.symtab
      printf("%08x  ", elf.symtab[k].st_value ); 
      if (elf.symtab[k].st_name){   //si il y a un nom
        printf("%s",(char*)elf.symtable[i]+elf.symtab[k].st_name); 
        return 0;
      }
    return elf.symtab[k].st_shndx;
    }
  }
  return 0;
}




void aff_s(Elf32_info elf,FILE *file) {  // Affichage de tables de symbole
  int nr_valeurs;
  for (int i = 0; i < elf.header.e_shnum; i++){
    if (elf.section[i].sh_type==SHT_SYMTAB){
        nr_valeurs = elf.section[i].sh_size / elf.section[i].sh_entsize;
        printf("La table de symboles < %s > contient %d entrées: \n",(char*)elf.strtable+elf.section[i].sh_name,nr_valeurs);
        printf("Num: Valeur      Tail  Type     Lien   Vis         Ndx  Nom   \n");        
            for (int j=0; j < nr_valeurs; j++){             
              printf("%-5d:",j);
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
