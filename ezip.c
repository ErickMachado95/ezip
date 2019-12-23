#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "compression.h"
#include "decompression.h"


int main(int argc, char** argv){

	if(argc != 4){ 
		printf("Error: wrong number of arguments. See README for more information\n"); 
		exit(-1);
	}
	



	if(strcmp("-c",argv[1]) == 0){
	

		FILE* file = fopen(argv[2],"r"); 
		
		if(file == NULL){ 
			printf("Error: could not open file %s\n",argv[2]); 
			exit(-1);
		}


		if(fopen(argv[3],"r") != NULL){// checks if file already exists
			printf("Error: file %s already exists\n",argv[3]);	
			fclose(file);	
			exit(-1);
			
		}

		FILE* comp_file = fopen(argv[3],"w");//creates file
		compression(file,comp_file); 		

		fclose(file);
		fclose(comp_file);

	}else if(strcmp("-d",argv[1]) == 0){


		FILE* comp_file = fopen(argv[2],"r"); 
		
		if(comp_file == NULL){ 
			printf("Error: could not open file %s\n",argv[2]); 
			exit(-1);
		}


		if(fopen(argv[3],"r") != NULL){// checks if file already exists
			printf("Error: file %s already exists\n",argv[3]);	
			fclose(comp_file);	
			exit(-1);
			
		}

		FILE* decomp_file = fopen(argv[3],"w");//creates file
		decompress(comp_file,decomp_file); 		

		fclose(decomp_file);
		fclose(comp_file);

	}else{

		printf("Error: Wrong parameters, use -c for compression and -d for decompression\n");
		exit(-1);
	}
}
