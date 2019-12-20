#include <stdio.h>
#include <elf.h>
#include <string.h>
#include <stdlib.h>
#include <byteswap.h>



uint16_t print_val_sym(FILE* file1,  Elf32_Ehdr header1 ,int k) {
  Elf32_Sym Sym1;
  Elf32_Shdr Shdr1,SymShdr;
  int res=0;
  for (int i = 0; i < __bswap_16(header1.e_shnum); i++){
    fseek(file1, __bswap_32( header1.e_shoff) + i * sizeof(Elf32_Shdr), SEEK_SET);
    fread(&Shdr1, 1, sizeof(Shdr1), file1);

    if (__bswap_32(Shdr1.sh_type)==SHT_SYMTAB){
        fseek(file1, __bswap_32( Shdr1.sh_offset) + k * sizeof(Elf32_Sym), SEEK_SET);
        fread(&Sym1, 1, sizeof(Elf32_Sym), file1);

              char* SymNames = NULL;
              const char* nameSym = "";
              fseek(file1,__bswap_32( header1.e_shoff)+__bswap_32(Shdr1.sh_link)*__bswap_16(header1.e_shentsize), SEEK_SET);
              fread(&SymShdr,1,__bswap_16(header1.e_shentsize),file1);
              SymNames = malloc(__bswap_32(SymShdr.sh_size));
              fseek(file1,__bswap_32(SymShdr.sh_offset), SEEK_SET);
              fread(SymNames, 1, __bswap_32(SymShdr.sh_size), file1);


              printf("%d-0x%08x  ", k,__bswap_32(Sym1.st_value) );
                if (Sym1.st_name){
                  nameSym =SymNames+__bswap_32(Sym1.st_name);
                  printf("%s",nameSym); 
                  return 0;
                }
                 return __bswap_16(Sym1.st_shndx);
                  free(SymNames);
                   }
}
return 0;}


void read_elf_header(const char* elfFile) {
  // switch to Elf32_Ehdr for x86 architecture.
  Elf32_Ehdr header;
  Elf32_Shdr Shdr;
  Elf32_Sym Sym;
  Elf32_Rel Rel;
  Elf32_Shdr ShdrForRead;
  int m;uint16_t boo=0;
  int nr_valeurs;
  const char* name = "";
  char* SectNames = NULL;

  FILE* file = fopen(elfFile, "rb");
  if(file) {

    fread(&header, 1, sizeof(header), file);
    fseek(file,__bswap_32(header.e_shoff)+__bswap_16(header.e_shstrndx)*__bswap_16(header.e_shentsize), SEEK_SET);
    fread(&Shdr,1,__bswap_16(header.e_shentsize),file);
    SectNames = malloc(__bswap_32(Shdr.sh_size));

    fseek(file,__bswap_32(Shdr.sh_offset), SEEK_SET);
    fread(SectNames, 1, __bswap_32(Shdr.sh_size), file);


    for (int i = 0; i < __bswap_16(header.e_shnum); i++){
      fseek(file, __bswap_32( header.e_shoff) + i * sizeof(Elf32_Shdr), SEEK_SET);
      fread(&Shdr, 1, sizeof(Shdr), file);

      if (__bswap_32(Shdr.sh_type)==SHT_REL){
        name = SectNames + __bswap_32(Shdr.sh_name);
        nr_valeurs = __bswap_32(Shdr.sh_size) / __bswap_32(Shdr.sh_entsize);

        printf("Section de readressage ' %s ' a l'adresse de decalage: 0x%x contient %d entree:\n\n",name, __bswap_32(Shdr.sh_offset ),  nr_valeurs);
        printf("Decalage     Info         Type         Val.-sym       Noms-symboles\n");
        for (int j=0 ; j<nr_valeurs; j++){

          fseek(file, __bswap_32( Shdr.sh_offset) + j * sizeof(Elf32_Rel), SEEK_SET);
          fread(&Rel, 1, sizeof(Elf32_Rel), file);

          printf("0x%08x   ",__bswap_32(Rel.r_offset) );
          printf("0x%08x   ",__bswap_32(Rel.r_info));
          switch (ELF32_R_TYPE(__bswap_32(Rel.r_info))) {
          case R_ARM_NONE: printf("%15.15s \t","R_ARM_NONE"); break;
          case R_ARM_PC24: printf("%15.15s \t","R_ARM_PC24"); break;
          case R_ARM_ABS32: printf("%15.15s \t","R_ARM_ABS32"); break;
          case R_ARM_REL32: printf("%15.15s \t","R_ARM_REL32"); break;
          case R_ARM_PLT32: printf("%15.15s \t","R_ARM_PLT32");break;
          case R_ARM_CALL: printf("%15.15s \t","R_ARM_CALL");break;
          case R_ARM_JUMP24: printf("%-15s \t","R_ARM_JUMP24");break;
          case R_ARM_THM_JUMP24: printf("%15.15s \t","R_ARM_THM_JUMP24");break;
          case R_ARM_BASE_ABS: printf("%15.15s \t","R_ARM_BASE_ABS");break;
          default: printf("%15.15s \t", "type non connu!"); break;
          }



          boo=print_val_sym(file, header, ELF32_R_SYM(__bswap_32(Rel.r_info)));
          if(boo!=0){
          const char* nameSym = "";
          fseek(file,__bswap_32(header.e_shoff)+boo*sizeof(ShdrForRead),SEEK_SET);
          fread(&ShdrForRead,1,sizeof(ShdrForRead),file);
              
          if (ShdrForRead.sh_name){nameSym=SectNames+__bswap_32(ShdrForRead.sh_name);}
            printf("%s",nameSym);
          }
printf("\n");

         
        }


          }
      }
    }



    fclose(file);
  }




int main(int argc, char *argv[]){

  read_elf_header(argv[1]);


}