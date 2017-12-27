#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

int main (void)
{
    int values[] = {3,5,4,2,1};
    
    printf("Current Array: ");
    for(int i=0; i<5; i++)
    {
        printf("%i ,",values[i]);
    }
    printf("\n");
    int minIndex = 0;
    int currentmin = values[0];
    bool minFound = false;
    for(int i=0; i<5; i++)
    {
       //currentmin = values[i];
        for (int j=i+1; j<5;j++)
        {
            if (values[j] < currentmin )
            {
                // int temp;
                // temp = b;
                // b = a;
                // a = temp;
                currentmin = values[j];
                 minIndex = j;
                 //use a boolean min found to trigger events outside this loop
                 
                 //printf("\n Minimum is %i and its index is %i\n",currentmin,minIndex);
                 minFound = true;
            }
            else
            {
                //i++
            }
            
            
        }
        if (minFound)
        {
        printf("current minimum is %i",currentmin);
               
                int temp;
                temp = values[i];
                values[i] = currentmin; 
                values[minIndex] = temp;
                
                if(i!=5-1)
                {
                    currentmin = values[i+1];
                }
        }
        else
        {
            if(i!=5-1)
            {
                currentmin = values[i+1];
            }
        }
    }
    printf("\n");
    
    printf("New Array: ");
    for(int i=0; i<5; i++)
    {
        printf("%i ,",values[i]);
    }
    
    return 0;

}