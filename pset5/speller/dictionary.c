/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

/**
 * Returns true if word is in dictionary else false.
 */
 
 /** 
 * Struct for Our Linked List
 * 
 * Humphrey Ogwal
 * 
 */
 #
 typedef struct node
 {
     char word[LENGTH-1];
     struct node *next;
 }node;

bool check(const char *word)
{
    // TODO
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO
    //first attempt: linked list
    
    //1. first open file
    FILE *fp= fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Could not open %s.\n at all", dictionary);
        //unload();
        return 1;
    }
    else
    {
        //printf("Opened successfully %s.\n", dictionary);
    }
    
    char word[LENGTH];
    int index = 0;
    boolean firstSet = false;
    node *listPointer ;
    node *tempNext ;
    node *node ;
    //newnode->word =
   for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        //printf("%c",c);
        if(c=='\n')
        {
            //printf("Current word is ");
            for(int j=0; j<index; j++)
            {
                printf("%c",word[j]);
            }
            printf("\n");
            if(firstSet==true)
            {
                tempNext = malloc(sizeof(node));
                tempNext=&node; 
            }
            node *node = malloc(sizeof(node));
            node->word = word;
            node->next = NULL;
            
            tempNext = node;
            
            //tempNext = 
            //set the first pointer to the first 
            if(firstSet==false)
            {
                listPointer = &node;
                firstSet = true;
            }
            
            
            for(int j=0; j<LENGTH; j++)
            {
                word[index] = '\0';
            }
            index = 0;
        }
        else
        {
        word[index] = c;
        index ++;
        }
    }
    free(node);
    free(listPointer);
    free(tempNext);
    fclose(fp);
    return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    return false;
}

