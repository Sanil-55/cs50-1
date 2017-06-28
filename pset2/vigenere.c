/**
   solution to CS50 problem set 2 vigenere
   Implement a program that encrypts messages using Vigenère’s cipher
   messages are encrypted using a sequence of keys, i.e., a keyword 
   each letter in the message is shifted by corresponding key in the keyword
  */

#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**
   printed encoded string by keyword
   @param s input string
   @param keyword a string composed entirely of alphabetical characters
  */
int encoder(string s, string keyword);

int main(int argc, char *argv[])
{
	// check if number of command-line arguments are valid
    if (argc != 2)
    {
        printf("incorrect command-line arguments\n");
        return 1;
    }
    
    // check if the keyword is valid
    for (int i = 0, n_k = strlen(argv[1]); i < n_k; i++)
    {   
        if (!isalpha(argv[1][i]))
        {
            printf("keyword must be entirely alphabetical\n");
            return 1;
        }
    } 
    
    // get input of the original message 
    printf("plaintext: ");
    string s = get_string();
    
    // print the encrypted message
    printf("ciphertext: ");
    encoder(s, argv[1]);
    
    printf("\n");
}

// function to print out encoded string given the keyword and the rules
int encoder(string s, string keyword)
{
	// loop through every character of the string s
    for (int i = 0, j = 0, n_s = strlen(s), n_k = strlen(keyword); i < n_s; i++)
    {
    	// determine key for each character of string s to be encrypted
        char key = keyword[j % n_k];
        if (isupper(key))
        {
            key -= 65;
        }
        else if (islower(key))
        {
            key -= 97;
        }
        
        // shift the character of string s by the key
        if (isupper(s[i]))  
        {
            printf("%c", (s[i] - 65 + key) % 26 + 65);
            j++;
        }
        else if (islower(s[i]))
        {
            printf("%c", (s[i] - 97 + key) % 26 + 97);
            j++;
        }
        else 
        {
            printf("%c", s[i]);
        }
    }
    
    return 0;
}

