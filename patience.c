#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include "patience.h"
#include "shuffle.h"
#include "histogram.h"
#include "utils.h"

/**
 * Prints the current state of piles with an optional annotation.
 * Cards are printed in a width of 3; annotations (if verbose) start at column 30.
 * @param head Pointer to the pile list.
 * @param action Annotation message (or empty string).
 * @param verbose If 1, print annotation; if 0, print state only.
 */
static void print_piles_verbose(Pile *head, const char *action, int verbose) {
    int col = 0;
    for (Pile *cur = head; cur; cur = cur->next) {
        col += printf("%3d", cur->top->value); // Print each card, track column width
    }
    if (verbose && action && action[0] != '\0') {
        int padding = (30 - col) < 1 ? 1 : (30 - col); // Pad to column 30 or at least 1 space
        printf("%*s%s", padding, "", action);
    }
    printf("\n");
}

/**
 * Draws a card from the deck.
 * @param deck Pointer to the deck.
 * @return Card value, or -1 if deck is empty.
 */
int draw_from_deck(Deck *deck) {
    if (deck->top >= 52) {
        printf("Error: No more cards in the deck!\n");
        return -1;
    }
    return deck->cards[deck->top++];
}

/**
 * Adds a new pile with a given card to the list.
 * @param head Pointer to the head of the pile list.
 * @param tail Pointer to the tail of the pile list.
 * @param card Card value for the new pile.
 */
void add_pile(Pile **head, Pile **tail, int card) {
    Pile *new_pile = malloc(sizeof(Pile));
    if (!new_pile) { perror("Failed to allocate pile"); exit(EXIT_FAILURE); }
    new_pile->top = malloc(sizeof(CardNode));
    if (!new_pile->top) { free(new_pile); perror("Failed to allocate card node"); exit(EXIT_FAILURE); }
    new_pile->top->value = card;
    new_pile->top->next = NULL;
    new_pile->next = NULL;
    if (!*head) { *head = *tail = new_pile; }
    else { (*tail)->next = new_pile; *tail = new_pile; }
}

/**
 * Finds pairs of piles where top cards add to 11.
 * @param visible_piles Pointer to the pile list.
 * @param num_piles_to_cover Number of piles in the returned array.
 * @return Array of pile pointers (2 per pair).
 */
Pile **add_to_11(Pile *visible_piles, int *num_piles_to_cover) {
    Pile *hash[14] = {NULL};
    Pile **piles_to_cover = malloc(sizeof(Pile *) * 18); // Max 9 pairs
    *num_piles_to_cover = 0;
    for (Pile *cur = visible_piles; cur; cur = cur->next) {
        int val = cur->top->value;
        int needed = 11 - val;
        if (needed > 0 && needed <= 10 && hash[needed]) {
            piles_to_cover[(*num_piles_to_cover)++] = hash[needed];
            piles_to_cover[(*num_piles_to_cover)++] = cur;
            hash[needed] = NULL;
        } else {
            hash[val] = cur;
        }
    }
    return piles_to_cover;
}

/**
 * Checks for piles with J (11), Q (12), K (13).
 * @param head Pointer to the pile list.
 * @param count Number of piles found (0 or 3).
 * @return Array of 3 pile pointers if JQK found, else NULL.
 */
Pile **jqk(Pile *head, int *count) {
    Pile *j = NULL, *q = NULL, *k = NULL;
    for (Pile *cur = head; cur; cur = cur->next) {
        if (cur->top->value == 11 && !j) j = cur;
        if (cur->top->value == 12 && !q) q = cur;
        if (cur->top->value == 13 && !k) k = cur;
    }
    if (j && q && k) {
        Pile **result = malloc(3 * sizeof(Pile *));
        result[0] = j; result[1] = q; result[2] = k;
        *count = 3;
        return result;
    }
    *count = 0;
    return NULL;
}

/**
 * Replaces top cards of given piles with new cards from the deck.
 * @param piles Array of piles to cover.
 * @param count Number of piles to cover.
 * @param deck Pointer to the deck.
 * @param verbose If 1, print details (unused here).
 */
void cover_cards(Pile **piles, int count, Deck *deck, int verbose) {
    if (!piles) return;
    for (int i = 0; i < count && deck->top < 52; i++) {
        int new_card = deck->cards[deck->top++];
        free(piles[i]->top);
        piles[i]->top = malloc(sizeof(CardNode));
        piles[i]->top->value = new_card;
        piles[i]->top->next = NULL;
    }
}

/**
 * Creates a shuffled deck of 52 cards (1-13, 4 suits).
 * @return Initialized deck.
 */
Deck initialize_deck() {
    Deck deck;
    deck.top = 0;
    for (int i = 0; i < 52; i++) {
        deck.cards[i] = (i % 13) + 1;
    }
    return deck;
}

/**
 * Sets up the game with two initial piles.
 * @param deck Pointer to the deck.
 * @param head Pointer to the head of the pile list.
 * @param tail Pointer to the tail of the pile list.
 */
void initialize_game(Deck *deck, Pile **head, Pile **tail) {
    deck->top = 0;
    add_pile(head, tail, draw_from_deck(deck));
    add_pile(head, tail, draw_from_deck(deck));
}

