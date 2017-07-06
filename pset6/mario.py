#!/usr/bin/env python3

def main():
    """
    solution to CS50 problem set 6
    print out a double half-pyramid of a specified height (0, 23]
    """
    height = int(input("Height: "))
    if height <= 0 or height > 23:
        height = int(input("Height: "))
    for i in range(height):
        print((height-1-i)*' '+(i+1)*'#'+"  "+(i+1)*'#'+(height-1-i)*' ')
    
if __name__ == "__main__":
    main()