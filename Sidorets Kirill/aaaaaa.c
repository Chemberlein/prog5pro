void Readelf4(Elf32_info elf){
  int nr_valeurs;
  const char* nameSym = "";
  const char* name = "";
  for (int i = 0; i < elf.header.e_shnum; i++){
    if (elf.section[i].sh_name){name =(char*)elf.strtable+elf.section[i].sh_name];}
          if (elf.section[i].sh_type==SHT_SYMTAB){    	
          	 nr_valeurs = elf.strtable[elf.section[i].sh_size / elf.strtable[elf.section[i].sh_entsize;
	            printf("La table de symboles < %s > contient %d entrées: \n",name,nr_valeurs);
    	        printf("Num: Valeur      Tail  Type     Lien   Vis         Ndx  Nom   \n");
    	        for (int j=0; j < nr_valeurs; j++){
    	        	printf("%-5d",j);
              		printf("0x%08x ", elf.symtab[j].st_value);
              		printf("%-5d ", elf.symtab[j].st_size);   
	              switch(ELF32_ST_TYPE(elf.symtab[j].st_info)){
	                case STT_SECTION:
	                  printf("SECTION   ");
	                  break;

	                case STT_NOTYPE:
	                  printf("NOTYPE    ");
	                  break;

	                default:

	                  printf("ERROR   ");
	                  break;}

	              switch(ELF32_ST_TYPE(ELF32_ST_BIND(elf.symtab[j].st_info))){
	                case STB_LOCAL:
	                  printf("LOCAL  ");
	                  break;

	                case STB_GLOBAL:
	                  printf("GLOBAL ");
	                  break;

	                default:
	                  printf("ERROR   ");
	                  break;}
              if (elf.symtab[j].st_other==0)
                printf("DEFAULT     ");
              else
                printf("NOT DEFAULT ");

              printf("%d ", elf.symtab[j].st_shndx);
              if (elf.symtab[j].st_name){
                  nameSym =(char*)elf.symtable+elf.symtab[i].sh_name];
				}
                  printf("%s\n",nameSym);
            }
		}
    }
}
