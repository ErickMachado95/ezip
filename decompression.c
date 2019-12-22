#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "decompression.h"


bool decompress(FILE* comp_file, FILE* decomp_file){



	uint32_t file_size; 
	fread(&file_size,sizeof(uint32_t),1,comp_file);

	dec_node* root = make_tree(comp_file); 		

	if(root == NULL){
		return false;
	}	


	write_to_decomp_file(file_size,comp_file,decomp_file,root);
	free_dec_node_tree(root);

}



/*This function rebuilds the huff tree found in teh meta data of the file */
/*input: compressed file */
/*output: constructed  tree  */

dec_node* make_tree(FILE* f){

	
	bool b; 

	fread(&b,sizeof(bool),1,f);

	dec_node* dn = (dec_node*) malloc(sizeof(dec_node));


	if(b){

		char c;  
		fread(&c,sizeof(char),1,f);			
		dn->leaf = true;
		dn->left = NULL;
		dn->right = NULL;
		dn->c = c; 

	}else{
		dn->leaf = false;
		dn->left = make_tree(f);
		dn->right = make_tree(f);
	}


	return dn;	

}

/*recursively free's allocated data for dec_node tree */
/*input: root of tree */
/*output: none */

void free_dec_node_tree(dec_node* root){ 

	if(root->leaf){
		free(root);
	}else{
		free_dec_node_tree(root->left);
		free_dec_node_tree(root->right);
		free(root);
	}		
}


/*function that will write the proper data to the decompressed file */
/*input: file size, the compressed file (with meta data already taking care of ),the decompressed file to write to, and the huffman tree to follow */
/*output: none */
void write_to_decomp_file(uint32_t file_size, FILE* comp_file, FILE* decomp_file,dec_node* root){
	
	char char_left = 8; 
	char c; 
	fread(&c,sizeof(char),1,comp_file);	
	dec_node* current = root; 
	
	for(int i  = 0 ;  i < file_size ; i++){		
		
		while(!current->leaf){
			if(char_left == 0){
				fread(&c,sizeof(char),1,comp_file);	
				char_left = 8; 
			}

			if(((c >> (char_left-1)) & 0x01)){

				current = current->right;

			}else{
				current = current->left;
			}	

			char_left--; 

		}

		fwrite(&current->c,sizeof(char),1,decomp_file);	
		current = root;
		
	}

}



