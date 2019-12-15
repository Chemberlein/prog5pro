#include <elf.h>
#include <stdio.h>
#include <string.h>
uint16_t swap_uint16(uint16_t x) {
    return (x << 8) | (x >> 8 );
}
uint32_t swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
}
uint64_t swap_uint64( uint64_t val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}
int main(int argc, char const *argv[]){
  	FILE* file = fopen(argv[1], "rb");
  	unsigned char e_ident[EI_NIDENT];
  	fread(&e_ident, 1, sizeof(e_ident), file);
  	int i=0;
  	printf("En-tête ELF:\n");
  	printf("            Magique:                                         ");
  	while(i<EI_NIDENT){
  		printf("%02x ",e_ident[i]);
  		i++;
  	}
  	printf("\n");
  	printf("            Classe:                                          ");
  	switch(e_ident[EI_CLASS]){
  		case ELFCLASSNONE:
  			printf("This class is invalid\n");
  		break;
  		case ELFCLASS32:
  			printf("ELF32\n");
  		break;
  		case ELFCLASS64:
  			printf("ELF64\n");
  		break;
  	}
  	printf("            Données:                                         ");
  	switch(e_ident[EI_DATA]){
  		case ELFDATANONE:
  			printf("Unknown data format\n");
  		break;
  		case ELFDATA2LSB:
  			printf("complément à 2, ????????????????????????????????????? (little endian)\n");
  		break;
  		case ELFDATA2MSB:
  			printf("complément à 2, système à octets de poids fort d'abord (big endian)\n");
  		break;
  	}
  	printf("            Version:                                         ");
  	switch(e_ident[EI_VERSION]){
  		case ELFDATANONE:
  			printf("0 (Version invalide) \n");
  		break;
  		case ELFDATA2LSB:
  			printf("1 (current)\n");
  		break;
  	}
  	printf("            OS/ABI:                                          ");
  	switch(e_ident[EI_OSABI]){ 		
        case ELFOSABI_SYSV:
            printf("ABI d'UNIX System V\n");
        break;
        case ELFOSABI_HPUX:
            printf("ABI de HP-UX\n");
        break;
        case ELFOSABI_NETBSD:
            printf("ABI de NetBSD\n");
        break;
        case ELFOSABI_LINUX:
            printf("ABI de Linux\n");
        break;
        case ELFOSABI_SOLARIS:
            printf("ABI de Solaris\n");
        break;
        case ELFOSABI_IRIX:
            printf("ABI d'IRIX\n");
        break;
        case ELFOSABI_FREEBSD:
            printf("ABI de FreeBSD\n");
        break;
        case ELFOSABI_ARM:
            printf("ABI de l'architecture ARM\n");
        break;
        case ELFOSABI_TRU64:
            printf("ABI de l'UNIX TRU64\n");
        break;
        case ELFOSABI_STANDALONE:
            printf("ABI autonome (intégrée)\n");
        break;
  	}
  	printf("            Version ABI:                                     ");
	printf("%d\n",e_ident[EI_ABIVERSION]);
	printf("            Type:                                            ");
	uint16_t e_type;
  	fread(&e_type, 1, sizeof(e_type), file);
  	if (e_ident[EI_DATA]!=ELFDATA2LSB){
      	e_type =swap_uint16(e_type);
  	}
    switch (e_type){
        case ET_NONE:
            printf("An unknown type\n");
            break;
        case ET_REL:
            printf("REL A relocatable file\n");
            break;
        case ET_EXEC:
            printf("EXEC An executable file\n");
            break;
        case ET_DYN:
            printf("DYN A shared object\n");
            break;
        case ET_CORE:
            printf("CORE A core file\n");
            break;
        case ET_LOPROC:
            printf("LOPROC Processor-specific\n");
            break;
        case ET_HIPROC:
            printf("HIPROC Processor-specific\n");
           	break;
          }
  	printf("            Machine:                                         ");
  	uint16_t e_machine;
  	fread(&e_machine, 1, sizeof(e_machine), file);
  	if (e_ident[EI_DATA]!=ELFDATA2LSB){
      	e_machine=swap_uint16(e_machine);
  	}
    switch (e_machine){
            case EM_NONE:
                printf("NONE  An unknown machine\n");
                break;
            case EM_M32:
                printf("M32 AT&T WE 32100\n");
                break;
            case EM_SPARC:
                printf("SPARC Sun Microsystems SPARC\n");
                break;
            case EM_386:
                printf("386 Intel 80386\n");
                break;
            case EM_68K:
                printf("68K Motorola 68000\n");
                break;
            case EM_88K:
                printf("88K Motorola 88000 \n");
                break;
            case EM_860:
                printf("860 Intel 80860 \n");
                break;
            case EM_MIPS:
                printf("MIPS MIPS RS3000 (big-endian only) \n");
                break;
            case EM_PARISC:
                printf("PARISC HP/PA \n");
                break;
            case EM_SPARC32PLUS:
                printf("SPARC32PLUS SPARC with enhanced instruction set \n");
                break;
            case EM_PPC:
                printf("PPC PowerPC \n");
                break;
            case EM_PPC64:
                printf("PPC64 PowerPC 64-bit \n");
                break;
            case EM_S390:
                printf("S390 IBM S/390 \n");
                break;
            case EM_ARM:
                printf("ARM Advanced RISC Machines \n");
                break;
            case EM_SH:
                printf("SH Renesas SuperH \n");
                break;
            case EM_SPARCV9:
                printf("SPARCV9 SPARC v9 64-bit \n");
                break;
            case EM_IA_64:
                printf("IA_64 Intel Itanium \n");
                break;
            case EM_X86_64:
                printf("IA_64 AMD x86-64 \n");
                break;
            case EM_VAX:
                printf("VAX DEC Vax  \n");
                break;
          } 
  	printf("            Version:                                         ");
  	uint32_t e_version;
  	fread(&e_version, 1, sizeof(e_version), file);
	if (e_ident[EI_DATA]!=ELFDATA2LSB){
    	e_version =swap_uint32(e_version);
  	}
    printf("0x%lx\n",e_type);
    printf("            Adresse du point d'entrée:                       ");
  	Elf32_Addr e_entry32;
  	Elf64_Addr e_entry64;
  	switch(e_ident[EI_CLASS]){
  		case ELFCLASSNONE:
  			printf("This class is invalid\n");
  		break;
  		case ELFCLASS32:
  			fread(&e_entry32,1,sizeof(e_entry32),file);
  			if (e_ident[EI_DATA]!=ELFDATA2LSB){
      			e_entry32 =swap_uint32(e_entry32);
  				}
  			printf("0x%hx\n",e_entry32);
  		break;
  		case ELFCLASS64:
  			fread(&e_entry64,1,sizeof(e_entry64),file);
  			if (e_ident[EI_DATA]!=ELFDATA2LSB){
      			e_entry64 =swap_uint64(e_entry64);
  				}
  			printf("0x%hx\n",e_entry64);
  		break;
  	}
    printf("            Début des en-têtes de programme:                 ");
  	Elf32_Off e_phoff32;
  	Elf64_Off e_phoff64;
  	switch(e_ident[EI_CLASS]){
  		case ELFCLASSNONE:
  			printf("This class is invalid\n");
  		break;
  		case ELFCLASS32:
  			fread(&e_phoff32,1,sizeof(e_phoff32),file);
  			if (e_ident[EI_DATA]!=ELFDATA2LSB){
      			e_phoff32 =swap_uint32(e_phoff32);
  				}
  			printf("%d",e_phoff32);
  		break;
  		case ELFCLASS64:
  			fread(&e_phoff64,1,sizeof(e_phoff64),file);
  			if (e_ident[EI_DATA]!=ELFDATA2LSB){
      			e_phoff64 =swap_uint64(e_phoff64);
  				}
  			printf("%d",e_phoff64);
  		break;
  	}
  	printf (" (bytes into file)\n");
  	printf("            Début des en-têtes de section:                   ");
    Elf32_Off e_shoff32;
    Elf64_Off e_shoff64;
    switch(e_ident[EI_CLASS]){
        case ELFCLASSNONE:
            printf("This class is invalid\n");
        break;
        case ELFCLASS32:
            fread(&e_shoff32,1,sizeof(e_shoff32),file);
            if (e_ident[EI_DATA]!=ELFDATA2LSB){
                e_shoff32 =swap_uint32(e_shoff32);
                }
            printf("%d",e_shoff32);
        break;
        case ELFCLASS64:
            fread(&e_shoff64,1,sizeof(e_shoff64),file);
            if (e_ident[EI_DATA]!=ELFDATA2LSB){
                e_shoff64 =swap_uint64(e_shoff64);
                }
            printf("%d",e_shoff64);
        break;
    }
  	printf (" (bytes into file)\n");
  	printf("            Fanions:                                         ");
  	uint32_t e_flags;
  	fread(&e_flags, 1, sizeof(e_flags), file);
	if (e_ident[EI_DATA]!=ELFDATA2LSB){
    	e_flags =swap_uint32(e_flags);
  	}
    printf("0x%lx\n",e_flags);
  	printf("            Taille de cet en-tête:                           ");
  	uint16_t e_ehsize;
  	fread(&e_ehsize, 1, sizeof(e_ehsize), file);
  	if (e_ident[EI_DATA]!=ELFDATA2LSB){
      	e_ehsize=swap_uint16(e_ehsize);
  	}
  	printf("%u (bytes)\n",e_ehsize);
  	printf("            Taille de l'en-tête du programme:                ");
  	uint16_t e_phentsize;
  	fread(&e_phentsize, 1, sizeof(e_phentsize), file);
  	if (e_ident[EI_DATA]!=ELFDATA2LSB){
      	e_phentsize=swap_uint16(e_phentsize);
  	}
  	printf("%u (bytes)\n",e_phentsize);
  	printf("            Nombre d'en-tête du programme:                   ");
  	uint16_t e_phnum;
  	fread(&e_phnum, 1, sizeof(e_phnum), file);
  	if (e_ident[EI_DATA]!=ELFDATA2LSB){
      	e_phnum=swap_uint16(e_phnum);
  	}
  	printf("%u \n",e_phnum);
  	printf("            Taille des en-têtes de section:                  ");
  	uint16_t e_shentsize;
  	fread(&e_shentsize, 1, sizeof(e_shentsize), file);
  	if (e_ident[EI_DATA]!=ELFDATA2LSB){
      	e_shentsize=swap_uint16(e_shentsize);
  	}
  	printf("%u (bytes)\n",e_shentsize);
  	printf("            Nombre d'en-têtes de section:                    ");
  	uint16_t e_shnum;
  	fread(&e_shnum, 1, sizeof(e_shnum), file);
  	if (e_ident[EI_DATA]!=ELFDATA2LSB){
      	e_shnum=swap_uint16(e_shnum);
  	}
  	printf("%u \n",e_shnum);
  	printf("            Table d'indexes des chaînes d'en-tête de section:");
  	uint16_t e_shstrndx;
  	fread(&e_shstrndx, 1, sizeof(e_shstrndx), file);
  	if (e_ident[EI_DATA]!=ELFDATA2LSB){
      	e_shstrndx=swap_uint16(e_shstrndx);
  	}
  	printf("%u \n",e_shstrndx);
  	fclose(file);
}

