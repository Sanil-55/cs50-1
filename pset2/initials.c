/**
   solution to CS50 problem set 2 initials

   Implement a program that given a person’s name prints a person’s initials
   Able to take sloppy user input, i.e., one or more spaces at the start and/or 
   end of the user’s input or even multiple spaces in a row.
*/

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string name = get_string();
    
    // loop through all characters, print any letter (captilized) found after a space
    for (int i = 0, length = strlen(name); i < length; i++)
    {
        if ((i == 0 && isalpha(name[0])) ||
            (isalpha(name[i]) && isspace(name[i - 1])))
        {
            printf("%c", toupper(name[i]));
        }
    }
    
    printf("\n");
    
}
