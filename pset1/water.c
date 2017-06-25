/**
	solution to CS50 problem set 1
	Implement a program that reports a user’s water usage 
	converting minutes spent in the shower to bottles of drinking water.
 */



#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // prompts user for shower minute input
    printf("Minutes: ");
    int minute = get_int();
    
    printf("Bottles: %i\n", 12*minute);
}