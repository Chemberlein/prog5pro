#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <byteswap.h>

int main(int argc, char const *argv[]){
    FILE* file = fopen(argv[1], "rb");
    Elf32_Ehdr heder;
    char* SectNames = NULL;
    fread(&heder,1,sizeof(heder),file);
    Elf32_Shdr Shdr;
    printf("%u\n",__bswap_32(heder.e_shoff));
    fseek(file,__bswap_32(heder.e_shoff)+__bswap_16(heder.e_shstrndx)*sizeof(Shdr), SEEK_SET);
    fread(&Shdr,1,sizeof(Shdr),file);
    SectNames = malloc(__bswap_32(Shdr.sh_size));
    fseek(file,__bswap_32(Shdr.sh_offset), SEEK_SET);
    fread(SectNames, 1, __bswap_32(Shdr.sh_size), file);
    for (int idx = 0; idx < __bswap_16(heder.e_shnum); ++idx){
      const char* name = "";
      fseek(file, __bswap_32(heder.e_shoff) + idx * sizeof Shdr, SEEK_SET);
      fread(&Shdr, 1, sizeof Shdr, file);

      if (Shdr.sh_name);
      name = SectNames + __bswap_32(Shdr.sh_name);
    printf("%2u %s\n", idx, name);
  }
  free(SectNames);
    fclose(file);
}

