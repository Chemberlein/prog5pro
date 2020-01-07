#bin/bash
gcc -c read_elf_fonctions.c read_elf_fonctions.h myelf.h
gcc -c read_elf.c myelf.h read_elf_fonctions.h
gcc read_elf.o read_elf_fonctions.o -o  readelfdemo
