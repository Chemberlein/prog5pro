#include <elf.h>
#include <stdio.h>
#include <string.h>
uint16_t swap_uint16(uint16_t x) {
    return (x << 8) | (x >> 8 );
}

int main(int argc, char const *argv[]){
  FILE* file = fopen(argv[1], "rb");
//#########################################################################################################################################
  // e_ident mem pour 32 bit et 64 bit elf file
//#########################################################################################################################################
  unsigned char e_ident[EI_NIDENT];
  fread(&e_ident, 1, sizeof(e_ident), file);
          printf("EI_MAG0  The first byte of the magic number.  It must be filled with ELFMAG0.  (0: 0x7f)  and in %s file EI_MAG0 == %x\n",argv[1],e_ident[EI_MAG0] );
          printf("EI_MAG1  The second byte of the magic number.  It must be filled with ELFMAG1.  (1: E)  and in %s file EI_MAG1 == %c\n",argv[1],e_ident[EI_MAG1]);
          printf("EI_MAG2  The third byte of the magic number.  It must be filled with ELFMAG2.  (2: L)  and in %s file EI_MAG2 == %c\n",argv[1],e_ident[EI_MAG2]);
          printf("EI_MAG3  The fourth byte of the magic number.  It must befilled with ELFMAG3.  (3: F) and in %s file EI_MAG3 == %c\n",argv[1],e_ident[EI_MAG3]);
          printf("EI_CLASS The fifth byte identifies the architecture for this binary  and in %s file EI_CLASS == %x\n",argv[1],e_ident[EI_CLASS]);
          switch (e_ident[EI_CLASS]){
            case ELFCLASSNONE:
                printf("In file %s EI_CLASS=ELFCLASSNONE This class is invalid.\n",argv[1]);
                break;
            case ELFCLASS32:
                printf("In file %s EI_CLASS=ELFCLASS32 This defines the 32-bit architec-ture.  It supports machines with files and virtual address  spaces up to 4 Gigabytes.\n",argv[1]);
                break;
            case ELFCLASS64:
                printf("In file %s EI_CLASS=ELFCLASS64  This defines the 64-bit architec-ture.\n",argv[1]);
                break;
          }
          printf("EI_DATA  The sixth byte specifies the data encoding of the processor-specific data in the file. Currently, these encodings are supported and in %s file EI_DATA == %x\n",argv[1],e_ident[EI_DATA]);
          switch (e_ident[EI_DATA]){
            case ELFDATANONE:
                printf("In file %s EI_DATA=ELFDATANONE  Unknown data format.\n",argv[1]);
                break;
            case ELFDATA2LSB:
                printf("In file %s EI_DATA=ELFDATA2LSB  Two's complement, little-endian.\n",argv[1]);
                break;
            case ELFDATA2MSB:
                printf("In file %s EI_DATA=ELFDATA2MSB  Two's complement, big-endian.\n",argv[1]);
                break;
          }
          printf("EI_VERSION The seventh byte is the version number of the ELF specification: and in %s file EI_VERSION == %x\n",argv[1],e_ident[EI_VERSION]);
          switch (e_ident[EI_VERSION]){
            case EV_NONE:
                printf("In file %s EI_VERSION=EV_NONE  Invalid version.\n",argv[1]);
                break;
            case EV_CURRENT:
                printf("In file %s EI_VERSION=EV_CURRENT  Current version.\n",argv[1]);
                break;
          }
          printf("EI_OSABI The eighth byte identifies the operating system and ABI to which the object is targeted.  Some fields in other ELF structures have flags and values  that have platform-specific meanings; the Stand-alone  (embedded) ABIinterpretation of those fields is determined by the value of this byte. For example: in %s file EI_OSABI == %x\n",argv[1],e_ident[EI_OSABI]);
          switch (e_ident[EI_OSABI]){
            case ELFOSABI_NONE:
                printf("In file %s EI_OSABI=ELFOSABI_NONE  Same as ELFOSABI_SYSV ( UNIX System V ABI ).\n",argv[1]);
                break;
            case ELFOSABI_HPUX:
                printf("In file %s EI_OSABI=ELFOSABI_HPUX  HP-UX ABI.\n",argv[1]);
                break;
            case ELFOSABI_NETBSD:
                printf("In file %s EI_OSABI=ELFOSABI_NETBSD  NetBSD ABI.\n",argv[1]);
                break;
            case ELFOSABI_LINUX:
                printf("In file %s EI_OSABI=ELFOSABI_LINUX  Linux ABI.\n",argv[1]);
                break;
            case ELFOSABI_SOLARIS:
                printf("In file %s EI_OSABI=ELFOSABI_SOLARIS  Solaris ABI.\n",argv[1]);
                break;
            case ELFOSABI_IRIX:
                printf("In file %s EI_OSABI=ELFOSABI_IRIX  IRIX ABI.\n",argv[1]);
                break;
            case ELFOSABI_FREEBSD:
                printf("In file %s EI_OSABI=ELFOSABI_FREEBSD  FreeBSD ABI.\n",argv[1]);
                break;
            case ELFOSABI_TRU64:
                printf("In file %s EI_OSABI=ELFOSABI_TRU64  TRU64 UNIX ABI.\n",argv[1]);
                break;
            case ELFOSABI_ARM:
                printf("In file %s EI_OSABI=ELFOSABI_TRU64  ARM architecture ABI.\n",argv[1]);
                break;
            case ELFOSABI_STANDALONE:
                printf("In file %s EI_OSABI=ELFOSABI_STANDALONE  Stand-alone (embedded) ABI.\n",argv[1]);
                break;
          }          
          printf("EI_ABIVERSION  The ninth byte identifies the version of the ABI to which the object is targeted.  This field is used to distinguish among incompatible versions of an ABI.  The interpretation of this version number is dependent on the ABI identified by the EI_OSABI field.  Applications conforming to this specifica‐ tion use the value 0.  and in %s file EI_ABIVERSION == %x\n",argv[1],e_ident[EI_ABIVERSION] );
          printf("EI_PAD  Start of padding.  These bytes are reserved and set to zero.  Programs which read them should ignore them.  The value for EI_PAD will change in the future if currently unused bytes are given meanings.  and in %s file EI_PAD == %x\n",argv[1],e_ident[EI_PAD] );
          printf("EI_NIDENT The size of the e_ident array. and in %s file EI_NIDENT == %x\n",argv[1],e_ident[EI_NIDENT]);
          
//#########################################################################################################################################
  // End of head 
//#########################################################################################################################################
 //#########################################################################################################################################
  // read e_type 
//#########################################################################################################################################
  uint16_t e_type,e_type_for_switch;
  fread(&e_type, 1, sizeof(e_type), file);

  if (e_ident[EI_DATA]!=ELFDATA2LSB){
      e_type_for_switch =swap_uint16(e_type);
  }else{e_type_for_switch=e_type;}
    printf("e_type This member of the structure identifies the object file type: and in %s file e_type == %x \n",argv[1],e_type_for_switch);
         switch (e_type_for_switch){
            case ET_NONE:
                printf("In file %s e_type=ET_NONE  An unknown type.\n",argv[1]);
                break;
            case ET_REL:
                printf("In file %s e_type=ET_REL A relocatable file.\n",argv[1]);
                break;
            case ET_EXEC:
                printf("In file %s e_type=ET_EXEC An executable file.\n",argv[1]);
                break;
            case ET_DYN:
                printf("In file %s e_type=ET_DYN A shared object.\n",argv[1]);
                break;
            case ET_CORE:
                printf("In file %s e_type=ET_CORE A core file.\n",argv[1]);
                break;
            case ET_LOPROC:
                printf("In file %s e_type=ET_LOPROC   Processor-specific.\n",argv[1]);
                break;
            case ET_HIPROC:
                printf("In file %s e_type=ET_HIPROC   Processor-specific.\n",argv[1]);
                break;
          }
   //#########################################################################################################################################
  // END read e_type 
//######################################################################################################################################### 
  //#########################################################################################################################################
  //  read e_machine 
//#########################################################################################################################################
uint16_t e_machine,e_machine_for_switch;
  fread(&e_machine, 1, sizeof(e_machine), file);

  if (e_ident[EI_DATA]!=ELFDATA2LSB){
      e_machine_for_switch =swap_uint16(e_machine);
  }else{e_machine_for_switch=e_type;}

           printf("e_machine This member's value specifies the required architecture for an individual file.  and in %s file e_machine == %d \n",argv[1],e_machine_for_switch);
         switch (e_machine_for_switch){
            case EM_NONE:
                printf("In file %s e_machine=EM_NONE  An unknown machine. \n",argv[1]);
                break;
            case EM_M32:
                printf("In file %s e_machine=EM_M32 AT&T WE 32100. \n",argv[1]);
                break;
            case EM_SPARC:
                printf("In file %s e_machine=EM_SPARC Sun Microsystems SPARC. \n",argv[1]);
                break;
            case EM_386:
                printf("In file %s e_machine=EM_386 Intel 80386. \n",argv[1]);
                break;
            case EM_68K:
                printf("In file %s e_machine=EM_68K Motorola 68000. \n",argv[1]);
                break;
            case EM_88K:
                printf("In file %s e_machine=EM_88K Motorola 88000.  \n",argv[1]);
                break;
            case EM_860:
                printf("In file %s e_machine=EM_860 Intel 80860.  \n",argv[1]);
                break;
            case EM_MIPS:
                printf("In file %s e_machine=EM_MIPS MIPS RS3000 (big-endian only).  \n",argv[1]);
                break;
            case EM_PARISC:
                printf("In file %s e_machine=EM_PARISC HP/PA.  \n",argv[1]);
                break;
            case EM_SPARC32PLUS:
                printf("In file %s e_machine=EM_SPARC32PLUS SPARC with enhanced instruction set.  \n",argv[1]);
                break;
            case EM_PPC:
                printf("In file %s e_machine=EM_PPC PowerPC.  \n",argv[1]);
                break;
            case EM_PPC64:
                printf("In file %s e_machine=EM_PPC64 PowerPC 64-bit.  \n",argv[1]);
                break;
            case EM_S390:
                printf("In file %s e_machine=EM_S390 IBM S/390.  \n",argv[1]);
                break;
            case EM_ARM:
                printf("In file %s e_machine=EM_ARM Advanced RISC Machines.  \n",argv[1]);
                break;
            case EM_SH:
                printf("In file %s e_machine=EM_SH Renesas SuperH.  \n",argv[1]);
                break;
            case EM_SPARCV9:
                printf("In file %s e_machine=EM_SPARCV9 SPARC v9 64-bit.  \n",argv[1]);
                break;
            case EM_IA_64:
                printf("In file %s e_machine=EM_IA_64 Intel Itanium.  \n",argv[1]);
                break;
            case EM_X86_64:
                printf("In file %s e_machine=EM_IA_64 AMD x86-64.  \n",argv[1]);
                break;
            case EM_VAX:
                printf("In file %s e_machine=EM_VAX DEC Vax.   \n",argv[1]);
                break;
          }
   //#########################################################################################################################################
  // END read e_machine 
//#########################################################################################################################################
           //#########################################################################################################################################
  // read e_version 
//#########################################################################################################################################
  uint16_t e_version,e_version_for_switch;
  fread(&e_version, 1, sizeof(e_version), file);

  if (e_ident[EI_DATA]!=ELFDATA2LSB){
      e_version_for_switch =swap_uint16(e_version);
  }else{e_version_for_switch=e_version;}
    printf("e_version This member identifies the object file version.  and in %s file e_type == %x \n",argv[1],e_type_for_switch);
         switch (e_type_for_switch){
            case EV_NONE:
                printf("In file %s e_version=ET_NONE  Invalid version\n",argv[1]);
                break;
            case EV_CURRENT:
                printf("In file %s e_version=EV_CURRENT Current version.\n",argv[1]);
                break;
          }
   //#########################################################################################################################################
  // END read e_version 
//######################################################################################################################################### 

           //#########################################################################################################################################
  // read e_version 
//#########################################################################################################################################
  uint16_t e_entry,e_entry_for_switch;
  fread(&e_entry, 1, sizeof(e_entry), file);

  if (e_ident[EI_DATA]!=ELFDATA2LSB){
      e_entry_for_switch =swap_uint16(e_version);
  }else{e_version_for_switch=e_version;}
    printf("e_version This member identifies the object file version.  and in %s file e_type == %x \n",argv[1],e_type_for_switch);
         switch (e_type_for_switch){
            case EV_NONE:
                printf("In file %s e_version=ET_NONE  Invalid version\n",argv[1]);
                break;
            case EV_CURRENT:
                printf("In file %s e_version=EV_CURRENT Current version.\n",argv[1]);
                break;
          }
   //#########################################################################################################################################
  // END read e_version 
//######################################################################################################################################### 

  fclose(file);
}