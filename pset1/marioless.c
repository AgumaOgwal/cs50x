#include <stdio.h>
#include <cs50.h>

int main(void)
{
   
    int height=0;
    do{
    printf("Height: ");
      height = get_int();
      
      if(height == 0){
          exit(0);
      }
    } while(height>23 || height <=0);
    
    int spaces = height - 1;
    
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<spaces; j++)
        {
            printf(" ");
        }
        
        for(int k=spaces; k<height; k++)
        {
            printf("#");
        }
        
        printf("#\n");
        
        spaces--;
    }
}