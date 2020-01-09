#include <string.h> 
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include "all_elf_func.h"

#define N_ligne 16

void initElf(Elf32_info *elf,FILE *file){
	//lire le contenu de l'en-tête
	if(fread(&elf->header,sizeof(elf->header),1,file)){
			//printf("réussir à lire header\n");
			//if big endian then swap to little endian
			if(elf->header.e_ident[EI_DATA]==2){		
				setto_little_endian(elf);
			}
	}
	if(elf->header.e_shnum == 0){
		printf("\nIl n'y a pas de sections dans ce fichier.\n");
		
	}else{
		elf->section = malloc(sizeof(Elf32_Shdr) * swap_uint16(elf->header.e_shnum)); //allocation de table de section

		lire_Section_table(elf,file);  // lire le contenu de table des sections
		init_strtable(elf,file);		//Alloue la mémoire pour stocker la table contenant les noms des symboles.
	}
	read_progbits(elf,file);
	read_Sym(elf,file); //Stocke les informations relatives à la table des symboles dans la structure elf.
	read_SymTable(elf,file); //Alloue la mémoire pour stocker la table contenant les noms des symboles.

	read_Rel(elf,file); // Stocke les informations relatives à la table des relocations dans la structure elf.

	

}

void liberer(Elf32_info elf){
	int i,j;	
	int nr_valeurs ;
	

	for(i = 0 ; i <elf.header.e_shnum;i++){
		if (elf.section[i].sh_type==SHT_SYMTAB){			
			nr_valeurs = elf.section[i].sh_size / elf.section[i].sh_entsize;			
			for(j = 0 ; j<nr_valeurs;j++ ){
				free(elf.symtable[j]);
				free(elf.reltab[j]);
			}
		}
	}
	free(elf.symtable);
	free(elf.symtab);
	free(elf.reltab);
	free(elf.section);
	free(elf.strtable);
	
}

void getNbSection(Elf32_info elf){ 
    FILE *fout;
    fout=fopen("section.tmp","w");
    if(fout==NULL)
    {
        printf("File cannot open! " );
        exit(0);
    }
 	
    //écrire le nombre de section dans le fichier tmp.txt pour générer le chiffre aléatoire entre 0 et nombre de section
    fprintf(fout,"%d\t", elf.header.e_shnum);
    fclose(fout);
   
}


void read_progbits(Elf32_info *elf,FILE *file){
	int nb=0;
	int tableSize=0;
	for(int i=0;i<elf->header.e_shnum;i++){
		if(elf->section[i].sh_type==SHT_SYMTAB){
			nb=(elf->section[i].sh_size/elf->section[i].sh_entsize)+nb;
		}
	}
	for (int i = 0; i < elf->header.e_shnum; ++i){
		if ((elf->section[i].sh_type==SHT_PROGBITS)||(elf->section[i].sh_type==SHT_NULL)||(elf->section[i].sh_type==SHT_STRTAB)||(elf->section[i].sh_type==SHT_ARM_ATTRIBUTES)||(elf->section[i].sh_type==SHT_REL)){
			for (int j = 0;j<nb;j++)tableSize++;
		}
	}
	elf->progbits=malloc(tableSize * sizeof (unsigned char *));
	for(int i = 0;i<elf->header.e_shnum;i++){
		if ((elf->section[i].sh_type==SHT_PROGBITS)||(elf->section[i].sh_type==SHT_NULL)||(elf->section[i].sh_type==SHT_STRTAB)||(elf->section[i].sh_type==SHT_ARM_ATTRIBUTES)||(elf->section[i].sh_type==SHT_REL)){
              fseek(file,elf->section[i].sh_offset, SEEK_SET);
              elf->progbits[i] = malloc(elf->section[i].sh_size * sizeof(unsigned char));
              if(fread(elf->progbits[i], 1,elf->section[i].sh_size, file)){}
		}
	}
}

int funForFindIndex(Elf32_info elfo,Elf32_info elft,int ind1){
	for (int i = 0; i < elft.header.e_shnum; i++) {
		char *name1=(char*)elfo.strtable+elfo.section[ind1].sh_name;
		char *name2=(char*)elft.strtable+elft.section[i].sh_name;
		if(strcmp(name1,name2)==0){
			return i;
		}
	}
	return -1;
}

