/**
    solution to CS50 problem set 1 greedy
    Implement a greedy algorithm that calculates the minimum number of coins required to give a user change
*/

#include <cs50.h>
#include <math.h>
#include <stdio.h>

/**
    Return the minimum number of coins required to give a user change
    @param change the amount of change
    @return the minimum number of coins need to give the change
*/
int calculate_change(float change); 

int main(void)
{
    float change;
    
    // get user input of the amount of change
    do 
    {
        printf("O hai! How much change is owed?\n");
        change = get_float();
    }
    while (change < 0); 
    
    printf("%i\n", calculate_change(change));
    
}

// return the minimum number of coins need to give the change
int calculate_change(float change)
{

    int coins = 0;

    // total change amount by cents 
    change = (int)round(change*100);
    
    // check number of quaters to use 
    while (change >= 25)
    {
        coins++;
        change -= 25;
    } 
    
    // check number of dimes to use 
    while (change >= 10)
    {
        coins++;
        change -= 10;
    } 
    
    // check number of nickels to use 
    while (change >= 5)
    {
        coins++;
        change -= 5;
    } 
    
    // check number of pennies to use
    while (change >= 1)
    {
        coins++;
        change -= 1;
    } 
    
    return coins;
}