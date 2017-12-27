#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <ctype.h>

int main(void)
{
    printf("O hai! How much change is owed\n");
    float change;
    int coins =0;
    do
    {
        change = get_float();
    }while (change <= 0 && isdigit(change)==true);
    
    int cents = (change+0.005) * 100;
    printf("change is %f, cents %i\n",change,cents);
    
    int quarters = 0;
    int dimes = 0;
    // int nickels =0;
    // int pennies =1;
    while (cents >=25)
    {
        quarters = cents/25;
        coins = coins + quarters;
        cents = cents % 25;
        printf("coins: %i, cents: %i\n",coins,cents);
    }
    while (cents >=10)
    {
        dimes = cents/10;
        coins = coins + dimes;
        cents = cents % 10;
        printf("coins: %i, cents: %i\n",coins,cents);
    }
    while (cents >=5)
    {
        dimes = cents/5;
        coins = coins + dimes;
        cents = cents % 5;
        printf("coins: %i, cents: %i\n",coins,cents);
    }
    while (cents >=1)
    {
        dimes = cents/1;
        coins = coins + dimes;
        cents = cents % 1;
        printf("coins: %i, cents: %i\n",coins,cents);
    }
    
    printf("%i\n",coins);
}