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
    string s = get_string();
    
    if (s != NULL)
    {
        // print the first letter (captalized) if no leading space
        if (s[0] != ' ')
        {
            printf("%c", toupper(s[0]));
        }
        
        // loop through the string and print the first letter after a space
        for (int i = 0, n = strlen(s); i < n; i++)
        {
            if (s[i] == ' ' && s[i+1] != ' ') 
            {
                printf("%c", toupper(s[i+1]));
            }
        }
    }
    
    printf("\n");    
    
}