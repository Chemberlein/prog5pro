#include <stdlib.h>
#include "relocation.h"
#include "symbole.h"

void read_Rel(Elf32_info *elf,FILE *file){   //Stocker les infos de tables de réimplantation dans elf->reltab
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

void aff_r(Elf32_info elf,FILE *file) { //Affichage de tables de réimplantation
  int nr_valeurs;
  uint16_t boo = 0;
  for (int i = 0; i < elf.header.e_shnum; i++){
      if (elf.section[i].sh_type==SHT_REL){
        nr_valeurs = elf.section[i].sh_size / elf.section[i].sh_entsize; //nombre de tables

        printf("Section de réadressage ' %s ' à l'adresse de décalage 0x%x contient %d entrées:\n\n",(char*)elf.strtable+elf.section[i].sh_name, elf.section[i].sh_offset,  nr_valeurs);
        printf("Décalage     Info         Type         Val.-sym       Noms-symboles\n");
        for (int j=0 ; j<nr_valeurs; j++){
          printf("0x%08x   ",elf.reltab[i][j].r_offset); //offset
          printf("0x%08x   ",elf.reltab[i][j].r_info);  //info
          switch (ELF32_R_TYPE(elf.reltab[i][j].r_info)) {  //type
            case R_ARM_NONE: printf("%15.15s  ","R_ARM_NONE"); break;
            case R_ARM_PC24: printf("%15.15s  ","R_ARM_PC24"); break;
            case R_ARM_ABS32: printf("%15.15s  ","R_ARM_ABS32"); break;
            case R_ARM_REL32: printf("%15.15s  ","R_ARM_REL32"); break;
            case R_ARM_PLT32: printf("%15.15s  ","R_ARM_PLT32");break;
            case R_ARM_CALL: printf("%15.15s  ","R_ARM_CALL");break;
            case R_ARM_JUMP24: printf("%-15s  ","R_ARM_JUMP24");break;
            case R_ARM_THM_JUMP24: printf("%15.15s  ","R_ARM_THM_JUMP24");break;
            default: printf("%15.15s  ", "type non connu!"); break;
          }

          boo=print_val_sym(elf,file, ELF32_R_SYM(elf.reltab[i][j].r_info));//Val.-sym
          if(boo!=0){
            if (elf.section[boo].sh_name){printf("%s",(char*)elf.strtable+elf.section[boo].sh_name);} //nom de section de symbole
            
          }
          printf("\n");         
        }
    }
  }
}
