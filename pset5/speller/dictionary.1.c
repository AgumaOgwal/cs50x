/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include <ctype.h>

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
 
 typedef struct NODE
 {
     //char dictword[LENGTH-1];
     char *dictword;
     struct NODE *next;
 }NODE;

 NODE *listPointer = NULL;
 int numberofwords = 0;
 
bool check(const char *word)
{
    // TODO
    NODE *cursor = listPointer;
    char tempword[(int)strlen(word)];
    //printf("word is of length %i\n",(int)strlen(word));
    while(cursor != NULL)
    {
        //logic here
        
        //printf("Word at %p is %s\n", cursor, cursor->dictword);
        //printf("Comparing %s to %s", word, cursor->dictword);
        // if(strcmp(word, cursor->dictword)==0)
        // {
        //     //printf(" | The two words are equal\n");
        //     //word exists in the dictionary so we can return true
        //     return true;
        // }
        
        for(int i=0; i<((int)strlen(word)+1); i++)
        {
            if(i==(int)strlen(word))
            {
                tempword[i] = '\0';
                break;
            }
            tempword[i] = word[i];
            tempword[i] = tolower(tempword[i]);
        }
        int compare = strcmp(tempword, cursor->dictword);
         if(compare==0)
         {
             //printf("%s == %s\n",tempword, cursor->dictword);
             return true;
         }
         else
         {
              //printf("%s != %s\n",tempword, cursor->dictword);
         }
        //Need to implement a check 
        cursor = cursor->next;
        
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    /**
     * Was getting a segmentation fault when running large dictionary
     * turns out it was because I was mallocing with size of (node)..which is a NODE variable
     * instead of size of NODE
     * so consequently, during execution, a word would be reached that couldnt be stored in the size of a previous
     * variable
     */
     
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
    bool firstSet = false;
    
    //initialised the below variables to null to silence errors at compilation
    
    NODE *tempNext = NULL;
    
    //using the variable currentnode to help set node->next to null when we have reached the end of file
    NODE *currentnode = NULL;
    //newnode->word =
    
    //the below statement reads character by character from the input ( fgetc(fp) )
    //I simply need to wait for the end of string terminator to know we have formed a full word
    //and then i kick off the logic to create a NODE with a word and pointer to a next word
   for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        
        if(c=='\n')
        {
            //printf("Current word is ");
            for(int j=0; j<index; j++)
            {
                //printf("%c",word[j]);
            }
            //printf("\n");
            
            //if the first node has been set, we take this route
            //in this route, instead of mallocing new memory space, we use memory that has already been allocated as a
            //next pointer from the first/previous pointer
            if(firstSet==true)
            {
                //tempNext = malloc(sizeof(node));
                //tempNext= node; 
                
                //printf("firstSet is true\n");
                //printf("Word is now %s\n",word);
                NODE *node = tempNext;
                //used the below loop. because:
                //1. node->dictword = word; directly assigning char * arrays was throwing an error
                //2. the other option would have been strcpy(), but I was getting over lapping variables
                char temporary[index];
            for(int k=0; k<index; k++)
            {
                temporary[k] = word[k];
                //node->dictword[k] = word[k];
            }
            node->dictword = temporary;
                //node->dictword = word;
                //strcpy(node->dictword, word);
                //printf("Node Address is %p\n",node);
                
                //printf("node->dictword is %s",node->dictword);
                node->next = NULL;
                //printf(" | Okay..1");
                tempNext = malloc(sizeof(NODE));
                //printf(" | Okay..2");
                node->next =tempNext;
                //printf(" | Okay..3\n");
                //printf("Next Address is %p\n",tempNext);
            
                currentnode = node;
            }
            else{
            //NODE *node = malloc(sizeof(node));
            NODE *node = malloc(sizeof(NODE));
            
            //used the below loop. because:
            //1. node->dictword = word; directly assigning char * arrays was throwing an error
            //2. the other option would have been strcpy(), but I was getting over lapping variables
            char temporary[index];
            for(int k=0; k<index; k++)
            {
                temporary[k] = word[k];
                //node->dictword[k] = word[k];
            }
            node->dictword = temporary;
            //strcpy(node->dictword, word);
            //printf("Node Address is %p\n",node);
            
            //printf("node->dictword is %s\n",node->dictword);
            node->next = NULL;
            tempNext = malloc(sizeof(NODE));
            node->next =tempNext;
            //printf("Next Address is %p\n",tempNext);
            
             listPointer = node;
              //printf("First node->dictword is %s\n",listPointer->dictword);
                firstSet = true;
                //printf("List Pointer Address is %p\n",listPointer);
                
                currentnode = node;
            }
            //tempNext = node;
            
            //tempNext = 
            //set the first pointer to the first 
            // if(firstSet==false)
            // {
            //     listPointer = node;
            //     firstSet = true;
            //     printf("List Pointer Address is %p\n",listPointer);
            // }
            
            
            for(int j=0; j<LENGTH; j++)
            {
                word[index] = '\0';
            }
            index = 0;
            
            numberofwords++;
        }
        else
        {
        word[index] = c;
        index ++;
        }
        
        //printf("C is %c \n",c);
    }
    //free(node);
    //free(listPointer);
    free(tempNext);
    //the below doesnt work in helping set the last pointer to null.
    //so I'll loop through the pointers and when I find currentnode, I set its next variable to null.
    //strcpy(currentnode->dictword,"Okayboss");
    //update: The statement below worked afterall
    currentnode->next = NULL;
    //printf("Current node word next is %s \n",currentnode->dictword);
    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return numberofwords;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    //time to start unloading
    NODE *cursor = listPointer;
    
    while (cursor != NULL)
    {
        NODE *temp = cursor;
        cursor = cursor->next;
        free(temp);
    }
    return true;
}

