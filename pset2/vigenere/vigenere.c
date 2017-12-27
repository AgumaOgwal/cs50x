#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./vigenere k\n");
        exit(1);
    }
    
    string cipher = argv[1];
    for(int i=0; i<strlen(cipher); i++)
    {
        if (!isalpha(cipher[i]))
        {
            printf("Usage: ./vigenere k\n");
            exit(1);
        }
    }
    printf("plaintext: ");
    
    //get the plaintext from input
    string text = get_string();
    
    int j=0;
    char key =' ';
    char cipheredchar = ' ';
    
    //loop through the plain text one character at a time
    printf("ciphertext: ");
    for(int i=0; i<strlen(text); i++)
    {
        //for each character in the plaintext...look for the appropriate keying mechanism. so we also loop through the  key string..
        //not automatically incrementing though. only incrementing when we do use a key
        
        while(j<=strlen(cipher)-1)
        {
            // printf("\n\nj start of loop is now %i\n",j);
            char letter = text[i];
            key = cipher[j];
            // printf("key is now is now %c\n",key);
                //each key has the same value whether lowercase or uppercase. so isolate them in their respective cases so as to map ASCII wise
                //if the key is upper..we subtract thee value of ascii A so that A would become 0. B..1 and etc
                if(isupper(key))
                {
                    key = key - 65;
                }
                //the same logic for uppercase goes for lowercase..because lowercase a is 97
                else if(islower(key))
                {
                    key = key - 97;
                }
            //now that we have our key figured out
            //we go to the actual plain text to encipher
            //and because of ASCII values, we separate them on a case by case (lower and upper)
            //and we can only encipher alphabetic characters. so this first check is to ensure its an alpha 
            if(isalpha(letter))
            {
                //if our letter is upper case
                if(isupper(letter))
                {
                    //deduct 65 from it because uppercase A has an ASCII value of 65.
                    letter = letter - 65;
                    
                    //add the key to the letter.
                    //do modulo 26 so that the cipher is always within the 26 characters of the alphabet
                    cipheredchar = (letter+key)% 26;
                    //print it but add it back to 65 so that we begin operating within our uppercase characters
                    printf("%c",cipheredchar+65);
                    
                    //loop back to the first character of the key (j) incase we are at the end of the key now
                    if(j==strlen(cipher)-1)
                    {
                        j=0;
                        //exit from this while loop to continue with the enciphering. since we have enciphered this text and output it already
                        break;
                    }
                    //because the key has been used, we can now iterate over to the next key in the key string
                    j++;
                    //exit from this while loop to continue with the enciphering. since we have enciphered this text and output it already
                    break;
                }
                
                //if our letter is lower case
                else if(islower(letter))
                {
                    //deduct 65 from it because uppercase A has an ASCII value of 65.
                    letter = letter - 97;
                    
                    //add the key to the letter.
                    //do modulo 26 so that the cipher is always within the 26 characters of the alphabet
                    cipheredchar = (letter+key)% 26;
                    //print it but add it back to 65 so that we begin operating within our uppercase characters
                    printf("%c",cipheredchar+97);
                    
                    if(j==strlen(cipher)-1)
                    {
                        
                        j=0;
                        //exit from this while loop to continue with the enciphering. since we have enciphered this text and output it already
                        break;
                    }
                    //because the key has been used, we can now iterate over to the next key in the key string
                    j++;
                    break;
                }
            }
            else
            {
             printf("%c",letter);
            //  if(j==strlen(cipher)-1)
            // {
            //     printf("j is now %i\n",j);
            //     //j=0;
                
            // }
             break;
            }
            printf("j is currently %i",j);
            
        }
    }
    printf("\n");
    
    return 0;
}