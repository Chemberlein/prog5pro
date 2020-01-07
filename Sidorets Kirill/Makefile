f: read_elf_fonctions.o f.o
	gcc read_elf_fonctions.o f.o -o fdemo

f.o: f.c read_elf_fonctions.h myelf.h
	gcc -c f.c

read_elf_fonctions.o: read_elf_fonctions.c read_elf_fonctions.h  myelf.h
	gcc -c read_elf_fonctions.c


clean:
		rm f.o read_elf_fonctions.o fdemo