/**
 * Counts the number of piles in the list.
 * @param head Pointer to the pile list.
 * @return Number of piles.
 */
int count_piles(Pile *head) {
    int count = 0;
    for (Pile *cur = head; cur; cur = cur->next) count++;
    return count;
}

/**
 * Runs the game loop, checking for pairs or JQK, covering cards, or adding piles.
 * @param deck Pointer to the deck.
 * @param verbose If 1, print annotations; if 0, print state only.
 * @return Cards left in deck (0 if exhausted).
 */
int play(Deck *deck, int verbose) {
    Pile *head = NULL, *tail = NULL;
    initialize_game(deck, &head, &tail);

    while (count_piles(head) < 9 && deck->top < 52) {
        char annotation[256] = "";
        int action_taken = 0;

        // Check for pairs adding to 11
        int pair_count = 0;
        Pile **pairs = add_to_11(head, &pair_count);
        if (pair_count > 0) {
            if (verbose) {
                int v1 = pairs[0]->top->value, v2 = pairs[1]->top->value;
                int available = 52 - deck->top;
                if (available < 2) {
                    snprintf(annotation, sizeof(annotation), "%d and %d add to 11, but %s",
                             v1, v2, available ? "only 1 card left" : "no cards left");
                } else {
                    snprintf(annotation, sizeof(annotation), "%d and %d add to 11; will cover with %d and %d",
                             v1, v2, deck->cards[deck->top], deck->cards[deck->top + 1]);
                }
            }
            print_piles_verbose(head, annotation, verbose);
            cover_cards(pairs, pair_count, deck, verbose);
            free(pairs);
            action_taken = 1;
        } else {
            // Check for JQK
            int jqk_count = 0;
            Pile **jqk_piles = jqk(head, &jqk_count);
            if (jqk_count == 3) {
                if (verbose) {
                    int available = 52 - deck->top;
                    if (available < 3) {
                        snprintf(annotation, sizeof(annotation), 
                                 "J, Q, K visible, but %s", 
                                 available ? "not enough cards" : "no cards left");
                    } else {
                        snprintf(annotation, sizeof(annotation), 
                                 "J, Q, K visible; will cover with %d, %d, %d", 
                                 deck->cards[deck->top], deck->cards[deck->top + 1], deck->cards[deck->top + 2]);
                    }
                }
                print_piles_verbose(head, annotation, verbose);
                cover_cards(jqk_piles, 3, deck, verbose);
                free(jqk_piles);
                action_taken = 1;
            }
        }

        // If no action, add a new pile
        if (!action_taken && deck->top < 52) {
            int new_card = deck->cards[deck->top];
            if (verbose) snprintf(annotation, sizeof(annotation), "Cards don't add to 11; will start a new pile with %d", new_card);
            print_piles_verbose(head, annotation, verbose);
            add_pile(&head, &tail, draw_from_deck(deck));
        }
    }

    // Print final state
    if (verbose) {
        char final_annotation[256];
        snprintf(final_annotation, sizeof(final_annotation),
                 count_piles(head) == 9 ? "Game ended with 9 piles" : "Game ended with no cards left in deck");
        print_piles_verbose(head, final_annotation, verbose);
    }

    // Clean up
    int result = deck->top == 52 ? 0 : (52 - deck->top);
    for (Pile *cur = head; cur; ) {
        Pile *next = cur->next;
        free(cur->top);
        free(cur);
        cur = next;
    }
    printf("\n");
    return result;
}

/**
 * Simulates n games and tracks frequency of cards left.
 * @param n Number of games to play.
 * @return Array of frequencies indexed by cards left.
 */
int *many_plays(int n) {
    int *remaining = calloc(53, sizeof(int));
    int seed = -1;
    for (int i = 0; i < n; i++) {
        Deck deck = initialize_deck();
        shuffle(deck.cards, 52, seed);
        seed = 0;
        int left = play(&deck, 1); // Verbose on for demo
        remaining[left]++;
        printf("\n");
    }
    return remaining;
}

/**
 * Gets all possible indices (0 to 52) for the histogram.
 * @param num_labels Pointer to store the number of labels (always 53).
 * @return Array of labels (0 to 52).
 */
int *get_labels(int *num_labels) {
    *num_labels = 53; // 0 to 52 inclusive
    int *labels = malloc(*num_labels * sizeof(int));
    if (!labels) { 
        perror("Memory allocation failed for labels"); 
        exit(1); 
    }
    for (int i = 0; i < *num_labels; i++) {
        labels[i] = i;
    }
    return labels;
}

/**
 * Calculates percentages from frequencies for all possible cards left (0 to 52).
 * @param results Frequency array.
 * @param num_games Total games played.
 * @param num_labels Number of labels (53).
 * @return Array of percentages for each possible number of cards left.
 */
double *get_percentages(int *results, int num_games, int num_labels) {
    double *percentages = malloc(num_labels * sizeof(double));
    if (!percentages) { 
        perror("Memory allocation failed for percentages"); 
        exit(1); 
    }
    for (int i = 0; i < num_labels; i++) {
        percentages[i] = (results[i] * 100.0) / num_games;
    }
    return percentages;
}