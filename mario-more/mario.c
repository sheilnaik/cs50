#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;

    // Ask user for height. Keep asking until input is between 1 and 8.
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Create 'height' number of lines for the pyramid
    for (int y = 1; y <= height; y++)
    {

        // Create first blank spaces
        for (int x = 0; x < height - y; x++)
        {
            printf(" ");
        }

        // Create first hash signs
        for (int x = 0; x < y; x++)
        {
            printf("#");
        }

        // Leave two spaces in between
        printf("  ");

        // Create second hash marks
        for (int x = 0; x < y; x++)
        {
            printf("#");
        }

        // Go to next line
        printf("\n");
    }

}
