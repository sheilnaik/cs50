// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10000;

// Number of words in dictionary
int word_counter = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Get the hash number for the word
    int hash_num = hash(word);

    // Keep going through the linked list at the hash number until you reach NULL
    // If the word at the current node in the linked list matches the queried word,
    // return True.
    for (node *tmp = table[hash_num]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
    }

    // No word found, return False.
    return false;
}

// Hashes word to a number
// Source: https://github.com/hathix/cs50-section/blob/master/code/7/sample-hash-functions/good-hash-function.c
unsigned int hash(const char *word)
{
    // MY BASIC ATTEMPT:
    // char first_letter = toupper(word[0]);
    // int num = first_letter - 'A';

    // return num;

    unsigned int hash = 5381;
    int c;

    for (const char *ptr = word; *ptr != '\0'; ptr++)
    {
        hash = ((hash << 5) + hash) + tolower(*ptr);
    }

    return hash % N;
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file. Quit if cannot open dictionary file.
    FILE *dict_file = fopen(dictionary, "r");
    if (dict_file == NULL)
    {
        return false;
    }

    char *word;
    char dict_word[LENGTH];
    int hash_num;

    // Loop through each word in dictionary file, saving to dict_word, until it reaches end of file
    for (int c = fscanf(dict_file, "%s", dict_word); feof(dict_file) == 0; c = fscanf(dict_file, "%s", dict_word))
    {
        // Allocate memory to a new node
        node *n = malloc(sizeof(node));

        // Quit if cannot allocate memory
        if (n == NULL)
        {
            return false;
        }

        // Add one to the word counter
        word_counter++;

        // Get the hash number of the word
        hash_num = hash(dict_word);

        // 1. Copy the word to the node's 'word' attribute
        // 2. Set the 'next' attribute equal to the linked list
        // 3. Set the linked list equal back to the node
        strcpy(n->word, dict_word);
        n->next = table[hash_num];
        table[hash_num] = n;

    }

    fclose(dict_file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }

    return true;
}
