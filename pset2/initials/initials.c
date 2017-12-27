#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
    //get the name as input
    string name = get_string();
    
    //convert first character to upper case and print
    printf("%c",toupper(name[0]));
    
    //loop through and extract every letter that comes after a space and print in upper case
    for(int i=0; i<strlen(name); i++)
    {
        if(name[i]==32)
        {
            printf("%c",toupper(name[i+1]));
        }
    }
    printf("\n");
}