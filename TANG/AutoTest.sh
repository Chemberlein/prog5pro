#!/bin/sh

reset="r"
repertoire="tests"
t=0
p="Les fichiers a et b sont identiques"
cd $repertoire
	if [ $reset = "r" ]
then
	for file in *.o
			do  

				   
				   echo \*\*         $file          \*\* 
				  
				   echo ""
					
			done



	echo Donner svp un fichier test ELF:
	read -r fic 
			
		for file in *
			do
					if [ $file = $fic ]
					then
						t=1
              					echo $t
							
					fi
			done
     if [ $t=1 ]
	then 
    	cd ..
       # ./configure
       # make 
        chmod +x readelf
	./readelf "$repertoire/$fic" "-h" > a
 	  readelf "-h" "$repertoire/$fic" > b
         ( diff -s a b ) > c
      
        var=`cat c`
        echo $var
        if [ "$var" = "$p" ]
		then 
      cat a
    echo "************************************"
      cat b
    echo ""
         echo " ******* Le contenu du header correct *******"
	fi
        truncate -s 0 a
        truncate -s 0 b
         
./readelf "$repertoire/$fic" "-S" | sed -e "s/ //g" | sed '$d' | sed '$d' | sed '$d' | sed '$d' | sed 3d | sed -e "s/arangesR/arangeR/g" > a
 
readelf "-S" "$repertoire/$fic" | sed -e "s/ //g"| sed '$d'| sed '$d' | sed '$d' | sed '$d'| sed '$d'| sed 3d > b  
    ( diff -s a b ) > c
      
        var=`cat c`
        echo $var
        if [ "$var" = "$p" ]
		then 
	cat a
    echo "************************************"
      cat b
    echo ""
			echo "************ le contenu des tables de sections sont correct *************"
	fi
      echo ""
	./readelf "$repertoire/$fic" "-s"|sed -e "s/ //g"|sed -e "s/DEFAULT0/DEFAULTUND/g"| sed 1d| sed -e "s/0x//g" > a
 	  readelf "-s" "$repertoire/$fic" | sed -e "s/ //g" | sed /^$/d | sed 1d > b
         ( diff -s a b ) > c
      
        var=`cat c`
        echo $var
        if [ "$var" = "$p" ]
		then 
      cat a
    echo "************************************"
      cat b
    echo ""
         echo " ******* Le contenu des symboles est correct *******"
	fi

	 echo ""
	./readelf "$repertoire/$fic" "-r" | sed -e "s/ //g"  | sed /^$/d| sed -e "s/0x//g"| sed -e "s/entrées/entrée/g" > a
 	  readelf "-r" "$repertoire/$fic" | sed -e "s/ //g" | sed /^$/d | sed -e "s/entrées/entrée/g"| sed -e "s/0x//g" > b
         ( diff -s a b ) > c
      
        var=`cat c`
        echo $var
        if [ "$var" = "$p" ]
		then 
      cat a
    echo "************************************"
      cat b
    echo ""
         echo " ******* La relocation est correcte *******"
	fi    
	
  	
	   
     fi

fi
