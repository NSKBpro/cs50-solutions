/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

//Globals
node *root;
int totalWords;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int wordLength = strlen(word);
    node *tmp = root; //Local node pointer to root we will work with in this function
    
    for(int i = 0;i<wordLength;++i){
        char c = tolower(word[i]); //We are using tolower because our function has to be case-insensitive
        
        if(c == '\'')
                c=123; /* If characer is aphostrophe ,it will go on end of children array ( [26] )
                        , as we are converting everything to lower case to be easier to work with, 123-96 == 26 */
        
        node *nextNode = tmp->children[c-'a']; //Go to corresponding element in children node of tmp(root) , ascii of c - 'a' (97) will give alphabetic index                
        if(nextNode == NULL) //Word is misspelled
            return false; 
        else
            tmp = nextNode; // Character is correct, move on
    }
    
    if(tmp->isWord) //Check if word is ended, if its true that means word is spelled correctly
        return true;
        
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE *fp = fopen(dictionary,"r"); //Try to open dictionary
    
    //Error handling for file
    if(fp == NULL){
        fprintf(stderr,"ERROR Opening file.");
        return false;
    }
    
    root = calloc(1, sizeof(node)); //Try to allocate memory for root node
    
    //Error handling for root node memory allocation
    if(root == NULL){
        fprintf(stderr,"ERROR allocating dictionary memory.");
        return false;
    }
    
    node *tmp = root; //Local node pointer to root we will work with in this function
    
    for(char c = tolower(fgetc(fp));c != EOF;c = tolower(fgetc(fp))){ //Iterate through fp until EOF is reached (character by character)
    
        if(c != '\n'){ //True if its not end of word
            if(c == '\'')
                c=123; /* If characer is aphostrophe ,it will go on end of children array ( [26] )
                        , as we are converting everything to lower case to be easier to work with, 123-96 == 26 */
                        
            node* nextNode =tmp->children[c-97]; //Try to get address of next child
            
            if(nextNode == NULL){ //True if there is no child 
                tmp->children[c-97] = calloc(1,sizeof(node)); //Allocate memory for child
                tmp = tmp->children[c-97]; //Set tmp pointer to new child address
            }else
                tmp = nextNode; // Child already exists, set tmp pointer to its address   
                
        } else { // If its end of word ('\n')
            if(tmp != root){ // Check if word has atleast one letter
                tmp->isWord = true; // Set bool for end of word
                tmp = root; // Reset our tmp pointer to begging of our trie data structure
                ++totalWords; // Increment number of words in the dictionary
            }
        }
    }
    fclose(fp); // Close dictionary file so other processes can use it 
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return (root)?totalWords:0;
}

/**
 * Recursive function that frees a trie from memory
 */
void helperUnload(node* ptr)
{
    for (int i = 0; i < 27; ++i) // Iterate through all children nodes
        if (ptr->children[i] != NULL) // True if current child is not freed (NULL)
            helperUnload(ptr->children[i]); // Call this function again(recursively) with this non NULL child as argument
 
    free(ptr); // Frees up memory previously allocated in heap
}
 
/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    helperUnload(root);
    return true;
}