void p2(Elf32_info elfo,Elf32_info elft){
	int ind;
	int nbsect=0;
	Elf32_Ehdr header;
	unsigned char **progbits;
	
	char *name=(char*)elfo.strtable+elfo.section[elfo.header.e_shstrndx].sh_name;
	uint16_t e_shstrndx;
	uint32_t e_shoff=0;
	//############## don't need change or same #########################3
	memcpy(header.e_ident,elfo.header.e_ident,sizeof(elfo.header.e_ident));
	header.e_type=elfo.header.e_type;
	header.e_machine=elfo.header.e_machine;
	header.e_version=elfo.header.e_version;
	header.e_entry=elfo.header.e_entry;
	header.e_phoff=elfo.header.e_phoff;
	header.e_flags=elfo.header.e_flags;
	header.e_ehsize=elfo.header.e_ehsize;
	header.e_phentsize=elfo.header.e_phentsize;
	header.e_shentsize=elfo.header.e_shentsize;
	//##############################################################

	for (int i = 0; i < elfo.header.e_shnum; i++)
		if((elfo.section[i].sh_type==SHT_STRTAB)||(elfo.section[i].sh_type==SHT_ARM_ATTRIBUTES)||(elfo.section[i].sh_type==SHT_PROGBITS)||(elfo.section[i].sh_type==SHT_NULL)||(elfo.section[i].sh_type==SHT_REL))nbsect++;
	for (int i = 0; i < elft.header.e_shnum; i++){
		char *name1=(char*)elft.strtable+elft.section[i].sh_name;
		if((elft.section[i].sh_type==SHT_STRTAB)||(elft.section[i].sh_type==SHT_ARM_ATTRIBUTES)||(elft.section[i].sh_type==SHT_PROGBITS)||(elft.section[i].sh_type==SHT_NULL)||(elft.section[i].sh_type==SHT_REL)){
			ind=funForFindIndex(elft,elfo,i);
			if(ind==-1){
				printf("[%s]",name1);nbsect++;	
			}
		}
	}
	progbits=malloc((nbsect) * sizeof (unsigned char *));
	int a=0;
	for (int i = 0; i < elfo.header.e_shnum; i++){
		
		ind=funForFindIndex(elfo,elft,i);
		if((elfo.section[i].sh_type==SHT_PROGBITS)||(elfo.section[i].sh_type==SHT_NULL)||(elfo.section[i].sh_type==SHT_REL)){
			if(ind!=-1){
				progbits[a]=malloc((elfo.section[i].sh_size+elft.section[ind].sh_size)* sizeof(unsigned char));
				e_shoff=e_shoff+elfo.section[i].sh_size+elft.section[ind].sh_size;
				for(int k=0;k<elfo.section[i].sh_size;k++){
					progbits[a][k]=elfo.progbits[i][k];
				}
				for(int k=0;k<elft.section[ind].sh_size;k++){
					progbits[a][k+elfo.section[i].sh_size]=elft.progbits[ind][k];
				}a++;
			}else{
				progbits[a]=malloc((elfo.section[i].sh_size) * sizeof(unsigned char));
				e_shoff=e_shoff+elfo.section[i].sh_size;
				for(int k=0;k<elfo.section[i].sh_size;k++){
					progbits[a][k]=elfo.progbits[i][k];
				}a++;
			}
		}
	}
	for (int i = 0; i < elft.header.e_shnum; i++){
		
		ind=funForFindIndex(elft,elfo,i);
		if((elft.section[i].sh_type==SHT_PROGBITS)||(elft.section[i].sh_type==SHT_NULL)||(elft.section[i].sh_type==SHT_REL)){
			if(ind==-1){
				progbits[a]=malloc((elft.section[i].sh_size) * sizeof(unsigned char));
				e_shoff=e_shoff+elft.section[i].sh_size;
				for(int k=0;k<elft.section[i].sh_size;k++){
					progbits[a][k]=elft.progbits[ind][k];
				}a++;
			}
		} 
	}
	for (int i = 0; i < elfo.header.e_shnum; i++){
		char *name1=(char*)elfo.strtable+elfo.section[i].sh_name;
		ind=funForFindIndex(elfo,elft,i);
		if((elfo.section[i].sh_type==SHT_STRTAB)||(elfo.section[i].sh_type==SHT_ARM_ATTRIBUTES)){
			if(strcmp(name,name1)==0){e_shstrndx=a;}
			if(ind!=-1){
				progbits[a]=malloc((elfo.section[i].sh_size+elft.section[ind].sh_size)* sizeof(unsigned char));
				e_shoff=e_shoff+elfo.section[i].sh_size+elft.section[ind].sh_size;
				for(int k=0;k<elfo.section[i].sh_size;k++){
					progbits[a][k]=elfo.progbits[i][k];
				}
				int index=1,skip=0,b=0,z;
				while(index+skip<elft.section[ind].sh_size){
					do{
						for(int k=0;k<elfo.section[i].sh_size;k++){
							z=index+skip;
							while(elft.progbits[ind][z]!=0){
								if(elfo.progbits[i][k+z-index-skip]==elft.progbits[ind][z]){b=1;}else{b=0;}
								z++;	
							}
							if(b)break;
						}
						if(!b){
							progbits[a][index+elfo.section[i].sh_size]=elft.progbits[ind][index+skip];
						}
						index++;
					}while(elft.progbits[ind][index+skip]!=0);
				}a++;
			}else{
				progbits[a]=malloc((elfo.section[i].sh_size) * sizeof(unsigned char));
				e_shoff=e_shoff+elfo.section[i].sh_size;
				for(int k=0;k<elfo.section[i].sh_size;k++){
					progbits[a][k]=elfo.progbits[i][k];
				}a++;
			}
		}
	}
	for (int i = 0; i < elfo.header.e_shnum; i++){
		//char *name1=elfo.strtable+elfo.section[i].sh_name;
		ind=funForFindIndex(elfo,elft,i);
		if((elft.section[i].sh_type==SHT_STRTAB)||(elft.section[i].sh_type==SHT_ARM_ATTRIBUTES)){
			if(ind==-1){
				progbits[a]=malloc((elft.section[i].sh_size) * sizeof(unsigned char));
				e_shoff=e_shoff+elft.section[i].sh_size;
				for(int k=0;k<elft.section[i].sh_size;k++){
					progbits[a][k]=elft.progbits[i][k];
				}a++;
			}
		}
	}
	header.e_shnum=elfo.header.e_shnum;
	header.e_shstrndx=e_shstrndx;
	header.e_shoff=e_shoff;
	
}


