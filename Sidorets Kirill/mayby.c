fseek(file, __bswap_32( Shdr.sh_offset), SEEK_SET)
char *s = malloc(__bswap_32(Shdr.sh_size));
fread(s, __bswap_32(Shdr.sh_size) , 1, file)
char *name = strtab + Shdr.sh_name;
printf("section %d: %s\n", i, name);
free(strtab);
free(s);


fseek(file, __bswap_32( Shdr.sh_offset), SEEK_SET);
      char *s = malloc(__bswap_32(Shdr.sh_size));
      fread(s, 1 , __bswap_32(Shdr.sh_size), file);
      char *name = s + __bswap_32(Shdr.sh_name);
      printf("%d %s\n",i, name);
      free(s);



fseek(file,symtab.sh_offset+i*symtab.sh_entsize,SEEK_SET);
    fread(&elfSym,1,sizeof elfSym,file);
