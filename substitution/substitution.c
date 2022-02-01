#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>


int main(int argc, string argv[])
{
    // Error check for bad arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    const string key = argv[1];

    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Check for invalid characters in key
    for (int i = 0; i < strlen(key); i++)
    {
        if (toupper(key[i]) < 'A' || toupper(key[i]) > 'Z')
        {
            printf("Your key has invalid characters.\n");
            return 1;
        }
    }

    // Check for duplicate characters in key
    int duplicates[127] = { 0 };

    for (int i = 0; i < strlen(key); i++)
    {
        if (duplicates[(int) key[i]] == 1)
        {
            printf("Your key has duplicate characters.\n");
            return 1;
        }
        else
        {
            duplicates[(int) key[i]] = 1;
        }
    }

    int ascii_num;

    // Get plain text input from user
    string input = get_string("plaintext: ");
    char output[strlen(input)];

    /*
        * Loop through each letter input
        * Determine if upper or lower-case
        * If upper, find integer value and subtract 65 to find index position of letter in key
        * If lower, find integer value and subtract 97 to find index position of letter in key

    */
    for (int l = 0; l < strlen(input); l++)
    {
        // Find integer value of letter for upper- and lowercase letters. If not a letter, set the output index as the character.
        ascii_num = (int) input[l];

        // Follow rules listed above
        if (input[l] >= 'A' && input[l] <= 'Z')
        {
            output[l] = toupper(key[ascii_num - 65]);

        }
        else if (input[l] >= 'a' && input[l] <= 'z')
        {
            output[l] = tolower(key[ascii_num - 97]);
        }
        else
        {
            output[l] = input[l];
        }
    }


    printf("ciphertext: ");
    for (int i = 0; i < strlen(input); i++)
    {
        printf("%c", output[i]);
    }
    printf("\n");

    return 0;
}
