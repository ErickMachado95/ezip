#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "compression.h"

//comment out for debugging purposes
#define DEBUG 

bool compression(FILE* file){

		
	ascii_node** index_list = (ascii_node**) malloc(sizeof(ascii_node*)*256);
	if(index_list == NULL) {return false;} // failed to compress
	
	char ch = getc(file); 

	while( ch != EOF){ 
		
		increment_char_count(index_list,ch);
		ch = getc(file);
	}

	#ifdef DEBUG
		print_char_frequency(index_list,256);
	#endif
	
	return true;
}

/* returns a newly initialized ascii_node_list */
/*input: slots to be initialized (length) */
/*output: new ascii node list, (returns -1 if fails) */
ascii_node_list* new_ascii_node_list(int init_len){			

}

/*increments a characters frequency value*/
/*input: index list and the character to be incremented */
/*output: none */
void increment_char_count(ascii_node** list, char ch){
	
	uint8_t index = (uint8_t) ch; 
	if(list[index] == NULL){ 
		list[index] = new_ascii_node(ch,false); // if no new node yet exists then make one
	}else{
		list[index]->freq++; // updates frequency
	}
}

/* creates a new ascii node and initializes it (frequency starts at 1) */
/* input: the character, and it's operation status */
/* output: new ascii node */
ascii_node*  new_ascii_node(char ch, bool no_op){
	ascii_node* an = (ascii_node*) malloc(sizeof(ascii_node)); 
	an->ascii_char = ch; 
	an->no_op = no_op; 
	an->freq = 1; 
	an->code = NULL; 
	an->code_length = 0; 	
}

/* orders a given ascii_list from lowest frequency to highest*/
/*input: ascii_node_list */
/*output: ordered list */
void order_list(ascii_node_list* list){

}


/*prints the frequency of characters found in a file */
/*input: list of ascii nodes */
/*output: prints the character and it's frequency */  

void print_char_frequency(ascii_node** list, int length){
	printf("Frequency of ascii nodes:\n\n c - char   f - frequency\n\n\n");

	for(int i = 0 ; i < length ; i++){
		if(list[i] != NULL) { 
			printf("c: %c  f: %i \n\n",list[i]->ascii_char,list[i]->freq);
		}
	}
}

