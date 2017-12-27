/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

#include <stdio.h>

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    if(n<=0)
    {
        return false;
    }
    else{
       
        for (int i=0; i<n; i++)
        {
            if(value==values[i])
            {
                return true;
            }
        }
        return false;
    }
    
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    
    //output unsorted list for debugging
    // printf("\nUnsorted List: ");
    // for(int i=0; i<n; i++)
    // {
    //     printf("%i ,",values[i]);
    // }
    // printf("\n");
    
    //selection sort
    int minIndex = 0;
    int currentmin = values[0];
    
     //use a boolean minFound to trigger events outside this loop
    bool minFound = false;
    for(int i=0; i<n; i++)
    {
       //currentmin = values[i];
        for (int j=i+1; j<n;j++)
        {
            if (values[j] < currentmin )
            {
                currentmin = values[j];
                 minIndex = j;
                
                 //use a boolean min found to trigger events outside this loop
                 minFound = true;
            }
            
        }
        if (minFound)
        {
            //perform swap of the two values. Previous smaller value and the newly found smaller value. because a smaller value was found
                int temp;
                temp = values[i];
                values[i] = currentmin; 
                values[minIndex] = temp;
                
                //to ensure we dont get an index out of bounds error.
                if(i!=n-1)
                {
                    //make the new minimum value the next value in the array. because we have already filled the ith position.
                    currentmin = values[i+1];
                }
        }
        else
        {
            //if no smaaller value is found. we still need to move on to the next value in the array because this ith value was already sorted
            if(i!=5-1)
            {
                currentmin = values[i+1];
            }
        }
        
    }
    
    // printf("\nSorted List: ");
    // for(int i=0; i<n; i++)
    // {
    //     printf("%i ,",values[i]);
    // }
    // printf("\n");
    return;
}
