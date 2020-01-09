#include "myelf.h"
#include <stdio.h>

typedef struct{
	Elf32_Ehdr header;
	Elf32_Shdr *section;
	unsigned char *strtable;
	unsigned char **progbits;
	Elf32_Sym  *symtab;
	unsigned char **symtable;
	Elf32_Rel  **reltab;
}Elf32_info;

//lecture
void initialiser_elf(Elf32_info *elf,FILE *fsource);

void lire_table_section(Elf32_info *elf,FILE *fsource);

void lire_strtab(Elf32_info *elf,FILE *file);
void read_progbits(Elf32_info *elf,FILE *file);

void read_Sym(Elf32_info *elf,FILE *file);

void aff_s(Elf32_info elf,FILE *file);
void read_SymTable(Elf32_info *elf,FILE *file);
void read_Rel(Elf32_info *elf,FILE *file);

//affichage
void afficher_header(Elf32_info elf);

void afficher_table_section(Elf32_info elf);

void afficher_contenu_section(Elf32_info elf,FILE* fsource);
void aff_r(Elf32_info elf,FILE *file) ;

void aff_s(Elf32_info elf,FILE *file);
uint16_t print_val_sym(Elf32_info elf,FILE *file,int k);
//structure
void setup_little_endian(Elf32_info *elf);

uint16_t swap_uint16( uint16_t val );

int16_t swap_int16( int16_t val );

uint32_t swap_uint32( uint32_t val );

int32_t swap_int32( int32_t val );


void p2e6e8(Elf32_info elfo,Elf32_info elft);
void p2e7(Elf32_info elfo,Elf32_info elft);
void p2(Elf32_info elfo,Elf32_info elft);