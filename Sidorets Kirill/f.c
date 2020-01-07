#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "myelf.h"
#include "read_elf_fonctions.h"

int main(int argc, char** argv){
	FILE *f1;
	FILE *f2;
	f1=fopen(argv[1],"r");
	f2=fopen(argv[2],"r");
	Elf32_info elf1;
	Elf32_info elf2;
	initialiser_elf(&elf1,f1);
	initialiser_elf(&elf2,f2);
	p2e6(elf1,elf2);
	fclose(f1);
	fclose(f2);
	return 0;
}

