/**
 * solution to CS50 problem set 4 recover
 * recovers JPEG images from a raw file
 */

#include <stdio.h>
#include <stdint.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // ensure proper usage usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover file\n");
        return 1;
    }
    
    // open raw file
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        fprintf(stderr, "Could not open file\n");
        return 2;
    }
    
    FILE *outfile = NULL;   // outfile to write to
    int counter = 0;   // keep track of the number of jpg files
	uint8_t buffer[BLOCK_SIZE];   // create a 512 bytes buffer
	
    // when not at the end of the raw file
    while (fread(buffer, 1, BLOCK_SIZE, raw_file))
    {
        // if the start of a new jpg file is found
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff 
        && ((buffer[3] & 0xf0) == 0xe0))
        {
            // close existing outfile, if any
            if (outfile != NULL)
            {
                fclose(outfile);
            }
            
            // start writing to a new jpg outfile
            char filename[8];
            sprintf(filename, "%03i.jpg", counter);
            outfile = fopen(filename, "w");
            fwrite(buffer, BLOCK_SIZE, 1, outfile);
            counter++;
        }
        else
        {
            // if new jpg file not found and a current outfile is open, keep writing to this file
            if (outfile != NULL)
            {
                fwrite(buffer, BLOCK_SIZE, 1, outfile);
            }
        }
    }
    
    // close the last jpg file
    if (outfile != NULL)
    {
        fclose(outfile);
    }
    
    fclose(raw_file);
    return 0;
}


