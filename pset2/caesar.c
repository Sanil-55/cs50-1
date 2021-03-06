/**
   solution to CS50 problem set 2 caesar

   Implement a program that encrypts messages using Caesar’s cipher
   each letter is shifted by some number of places
*/

#include <cs50.h>
#include <stdio.h>
#include <string.h>

/**
   print out encrypted message by shifting each letter of the message by k number of places 
   @param s user input string
   @param k the key of the cipher, i.e. number of places to shift
*/
int cipher(string s, int k); 

int main(int argc, char *argv[]) 
{
	// check if correct command-line arguments are received
	if (argc != 2)
	{
		printf("incorrect command-line argument\n");
		return 1;
	}

	printf("plaintext: ");
	string s = get_string();
	// deal with cases when k is greater than 26 by getting the remainder
	int k = atoi(argv[1]) % 26;
	
	printf("ciphertext: ");
	cipher(s, k);
	printf("\n");
}

// function to print out encrypted message
int decipher(string s, int k)
{
	// loop through each character of the input message
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (islower(s[i]))
	    {
	        printf("%c", (s[i] -97 + k) % 26 + 97);
	    }
	    else if (isupper(s[i]))
	    {
	        printf("%c", (s[i] - 65 + k) % 26 + 65);
	    }
	    else
	    {
	        printf("%c", s[i]);
	    }
    }
    return 0;
}
