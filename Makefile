et6_demo: read_elf_fonctions.o et6.o
	gcc read_elf_fonctions.o et6.o -o et6_demo

fdemo: read_elf_fonctions.o f.o
	gcc read_elf_fonctions.o f.o -o fdemo

readelfdemo: read_elf_fonctions.o read_elf.o
	gcc read_elf_fonctions.o read_elf.o -o readelfdemo

et6.o: et6.c read_elf_fonctions.h myelf.h
	gcc -c et6.c

f.o: f.c read_elf_fonctions.h myelf.h
	gcc -c f.c

read_elf.o: read_elf.c read_elf_fonctions.h myelf.h
	gcc -c read_elf.c

read_elf_fonctions.o: read_elf_fonctions.c read_elf_fonctions.h  myelf.h
	gcc -c read_elf_fonctions.c

clean:
		rm read_elf.o read_elf_fonctions.o readelfdemo et6.o f.o fdemo et6_demo
