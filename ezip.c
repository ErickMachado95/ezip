#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "compression.h"


int main(int argc, char** argv){

	if(argc != 3){ 
		printf("Error: wrong number of arguments. See README for more information"); 
		exit(-1);
	}
	
	FILE* file = fopen(argv[1],"r"); 
	
	if(file == NULL){ 
		printf("Error: could not open file %s",argv[1]); 
		exit(-1);
	}


	if(fopen(argv[2],"r") != NULL){// checks if file already exists
		printf("Error: file %s already exists",argv[2]);	
		fclose(file);	
		exit(-1);
		
	}

	FILE* comp_file = fopen(argv[2],"w");//creates file


	compression(file,comp_file); 		
}