void insert_bit(FILE *f3, Elf32_info *elf2, int dec_from_be, int size_of_fo, int size2 ,int nr_sec){ // la funtion pour concatener les bits

  int i=0;
  int size; //variable pour stocker combien des bits on doit copier apres le lieu ou on mis nos bits
  int tmp1, tmp2;

  fseek(f3 ,dec_from_be+size_of_fo, SEEK_SET); // la place ou on mis les nouveau donnes
  tmp1=ftell(f3);  // la position
  fseek(f3 ,0 , SEEK_END); //
  tmp2=ftell(f3);
  size=tmp2-tmp1; //


  unsigned char buf[size]; // varialbe pour stocker les donnes qui sont apres le lieu d'insertion

  fseek(f3 ,dec_from_be+size_of_fo, SEEK_SET);
  if(fread(buf, 1, sizeof(buf), f3 )){} //
  fseek(f3 ,dec_from_be+size_of_fo, SEEK_SET);

  for ( int y=0; y < elf2->section[nr_sec].sh_size; y++){
    fputc(elf2->progbits[nr_sec][y], f3);
  }

  fseek(f3 ,dec_from_be+size_of_fo+size2, SEEK_SET); // ici on doit mettre les bits qu'on a stocker dans buf

  while(i<size){
    fputc(buf[i], f3);
    i++;
  }

}

void insert_bit_end(FILE *f3, Elf32_info *elf2, int nr_sec, int size){  //la function pour ecrir les bits a la fin du fichier
  fseek(f3 ,0 , SEEK_END);
  for ( int y=0; y < elf2->section[nr_sec].sh_size; y++){
    fputc(elf2->progbits[nr_sec][y], f3);
  }
}






