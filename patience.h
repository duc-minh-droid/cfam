#ifndef PATIENCE_H
#define PATIENCE_H

#include <stdio.h>
#include <stdlib.h>

// Structure for a card node in a pile.
typedef struct CardNode {
    int value;
    struct CardNode* next;
} CardNode;

// Structure for a pile of cards.
typedef struct Pile {
    CardNode* top;
    struct Pile* next;
} Pile;

// Structure for a deck of cards.
typedef struct Deck {
    int cards[52];
    int top;
} Deck;

// Draws a card from the deck.
// Returns the card value, or -1 if no more cards remain.
int draw_from_deck(Deck* deck);

// Adds a new pile with a single card to the list of piles.
void add_pile(Pile** head, Pile** tail, int card);

// Finds pairs of piles whose top cards add to 11.
// Returns an array of pointers to the two piles for each valid pair,
// and updates num_piles_to_cover with the number of piles to cover.
Pile** add_to_11(Pile *visible_piles, int *num_piles_to_cover);

// Finds a group of three piles with values Jack, Queen, and King.
// If found, returns an array of three pointers and sets count to 3; otherwise, count is 0.
Pile** jqk(Pile *head, int *count);

// Covers the top card of each pile in the given array with a new card from the deck.
// If verbose is non-zero, prints details of the action.
void cover_cards(Pile** piles, int count, Deck* deck, int verbose);

// Initializes and shuffles a deck of cards.
Deck initialize_deck(void);

// Initializes the game by drawing two cards and creating two initial piles.
void initialize_game(Deck* deck, Pile** head, Pile** tail);

// Counts the number of piles.
int count_piles(Pile *head);

// Prints the top card of each pile.
// If verbose is non-zero, prints with additional formatting.
void print_piles(Pile* head, int verbose);

// Plays one game of patience.
// Returns the number of cards remaining in the deck (0 if the deck is empty).
int play(Deck* deck, int verbose);

// Plays many games (n plays) and returns an array of counts indexed by the number of cards remaining.
int* many_plays(int n);

// From the results array, extracts and returns an array of labels (remaining cards counts)
// and updates num_labels with the number of labels.
int* get_labels(int* results, int* num_labels);

// Computes and returns an array of percentages for each label based on num_games.
double* get_percentages(int* results, int num_games, int num_labels);

#endif // PATIENCE_H
