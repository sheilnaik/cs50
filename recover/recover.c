#include <stdio.h>
#include <stdlib.h>

/*
    1. Open memory card +
    2. Look for the beginning of a JPEG +
    3. Create a new JPEG file
    4. Keep writing 512 bytes at a time until a new JPEG is found.
*/

int main(int argc, char *argv[])
{
    // Error check for bad or no arguments
    if (argc != 2)
    {
        printf("Usage: recover.c [Name of file]...\n");
        return 1;
    }

    // Open file
    FILE *file = fopen(argv[1], "r");
    FILE *out = NULL;

    if (file == NULL)
    {
        return 1;
    }

    unsigned char buffer[512];
    int file_counter = 0;
    char *filename = malloc(sizeof(char) * 8);

    // Get to the first image
    do
    {
        fread(buffer, 512, 1, file);
    }
    while ((buffer[0] != 0xff) && (buffer[1] != 0xd8) && (buffer[2] != 0xff) && ((buffer[3] & 0xf0) != 0xe0));

    while (feof(file) == 0)
    {
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {
            printf("You're at the start of a JPEG!\n");

            if (file_counter > 1)
            {
                // Close previous file
                sprintf(filename, "%03i.jpg", file_counter - 1);
                printf("Closing %s...\n", filename);
                fclose(out);
            }

            // Open new file
            sprintf(filename, "%03i.jpg", file_counter);
            printf("Opening %s...\n", filename);
            out = fopen(filename, "w");

            file_counter++;
        }

        printf("Writing...\n");
        fwrite(buffer, 512, 1, out);

        printf("Reading...\n");
        fread(buffer, 512, 1, file);
    };

    fclose(file);
}
