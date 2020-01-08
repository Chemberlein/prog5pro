#include <stdio.h>
#include <elf.h>
#include <string.h>
#include <stdlib.h>
#include <byteswap.h>


void read_elf_header(const char* elfFile) {
  // switch to Elf32_Ehdr for x86 architecture.
  Elf32_Ehdr header;
  Elf32_Shdr Shdr;
  Elf32_Shdr SymShdr;
  Elf32_Sym Sym;
  int nr_valeurs;
  FILE* file = fopen(elfFile, "rb");
  char* SectNames = NULL;
  if(file) {
    // read the header
    fread(&header, 1, sizeof(header), file);
    fseek(file,__bswap_32(header.e_shoff)+__bswap_16(header.e_shstrndx)*__bswap_16(header.e_shentsize), SEEK_SET);
    fread(&Shdr,1,__bswap_16(header.e_shentsize),file);
    SectNames = malloc(__bswap_32(Shdr.sh_size));
    fseek(file,__bswap_32(Shdr.sh_offset), SEEK_SET);
    fread(SectNames, 1, __bswap_32(Shdr.sh_size), file);
    
    for (int i = 0; i < __bswap_16(header.e_shnum); i++){
      const char* name = "";  
      fseek(file, __bswap_32( header.e_shoff) + i * sizeof(Elf32_Shdr), SEEK_SET);
      fread(&Shdr, 1, sizeof(Shdr), file);
      if (Shdr.sh_name)name = SectNames + __bswap_32(Shdr.sh_name);
          if (__bswap_32(Shdr.sh_type)==SHT_SYMTAB){
            nr_valeurs = __bswap_32(Shdr.sh_size) / __bswap_32(Shdr.sh_entsize);

            printf("La table de symboles < %s > contient %d entrées: \n",name,nr_valeurs);
            printf("Num: Valeur      Tail  Type     Lien   Vis         Ndx  Nom   \n");
            for (int j=0; j < nr_valeurs; j++){
              
              fseek(file, __bswap_32( Shdr.sh_offset) + j *sizeof(Sym), SEEK_SET);
              fread(&Sym, 1, sizeof(Sym), file);
              

              char* SymNames = NULL;
              const char* nameSym = "";
              fseek(file,__bswap_32( header.e_shoff)+__bswap_32(Shdr.sh_link)*__bswap_16(header.e_shentsize), SEEK_SET);
              fread(&SymShdr,1,__bswap_16(header.e_shentsize),file);
              SymNames = malloc(__bswap_32(SymShdr.sh_size));
              fseek(file,__bswap_32(SymShdr.sh_offset), SEEK_SET);
              fread(SymNames, 1, __bswap_32(SymShdr.sh_size), file);
              





              printf("%-5d",j  );
              printf("0x%08x  ", __bswap_32(Sym.st_value) );
              printf("%-5d ", __bswap_32(Sym.st_size) );
              switch(ELF32_ST_TYPE(Sym.st_info)){
                case STT_SECTION:
                  printf("SECTION   ");
                  break;

                case STT_NOTYPE:
                  printf("NOTYPE    ");
                  break;

                default:

                  printf("ERROR   ");
                  break;}

              switch(ELF32_ST_TYPE(ELF32_ST_BIND(Sym.st_info))){
                case STB_LOCAL:
                  printf("LOCAL  ");
                  break;

                case STB_GLOBAL:
                  printf("GLOBAL ");
                  break;

                default:
                  printf("ERROR   ");
                  break;}

              if (Sym.st_other==0)
                printf("DEFAULT     ");
              else
                printf("NOT DEFAULT ");

              printf("%d ", __bswap_16(Sym.st_shndx) );
     

              
     
              if (Sym.st_name){
                  nameSym =SymNames+__bswap_32(Sym.st_name);}
                  printf("%s\n",nameSym);
                  free(SymNames);
                  }

          }
      }
      free(SectNames);
    }



    fclose(file);
  }




int main(int argc, char *argv[]){

  read_elf_header(argv[1]);


}