#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "compression.h"


int main(int argc, char** argv){

	if(argc != 3){ 
		printf("Wrong number of arguments. See README for more information"); 
		exit(-1);
	}
	
	FILE* file; 	

	file = fopen(argv[1],"r"); 
	
	if(file == NULL){ 
		printf("Error, could not open file"); 
		exit(-1);
	}

	compression(file); 		
}
