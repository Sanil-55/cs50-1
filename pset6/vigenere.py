#!/usr/bin/env python3

import sys
def main():
    """
    solution to CS50 problem set 6
    a program that encrypts messages using Vigenère’s cipher
    encrypting messages using a sequence of keys
    """
    
    # insure proper command line input
    if len(sys.argv) != 2:
        print("Incorrect command-line argument")
    else:
        keyword = sys.argv[1]
        
    # keyword should be all letters   
    if keyword.isalpha() == False:
        print("keyword must be entirely alphabetical\n")
    else:
        s = input("plaintext: ")
        print("ciphertext: {}".format(encoder(s, keyword)))
 
   
def encoder(s, keyword):
    """
    @param s: string to be encrypted
    @param keyword: a sequence of keys
    @return: encrypted string
    """
    s = [char for char in s]
    n_k = len(keyword)
    n_s = len(s)
    
    # loop through every letter of message
    for i in range(n_s):
        
        #determine key for each character of s to be encrypted
        key = ord(keyword[i % n_k])
        if chr(key).isupper():
            key -= 65
        else:
            key -= 97
        
        # shift the character of s by key
        if s[i].isupper():
            s[i] = chr((ord(s[i]) - 65 + key) % 26 + 65)
        else:
            s[i] = chr((ord(s[i]) - 97 + key) % 26 + 97)
    
    return ''.join(s)
    
if __name__ == "__main__":
    main()