#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
/*structs and data types */

typedef struct  dec_node dec_node;

struct dec_node{
	bool leaf; 
	char c; 
	dec_node* left; 
	dec_node* right; 
};

/*functions*/

bool decompress(FILE* comp_file, FILE* decomp_file);
dec_node* make_tree(FILE* f);
void free_dec_node_tree(dec_node* root);
void write_to_decomp_file(uint32_t file_size, FILE* comp_file, FILE* decomp_file,dec_node* root);

#endif
