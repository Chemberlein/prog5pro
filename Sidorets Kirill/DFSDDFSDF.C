#include <stdio.h>
#include <elf.h>
#include <string.h>
#include <stdlib.h>
#include <byteswap.h>






void read_elf_header(const char* elfFile) {
  // switch to Elf32_Ehdr for x86 architecture.
  Elf32_Ehdr header;
  Elf32_Shdr Shdr;
  Elf32_Sym Sym;
  int nr_valeurs;


  FILE* file = fopen(elfFile, "rb");
  if(file) {
    // read the header
    fread(&header, 1, sizeof(header), file);


    fseek(file,__bswap_32(header.e_shoff), SEEK_SET);


    fread(&Shdr,1,sizeof(Shdr),file);
    for (int i = 0; i < __bswap_16(header.e_shnum); i++){
      fseek(file, __bswap_32( header.e_shoff) + i * sizeof(Elf32_Shdr), SEEK_SET);
      fread(&Shdr, 1, sizeof(Shdr), file);
          if (__bswap_32(Shdr.sh_type)==SHT_SYMTAB){
            nr_valeurs = __bswap_32(Shdr.sh_size) / __bswap_32(Shdr.sh_entsize);

            printf("La table de symboles < .symtab > contient %d entrées: \n",nr_valeurs);
            printf("Num: Valeur      Tail  Type     Lien   Vis         Ndx  Nom   \n");

            for (int j=0; j < nr_valeurs; j++){
              fseek(file, __bswap_32( Shdr.sh_offset) + j * sizeof(Elf32_Sym), SEEK_SET);
              fread(&Sym, 1, sizeof(Elf32_Sym), file);
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

              printf("\n" );

            }
            i = __bswap_16(header.e_shnum);

          }
      }
    }



    fclose(file);
  }




int main(int argc, char *argv[]){

  read_elf_header(argv[1]);


}
