#ifndef EZIP_H
#define EZIP_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*structures and other data types */
struct ascii_node{ 	
	bool no_op; // indicates wheter this node is not a character for Huffman code
	char ascii_char; 
	uint32_t freq;  //amount of times a character is found in a file
	uint32_t *code; //codes in bits
	uint32_t code_length; //lenght of code
} ;

typedef struct ascii_node ascii_node; 

struct ascii_node_list{
	int length; 
	ascii_node** node_list; 
} ; 

typedef struct ascii_node_list ascii_node_list; 



/*Functions*/
bool compression(FILE* file); 
ascii_node_list* new_ascii_node_list(int init_len);
void increment_char_count(ascii_node** list, char ch);
ascii_node*  new_ascii_node(char ch, bool no_op);
void order_list(ascii_node_list* list); 


/*debugging functions */
void print_char_frequency(ascii_node** list, int length);

#endif

