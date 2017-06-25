/**
	solution to CS50 problem set 1 mario
	Implement a program that prints out a half-pyramid of a specified height
 */


#include <cs50.h>
#include <stdio.h>

/**
    print a symbol n times
    @param n number of times to print
    @param the symbol
*/
void print_symbol(int n, char symbol); 

int main(void)
{
	int height;

	// get the user input of pyramid height
	do
	{
		printf("Height: ");
		height = get_int();
	}
	while (height < 0 || height > 23);
    
    // a loop to print different layers of the pyramid
    for (int i = 0; i < height; i++)
    {
        print_symbol(height - 1 - i, ' ');
        print_symbol(i + 2, '#');
        printf("\n");
    }
    
}

// print a symbol n times 
void print_symbol(int n, char symbol)
{
    for (int i = 0; i < n; i++)
    {
        printf("%c", symbol);
    }
}