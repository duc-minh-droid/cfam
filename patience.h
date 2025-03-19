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

int draw_from_deck(Deck* deck);
void add_pile(Pile** head, Pile** tail, int card);
Pile** add_to_11(Pile *visible_piles, int *num_piles_to_cover);
Pile** jqk(Pile *head, int *count);
void cover_cards(Pile** piles, int count, Deck* deck, int verbose);
Deck initialize_deck(void);
void initialize_game(Deck* deck, Pile** head, Pile** tail);
int count_piles(Pile *head);
int play(Deck* deck, int verbose);
int* many_plays(int n);
int *get_labels(int *num_labels);
double *get_percentages(int *results, int num_games, int num_labels);

#endif