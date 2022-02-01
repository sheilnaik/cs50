#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

int number_of_letters(string text);
int number_of_words(string text);
int number_of_sentences(string text);
int coleman_liau(string text);

int main(void)
{
    string text;

    text = get_string("Text: ");

    int index = coleman_liau(text);

    /*
    printf("%s\n", text);
    printf("%i letter(s)\n", number_of_letters(text));
    printf("%i word(s)\n", number_of_words(text));
    printf("%i sentence(s)\n", number_of_sentences(text));
    */

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}


int number_of_letters(string text)
{
    /*
        Counts the number of letters in the supplied text.

        * Loop through each character in text.
        * Check if character is > a and < z or > A and < Z.
        * Add to letter count.
    */

    int letter_count = 0;
    int length = strlen(text);

    for (int i = 0; i < length; i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            letter_count++;
        }
    }

    return letter_count;
}


int number_of_words(string text)
{
    /*
        Counts the number of letters in the supplied text.

        * Loop through each character in text.
        * Check if character is a space.
        * Add to word count.
        * Add one.
    */

    int word_count = 0;
    int length = strlen(text);

    for (int i = 0; i < length; i++)
    {
        if (text[i] == ' ')
        {
            word_count++;
        }
    }

    word_count++;

    return word_count;
}


int number_of_sentences(string text)
{
    /*
        Counts the number of sentences in the supplied text.

        * Loop through each character in text.
        * Check if the current character is a period, exclamation point, or question mark.
        * Check if the following character is a space.
        * Add one.
    */

    int sentence_count = 0;
    int length = strlen(text);

    for (int i = 0; i < length; i++)
    {
        if ((text[i] == '.' && text[i + 1] == ' ') || (text[i] == '!' && text[i + 1] == ' ') || (text[i] == '?' && text[i + 1] == ' '))
        {
            sentence_count++;
        }
    }

    sentence_count++;

    return sentence_count;
}


int coleman_liau(string text)
{
    /*
    Calculates the Coleman-Liau index.

    Formula: index = 0.0588 * L - 0.296 * S - 15.8
    where L is the average number of letters per 100 words in the text, and S is the average number of sentences per 100 words in the text.
    */

    float letters_per_100_words = ((float) number_of_letters(text) * 100) / number_of_words(text);
    float sentences_per_100_words = ((float) number_of_sentences(text) * 100) / number_of_words(text);

    int index = round(0.0588 * letters_per_100_words - 0.296 * sentences_per_100_words - 15.8);

    return index;
}
