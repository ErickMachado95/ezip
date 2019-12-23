#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "compression.h"

//comment out for debugging purposes
//#define DEBUG 

bool compression(FILE* file, FILE* comp_file){
	
	ascii_node** index_list = (ascii_node**) malloc(sizeof(ascii_node*)*256);
	if(index_list == NULL) 
		return false; // failed to compress


	fseek(file,0,SEEK_END);	
	uint64_t length_of_file = ftell(file);
	char ch;
	rewind(file);
	for(uint64_t i = 0; i < length_of_file ; i++){ 
					
		fread(&ch,sizeof(char),1,file);
		increment_char_count(index_list,ch);
	}

	rewind(file);

	#ifdef DEBUG
		print_char_frequency(index_list,256);
	#endif
	
	int char_count = 0; 

	//counts how many characters are actually found in index_list
	for(int i = 0 ; i < 256; i++){
		if(index_list[i] != NULL)
			char_count++;
	}

	ascii_node** full_list = (ascii_node**) malloc(sizeof(ascii_node*)*char_count);
	
	if(full_list == NULL){
		free(index_list);
		return false; 
	}

	/* makes a complete list absent of NULLs */
	for(int i = 0, k = 0 ; (i < 256) && (k < char_count) ; i++){
		if(index_list[i] != NULL){ 
			full_list[k++] = index_list[i]; 		
		}
	}
	
	huff_node* root = huffman_process(full_list,char_count);
	
	write_compressed_file(file,length_of_file,comp_file,index_list,char_count,root);

	for( int i = 0 ; i < char_count; i++){
		free_ascii_node(full_list[i]);	
	}
	free(full_list);
	free(index_list);
}


/*increments a characters frequency value*/
/*input: index list and the character to be incremented */
/*output: none */
void increment_char_count(ascii_node** list, char ch){
	
	uint8_t index = (uint8_t) ch; 
	if(list[index] == NULL){ 
		list[index] = new_ascii_node(ch); // if no new node yet exists then make one
	}else{
		list[index]->freq++; // updates frequency
	}
}

/* creates a new ascii node and initializes it (frequency starts at 1) */
/* input: the character, and it's operation status */
/* output: new ascii node */
ascii_node*  new_ascii_node(char ch){
	ascii_node* an = (ascii_node*) malloc(sizeof(ascii_node)); 
	an->ascii_char = ch; 
	an->freq = 1; 
	an->cc = (char_code*) malloc(sizeof(char_code));
	an->cc->length = 0; 	
	return an;
}

/* frees ascii_node include the pointer to char_code*/
/*input: ascii code to be freed */
/*output: none */
void free_ascii_node(ascii_node* an){
	if(an == NULL){ return; }
	free(an->cc);
	free(an); 
}



/*creates a new huff node needed for huffman coding*/
/*inputs: value (frequency), inter - intermediate node or not, an - ascii_node if applicable */
/*output: a new huff_node, -1 if memory allocation fails */
huff_node* new_huff_node(int value,bool inter,ascii_node* an){
	huff_node* hn = (huff_node*) malloc(sizeof(huff_node)); 
	hn->value = value; 
	hn->intermediate = inter; 
	hn->an = an; 
	hn->left = NULL; 
	hn->right = NULL; 
	return hn;
}


/*huffman process takes an ascii_node list and uses huffman coding to create a code*/
/*input: ascii_node list*/
/*output: returns root of huffman tree */

huff_node*  huffman_process(ascii_node** list, int list_len){
	
	if(list_len == 1){} ; // special case of list being 1	
	huff_node** huff_tree = (huff_node**) malloc(sizeof(huff_node*)*list_len);
	
	//creates a huff node for each character in list
	for( int i = 0 ; i < list_len ; i++){
		huff_tree[i] = new_huff_node(list[i]->freq,false,list[i]);	
	}
	
	huff_node dummy_node; 
	dummy_node.value = 0x7FFFFFFF; 	
	huff_node* root;
	for( int i = 0; i < list_len - 1 ; i++){
		huff_node* min_one = &dummy_node;
		huff_node* min_two = &dummy_node; 
		int insert_index = 0 ; 
		for( int k = 0 ; k < list_len ; k++){
			if(huff_tree[k] != NULL && huff_tree[k]->value < min_one->value){
				min_one = huff_tree[k];
				insert_index = k; 
			}
		}
			
		huff_tree[insert_index] = NULL;

		for( int j = 0 ; j < list_len ; j++){
			if(huff_tree[j] != NULL && huff_tree[j]->value < min_two->value){
				min_two = huff_tree[j]; 
				insert_index = j; 
			}
		}

		if( i == (list_len-2)){
			root = new_huff_node(min_one->value + min_two->value,true,NULL);
			root->left = min_one;
			root->right = min_two; 
		}else{
		huff_tree[insert_index] = new_huff_node(min_one->value + min_two->value,true,NULL); 
		huff_tree[insert_index]->left = min_one; 
		huff_tree[insert_index]->right = min_two; 		
		}
	
	}

	code_assignment(root);
	free(huff_tree);
	return root;	
}

