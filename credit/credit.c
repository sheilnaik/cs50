#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long number;
    string card_type = "INVALID";
    bool checksum_check = false;
    int card_length = 0, divisor = 10;

    number = get_long("Number: ");

    // Find length of card number
    long n = number;
    do
    {
        n /= 10;
        card_length += 1;
        // printf("Number: %ld\n", n);
        // printf("Count: %i\n\n", card_length);
    }
    while (n > 0);

    // printf("Card length is %d\n", card_length);

    // Find divisor
    for (int d = 0; d < card_length - 2; d++)
    {
        divisor *= 10;
    }

    // Check for possible Visa 13-digit
    if (number / 1000000000000 >= 1 && number / 1000000000000 <= 9)
    {
        if (number / 1000000000000 == 4)
        {
            card_type = "VISA";
        }
    }

    // Check for possible AmEx, 15-digit
    if (number / 10000000000000 >= 10 && number / 10000000000000 <= 99)
    {
        if (number / 10000000000000 == 34 || number / 10000000000000 == 37)
        {
            card_type = "AMEX";
        }
    }

    // Check for possible MasterCard and Visa 16-digit
    if (number / 100000000000000 >= 10 && number / 100000000000000 <= 99)
    {
        // MasterCard
        if (number / 100000000000000 >= 51 && number / 100000000000000 <= 55)
        {
            card_type = "MASTERCARD";
        }

        // Visa 16-digit
        if (number / 1000000000000000 == 4)
        {
            card_type = "VISA";
        }
    }



    long counter_1 = 10, counter_2 = 1;
    int total_checksum_1 = 0, total_checksum_2 = 0;

    // Checksum Rule #1
    //printf("Checksum Rule 1:\n");
    for (int i = 0; i < (card_length / 2); i++)
    {
        //printf("%ld\n", (number / counter_1 % 10) * 2);
        int x = (number / counter_1 % 10) * 2;
        total_checksum_1 += (x / 10) + (x % 10);
        counter_1 *= 100;
    }
    //printf("Total checksum #1: %i\n", total_checksum_1);



    // Checksum Rule #2


    // Adjust for odd card lengths. Checksum Rule #2 will have to be checked for one more digit
    int checksum_rule_2_length = card_length / 2;
    if (card_length % 2 == 1)
    {
        checksum_rule_2_length = (card_length / 2) + 1;
    }

    //printf("Checksum Rule 2:\n");

    for (int i = 0; i < checksum_rule_2_length; i++)
    {
        //printf("%ld\n", (number / counter_2 % 10));
        int x = (number / counter_2 % 10);
        total_checksum_2 += (x / 10) + (x % 10);
        counter_2 *= 100;
    }
    //printf("Total checksum #2: %i\n", total_checksum_2);

    if ((total_checksum_1 + total_checksum_2) % 10 == 0)
    {
        checksum_check = true;
    }

    if (checksum_check == true)
    {
        printf("%s\n", card_type);
    }
    else
    {
        printf("INVALID\n");
    }

}

// VISA 1 1234567890123
// VISA 2 1234567890123456
// AMEX 123456789012345
// MASTERCARD