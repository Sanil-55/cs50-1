/**
 * helpers.c
 *
 * Helpers functions for CS50 problem set 3 find
 * 
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // binary search
    if (n >= 1)
    {
        int mid = (n - 1)/2 + 1;
        if (value == values[mid])
        {
            return true;
        } 
        else if (value < values[mid])
        {
            return search(value, values, mid);
        }
        else
        {
            return search(value, values + mid + 1, n - mid - 1);
        }
    }
    else 
    {
        return false;
    }
    
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // bubble sort
    for (int i = 0; i < n - 2; i++)
    {
        for (int j = 0; j < n - 2 - i; j++)
        {
        	// swap the elements if the pair is out of order
            if (values[j] > values[j+1])
            {
                int temp = values[j];
                values[j] = values[j+1];
                values[j+1] = temp;
            }
        }
    }
}