void concatener(Elf32_info *elf1, Elf32_info *elf2, Elf32_info *elf3 ,FILE *f3, FILE *f2){  //la functions qui parcours les deux ELF files et les concatenne
  //printf("Num: Valeur      Tail  Type     Lien   Vis         Ndx  Nom   \n");


  int decalage=0;   // le decalage qu'on obtien apres chaque concatenation
  int dec1=0;       //le decalage d'une section du premier ELF
  int size1=0;    //la dimension d'une section du premier ELF
  //int dec2=0;    //le decalage d'une section du deuxieme ELF
  int size2=0;    //la dimension d'une section du deuxieme ELF
  int size_fi=0;   // la dimension des section concatener
  int decalage_fi=0;   // le decalage des section concatener
  int x;
  int y;
  int i;
  int j;
  int pr=1;   // un variable pour pouvoir verifie si il y a des section dans ELF2 que ne sont pas dans ELF1

  for (i=0; i<= elf1->header.e_shnum; i++){
    if( elf1->section[i].sh_type==SHT_PROGBITS){
      char *name1=(char*)elf1->strtable+elf1->section[i].sh_name;
      for (j=0; j<=elf2->header.e_shnum; j++){
        char *name2=(char*)elf2->strtable+elf2->section[j].sh_name;
        if( elf2->section[j].sh_type==SHT_PROGBITS){     //si les de section sont des section SHT_PROGBITS et leur noms sont la meme, on fait la concatenation
          if (strcmp(name1,name2)==0){
            printf("\n%15s  : ",name1);
            printf("(ELF1:) ");
            for ( x=0; x < elf1->section[i].sh_size; x++){
              if((x)%4==0)
                printf(" ");
              printf("%02x",elf1->progbits[i][x] ); }

            printf(" || (ELF2:)  ");

            for ( y=0; y < elf2->section[j].sh_size; y++){
              if((y)%4==0)
                printf(" ");
              printf("%02x",elf2->progbits[j][y] ); }

            dec1=elf1->section[i].sh_offset;


            size1=elf1->section[i].sh_size;
            size2=elf2->section[j].sh_size;
            insert_bit(f3, elf2, dec1+decalage, size1 , size2 , j);

            size_fi=size1+size2;
            decalage_fi=dec1+decalage;

            elf3->section[i].sh_offset=decalage_fi;
            elf3->section[i].sh_size=size_fi;

            decalage=decalage+size2;

            printf("\n");
            break;
          }
        }
        else if (strcmp(name1,name2)!=0  && j==elf2->header.e_shnum ){ // si il y a des section dans ELF1 qui ne sont pas dans ELF2
          printf("\n%15s  : ",name1);
          printf("(ELF1:) ");

          for ( x=0; x < elf1->section[i].sh_size; x++){
            if((x)%4==0)
              printf(" ");
            printf("%02x",elf1->progbits[i][x] ); }

          dec1=elf1->section[i].sh_offset;
          decalage_fi=dec1+decalage;
          size_fi=size1;

          elf3->section[i].sh_offset=decalage_fi;
          elf3->section[i].sh_size=size_fi;
          decalage=decalage+size2;


          printf("\n");
          break;

          }
      }
    }
      else{
        elf3->section[i].sh_offset=elf3->section[i].sh_offset+decalage;
      }

  }

  decalage=elf1->section[elf1->header.e_shnum-1].sh_offset+decalage;
  for (i=0; i <= elf2->header.e_shnum; i++){    // si il y a des section dans ELF2 qui ne sont pas dans ELF3
    if( elf2->section[i].sh_type==SHT_PROGBITS){
      pr=1;
      char *name1=(char*)elf2->strtable+elf2->section[i].sh_name;
      for (j=0; j<=elf1->header.e_shnum; j++){
        char *name2=(char*)elf1->strtable+elf1->section[j].sh_name;
        if (strcmp(name1,name2)==0) {
          pr=0;
        }
      }
      if (pr==1){
        printf("\n%15s  : ",name1);
        printf("(ELF2:) ");

        for ( x=0; x < elf2->section[i].sh_size; x++){
          if((x)%4==0)
            printf(" ");
          printf("%02x",elf2->progbits[i][x] ); }



        size1=elf2->section[i].sh_size;

        insert_bit_end(f3, elf2, i, size1);


        // on ajoute des information pour la nouvelle section
        elf3->section[elf3->header.e_shnum].sh_name = elf2->section[i].sh_name;
        elf3->section[elf3->header.e_shnum].sh_type =elf2->section[i].sh_type;
        elf3->section[elf3->header.e_shnum].sh_addr =elf2->section[i].sh_addr;
        elf3->section[elf3->header.e_shnum].sh_offset =decalage+size1;
        elf3->section[elf3->header.e_shnum].sh_size =elf2->section[i].sh_size;
        elf3->section[elf3->header.e_shnum].sh_entsize =elf2->section[i].sh_entsize;
        elf3->section[elf3->header.e_shnum].sh_flags =elf2->section[i].sh_flags;
        elf3->section[elf3->header.e_shnum].sh_link =elf2->section[i].sh_link;
        elf3->section[elf3->header.e_shnum].sh_info =elf2->section[i].sh_info;
        elf3->section[elf3->header.e_shnum].sh_addralign = elf2->section[i].sh_addralign;


        elf3->header.e_shnum++;

        decalage=decalage+size1;
        printf("\n");
        pr=0;
      }
    }
  }
}
