/** 
 * solution to CS50 problem set 5 speller
 * dictionary's functionality for the speller checker
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

#define LENGTH 45
#define HASHTABLE_SIZE 300000

// defind a node
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// defind a hashtable
node *hashtable[HASHTABLE_SIZE];

// defind the hash function
int hash(char *word)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash = (hash << 2) ^ word[i];
    }
    return hash % HASHTABLE_SIZE;
}

// define a global variable to show if dictionary is loaded
bool loaded = false;

// define a global variable to keep track of word counts in dictionary
unsigned int counter = 0;


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // create char array to store a copy of the original word  
    int n = strlen(word);
    char copy[n + 1];
    
    if (word == NULL)
    {
        return false;
    }
    
    // convert word to lower case and store it in copy
    for (int i = 0; i < n; i++)
    {
        copy[i] = tolower(word[i]);
    }
    copy[n] = '\0';

    // get the hash code of the copied word
    int h = hash(copy);
    
    // compare the word with the words with the same hash code in hash table 
    node *cursor = hashtable[h];
    while (cursor != NULL)
    {
        // compare cursor->word with the target word
        if (strcmp(cursor->word, copy) == 0)
        {
            return true;
        }
        else
        {
            // move cursor to the next in the linked list 
            cursor = cursor->next;
        }
    }
    
    return false;
}



/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // initiate hashtable elements as all NULL 
    for(int i = 0; i < HASHTABLE_SIZE; i++)
    {
        hashtable[i] = NULL;
    }
    
    // open the dictionary
    FILE* dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        fprintf(stderr, "Could not open dictionary.\n");
        return false;
    }
    
    // scan the dictionary
    while(true)
    {
        // malloc a new node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        
        // free new_node and stop when the end of dictionary is reached
        if (feof(dict))
        {
            free(new_node);
            break;
        }
        
        // read word and store it in new_node->word
        fscanf(dict, "%s", new_node->word);
        
        // get the hash code of the word
        int h = hash(new_node->word);
    
        // insert the new node to the beginning of the corresponding linked list in hash table
        new_node->next = hashtable[h];
        hashtable[h] = new_node;
        
        // increase the word counter by 1
        counter++;
    }
    
    // close the dictionary
    fclose(dict);
    
    loaded = true;
    return true;
}



/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // return the size of dictionary
    if (counter > 0)
    {
        return counter;
    }
    else
    {
        return 0;
    }
}



/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    if (loaded)
    {
        for (int i = 0; i < HASHTABLE_SIZE; i++)
        {
            node *cursor = hashtable[i];
            while (cursor != NULL)
            {
                node *tmp = cursor;
                cursor = cursor->next;
                free(tmp);
            }
        }
        
        loaded = false;
        return true;
    }
    else 
    {
        return false;
    }
}