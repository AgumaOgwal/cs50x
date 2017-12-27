#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main (int argc, string argv[])
{
    if (argc != 2 )
    {
        printf("Usage: ./caesar k\n");
        exit(1);
    }
    string text ="";
    printf("plaintext: ");
     text = get_string();
    
    int key = atoi(argv[1]);

    printf("ciphertext: ");
    for (int i=0; i < strlen(text); i++)
    {
        //store the ith value of text as a new variable 'letter'
        char letter = text[i];
        //create a variable for the ciphertext to be outputted
        char cipher ;
        
        //if our letter is upper case
        if(isupper(letter)){
            //deduct 65 from it because uppercase A has an ASCII value of 65.
            letter = letter - 65;
            
            //add the key to the letter.
            //do modulo 26 so that the cipher is always within the 26 characters of the alphabet
            cipher = (letter+key)% 26;
            //print it but add it back to 65 so that we begin operating within our uppercase characters
            printf("%c",cipher+65);
        }
        
        //same comments from uppercase above apply here. only difference is that lowercase a's ascii value is 97
        else if(islower(letter)){
            letter = letter - 97;
            cipher = (letter+key)% 26;
            printf("%c",cipher+97);
        }
        else
        {
            printf("%c",letter);
        }
        //printf("%c",(text[i]+key));
    }
    printf("\n");
    
    return 0;
}