#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // Accept one command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }
    // Check if file exists
    FILE *raw_file = fopen(argv[1], "r");

    if (raw_file == NULL)
    {
        printf("file does not exist\n");
        return 1;
    }
    typedef uint8_t BYTE;
    BYTE buffer[BLOCK_SIZE];
    FILE *image;
    char filename[8];
    int count_image = 0;
    // Open memory card
    // Repeat until end of the card:
    // Read 512 bytes in(to a buffer
    while (fread(buffer, BLOCK_SIZE, 1, raw_file))
    {
        // If start of new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (image != NULL)
            {
                fclose(image);
            }
            sprintf(filename, "%03i.jpg", count_image++);
            image = fopen(filename, "w");
        }
        if (image != NULL)
        {
            fwrite(buffer, BLOCK_SIZE, 1, image);
        }
    }
    fclose(image);
    fclose(raw_file);
    return 0;
}