/*Assigns code to each of the characters found in a file */
/*input: huffman tree */
/*output: code assignment */
void code_assignment(huff_node* tree){
	char_code* left = (char_code*) malloc(sizeof(char_code)); 
	char_code* right = (char_code*) malloc(sizeof(char_code));
	
	left->length = 1;
	right->length = 1;

	left->code[0] = left_path(left->code[0],0); 
	right->code[0] = right_path(right->code[0],0);
	code_assignment_recurs(tree->left,left);
	code_assignment_recurs(tree->right,right);
	free(left);
	free(right);	

}

/*helper function of code assignment */
/*input: huffman tree and code that can be assigned */
/*output: assigns code to a node with no children */
//Note: passed by value is used for code because it will be easier than constantly allocating 
//memory for a change. (stack space won't be affected too much)
void code_assignment_recurs(huff_node* root ,char_code* cd){

	if((root->left == NULL) && (root->right == NULL)){
		char_code_copy(root->an->cc,cd); 
		#ifdef DEBUG
			printf("character %c has: \n\n",root->an->ascii_char);
			code_print(root->an->cc);	
		#endif
	}else{	
		if(root->left != NULL){
			char_code* left = (char_code*) malloc(sizeof(char_code));
			char_code_copy(left,cd); 
			left->code[left->length/8] = left_path(left->code[left->length/8],left->length%8);
			left->length++;
			code_assignment_recurs(root->left, left);
			free(left);
			
		}

		if(root->right != NULL){
			char_code* right = (char_code*) malloc(sizeof(char_code));
			char_code_copy(right,cd); 
			right->code[right->length/8] = right_path(right->code[right->length/8],right->length%8);
			right->length++;
			code_assignment_recurs(root->right,right);
			free(right);
		}
	}

}

/* copies char_code of src to dest*/ 
void char_code_copy(char_code* dest, char_code* src){
	
	for(int i = 0 ; i < 32 ; i++){
		dest->code[i] = src->code[i];
	}
	
	dest->length = src->length; 
}

/* writes to compressed version of a file */
/*input: file to be compressed (src), compression destination (dest), ascii_node list that holds code*/
/*output: true if successful, false otherwise */
bool write_compressed_file(FILE* src,uint64_t src_length, FILE* dest,ascii_node** list,int list_len, huff_node* root){
	
	fwrite(&src_length, sizeof(uint64_t),1,dest);
	write_huffman_tree(dest,root);		

	char ch;	
	uint8_t index; 
	char data =0 ; 
	char_code* cc; 
	uint8_t char_left = 8; 
	for(int k = 0 ; k < src_length; k++){
		fread(&ch,sizeof(char),1,src);
		index = (uint8_t) ch; 	
		cc = list[index]->cc;
		uint16_t code_index = 0;	
		while(code_index < cc->length){
			int iter = (cc->length-code_index) < char_left ? (cc->length-code_index) : char_left;
			char temp;
			for(int i = 0; i < iter; i++){
				temp = (char) cc->code[code_index/8]; 
				temp = 0x01 & (temp >> (7-(code_index%8))); 
				data = data | (temp << (char_left-1)); 	
				code_index++; 
				char_left--; 
			}	

			if(char_left == 0){
				char_left = 8; 
				fwrite(&data,sizeof(char),1,dest);
				#ifdef DEBUG
					printf("%x\n",(uint8_t)data);
				#endif
				data = 0;
			}		
		}

	}

	if(char_left != 0 && char_left != 8){

		fwrite(&data,sizeof(char),1,dest);
	}
	return true;

}

void write_huffman_tree(FILE* file, huff_node* root){
	bool leaf = false;
	if(root->left == NULL && root->right == NULL){
		leaf = true;	
		fwrite(&leaf, sizeof(bool),1,file);
		fwrite(&root->an->ascii_char,sizeof(char),1,file);
	}else{
		fwrite(&leaf,sizeof(bool),1,file);	
		write_huffman_tree(file,root->left);
		write_huffman_tree(file,root->right);
	}

	free(root); // we will no longer need this 
}

/* debug functions */

/*prints the frequency of characters found in a file */
/*input: list of ascii nodes */
/*output: prints the character and it's frequency */  

void print_char_frequency(ascii_node** list, int length){
	printf("Frequency of ascii nodes:\n\n c - char   f - frequency\n\n\n");

	for(int i = 0 ; i < length ; i++){
		if(list[i] != NULL) { 
			printf("c: %c  f: %lu \n\n",list[i]->ascii_char,list[i]->freq);
		}
	}
}

void code_print(char_code* cc){
	
	printf("code length: %i\n\n path/code: ",cc->length);
	int code_left = cc->length;

	/*for(int i = 0; i < 8 ; i++){*/

		/*printf("%x ",cc->code[i]) ; */
	/*}*/
	for(int i = 0 ; i < (cc->length+7)/8; i++){
		int iter = code_left < 8 ? code_left : 8 ; 
		for(int k = 0 ; k < iter ; k++){
			if( ((cc->code[i] >> 7-k) & 0x01) == 1){
				printf("1");
			}else{
				printf("0");
			}
		}
		code_left - iter; 

	}

	printf("\n\n");
}
