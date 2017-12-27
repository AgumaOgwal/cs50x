#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Minutes: ");
    int minutes = get_int();
    
    //find number of bottles
    int bottles = minutes * 12;
    
    //print number of bottles
    printf("%i\n",bottles);
}