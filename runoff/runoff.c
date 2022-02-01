#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{

    for (int i = 0; i < candidate_count; i++)
    {
        // If the name provided is a valid candidate...
        if (strcmp(candidates[i].name, name) == 0)
        {
            // e.g. For voter 0, the 2nd rank is candidate 1...
            preferences[voter][rank] = i;
            return true;
        }
    }

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{

    for (int i = 0; i < voter_count; i++)
    {
        // Keep looping until the current voter's current ranked candidate is not eliminated
        // If the candidate at the current rank is not eliminated, add a vote for them
        // Otherwise, just increase the current rank and try again
        for (int j = 0; j < candidate_count; j++)
        {
            if (candidates[preferences[i][j]].eliminated != true)
            {
                candidates[preferences[i][j]].votes++;
                break;
            }
        }

    }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int total_votes = 0;
    int highest_vote_total = 0;
    string current_winner;

    // Figure out the total number of votes cast in the election
    for (int x = 0; x < candidate_count; x++)
    {
        total_votes += candidates[x].votes;
    }

    // Check if the current candidate has the highest number of votes so far
    for (int i = 0; i < candidate_count; i++)
    {
        printf("Candidate %s has %i votes.\n", candidates[i].name, candidates[i].votes);
        if (candidates[i].votes > highest_vote_total)
        {
            highest_vote_total = candidates[i].votes;
            current_winner = candidates[i].name;
        }
    }

    // Only print out a winner if the candidate with the highest vote total has more than 50% of the total votes
    if ( (float) highest_vote_total / (float) total_votes > .5)
    {
        printf("%s\n", current_winner);
        return true;
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int fewest_vote_count = 99999;

    // Figure out the total number of votes cast in the election
    for (int i = 0; i < candidate_count; i++)
    {
        // If the current candidate is still in the race...
        if (candidates[i].eliminated != true)
        {
            // Check to see if the current candidate's number of votes is less than the fewest vote count.
            // If so, that candidate now has the fewest vote count
            if (candidates[i].votes < fewest_vote_count)
            {
                fewest_vote_count = candidates[i].votes;
            }
        }
    }

    return fewest_vote_count;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int num_of_votes;
    bool first_candidate = true;

    // For each candidate...
    for (int i = 0; i < candidate_count; i++)
    {
        // If the candidate has not yet been eliminated...
        if (candidates[i].eliminated != true)
        {
            // Set of the current number of votes equal to the current candidate's votes if it's the
            // first non-eliminated candidate we're looking at
            if (first_candidate == true)
            {
                first_candidate = false;
                num_of_votes = candidates[i].votes;
            }
            else
            {
                // Check if the current candidate equals the previous candidate's number of votes.
                // If not, the election is not tied.
                if (candidates[i].votes != num_of_votes)
                {
                    return false;
                }
            }
        }
    }

    // The election is tied since all the candidates had the same number of votes
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }

    return;
}
