#include <stdio.h>
#include "elf.h"
#include "header.h"
#include "section.h"
#include "swap.h"
#include "baseElf32.h"

void initElf(Elf32_info *elf,FILE *file);

//etape1
void afficheHeader(Elf32_info elf);
//etape2
void afficheTableSection(Elf32_info elf,FILE *file);
//etape3
void afficher_contenu_section(Elf32_info elf,FILE* fsource);


