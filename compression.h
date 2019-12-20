#ifndef EZIP_H
#define EZIP_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*definitions*/
#define left_path(x,y) (x & (~(0x01 << (7-y))))
#define right_path(x,y) (x  | (0x01 << (7-y)))

/*structures and other data types */

typedef struct char_code char_code;

struct ascii_node{ 	
	char ascii_char; 
	uint32_t freq;  //amount of times a character is found in a file
	char_code* cc;
} ;

typedef struct ascii_node ascii_node; 

struct ascii_node_list{
	int length; 
	ascii_node** node_list; 
} ; 

typedef struct ascii_node_list ascii_node_list; 

typedef struct huff_node huff_node;

struct huff_node{
	int value; 
	bool intermediate; 
	ascii_node* an; 
	huff_node* left; 
	huff_node* right; 	
};



struct char_code{
	uint8_t code[32]; 
	uint32_t length; 
}; 



/*Functions*/
bool compression(FILE* file, FILE* comp_file); 
ascii_node_list* new_ascii_node_list(int init_len);
void increment_char_count(ascii_node** list, char ch);
ascii_node*  new_ascii_node(char ch);
void free_ascii_node(ascii_node* an);
void order_list(ascii_node_list* list); 
huff_node* huffman_process(ascii_node** list, int list_len); 
huff_node* new_huff_node(int value,bool inter,ascii_node* an);
void code_assignment(huff_node* tree);
void code_assignment_recurs(huff_node* root, char_code* cd); 
void char_code_copy(char_code* dest, char_code* src);
bool write_compressed_file(FILE* src, FILE* dest,ascii_node** list, int list_len, huff_node* root);
void write_huffman_tree(FILE* file,huff_node* root);
/*debugging functions */
void print_char_frequency(ascii_node** list, int length);
void code_print(char_code* cc);
#endif

