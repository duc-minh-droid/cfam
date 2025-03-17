#ifndef PATIENCE_H
#define PATIENCE_H

#include <stdio.h>
#include <stdlib.h>
#include "shuffle/shuffle.h"

// 🔹 Card structure (represents a single card)
typedef struct CardNode {
    int value;
    struct CardNode *next;  // Next card in the stack (pile)
} CardNode;

// 🔹 Pile structure (stack of cards)
typedef struct Pile {
    CardNode *top;  // Top card of the pile (acts as a stack)
    struct Pile *next;  // Pointer to the next pile
} Pile;

// 🔹 Deck structure (array-based deck with an index for drawing)
typedef struct {
    int cards[52];  // Stores the shuffled deck
    int top;  // Index of the next card to draw
} Deck;

// 🔹 Function Prototypes

// 📌 Deck Management
Deck initialize_deck();
int draw_from_deck(Deck *deck);

// 📌 Pile Management
void add_pile(Pile **head, Pile **tail, int card);
void cover_cards(Pile** piles, int count, Deck* deck, int verbose);
void print_piles(Pile* head, int verbose);  // 🖨️ Print visualization of piles

// 📌 Game Logic
Pile** add_to_11(Pile *visible_piles, int *num_piles_to_cover);
Pile** jqk(Pile *head, int *count);
int play(Deck *deck, int verbose);
// int* many_plays(int N);

#endif  // PATIENCE_H
