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
 * Displays the current state of all piles, with an optional note about the next action.
 * Each card takes up 3 spaces for neat alignment. If verbose mode is on, an annotation
 * (like "covering cards") appears starting at column 30 for readability.
 * @param head Pointer to the start of the pile list.
 * @param action Message to show as an annotation (ignored if empty or verbose is off).
 * @param verbose If 1, include the annotation; if 0, just show the pile state.
 */
static void print_piles_verbose(Pile *head, const char *action, int verbose) {
    int col = 0;
    for (Pile *cur = head; cur; cur = cur->next) {
        col += printf("%3d", cur->top->value); // Print each card, track column width
    }
    if (verbose && action && action[0] != '\0') {
        int padding = (30 - col) < 1 ? 1 : (30 - col); // Ensure at least 1 space before annotation
        printf("%*s%s", padding, "", action);
    }
    printf("\n");
}

/**
 * Pulls the next card from the deck, keeping track of the current position.
 * The deck starts at index 0, and top moves up as cards are drawn.
 * @param deck Pointer to the deck structure.
 * @return The value of the drawn card, or -1 if the deck is out of cards.
 */
int draw_from_deck(Deck *deck) {
    if (deck->top >= 52) {
        printf("Error: No more cards in the deck!\n");
        return -1;
    }
    return deck->cards[deck->top++]; // Return card and increment position
}

/**
 * Creates a new pile with a single card and adds it to the end of the pile list.
 * Updates both head and tail pointers to keep the list linked properly.
 * @param head Pointer to the start of the pile list (updated if list was empty).
 * @param tail Pointer to the end of the pile list (updated to new pile).
 * @param card The value of the card to start the new pile with.
 */
void add_pile(Pile **head, Pile **tail, int card) {
    Pile *new_pile = malloc(sizeof(Pile));
    if (!new_pile) { perror("Failed to allocate pile"); exit(EXIT_FAILURE); }
    new_pile->top = malloc(sizeof(CardNode));
    if (!new_pile->top) { free(new_pile); perror("Failed to allocate card node"); exit(EXIT_FAILURE); }
    new_pile->top->value = card;
    new_pile->top->next = NULL;
    new_pile->next = NULL;
    if (!*head) { *head = *tail = new_pile; } // First pile sets both head and tail
    else { (*tail)->next = new_pile; *tail = new_pile; } // Add to end and update tail
}

/**
 * Looks for pairs of piles where the top cards add up to 11 (e.g., 6 and 5).
 * Uses a hash table to efficiently find matches as it scans the piles.
 * @param visible_piles Pointer to the start of the pile list.
 * @param num_piles_to_cover Pointer to store the number of piles in the returned array.
 * @return Array of pile pointers (two per pair found), which will need covering.
 */
Pile **add_to_11(Pile *visible_piles, int *num_piles_to_cover) {
    Pile *hash[14] = {NULL}; // Hash table for card values 1-13 (0 unused)
    Pile **piles_to_cover = malloc(sizeof(Pile *) * 18); // Max 9 pairs possible with 9 piles
    *num_piles_to_cover = 0;
    for (Pile *cur = visible_piles; cur; cur = cur->next) {
        int val = cur->top->value;
        int needed = 11 - val; // What value pairs with this to make 11
        if (needed > 0 && needed <= 10 && hash[needed]) { // Found a pair
            piles_to_cover[(*num_piles_to_cover)++] = hash[needed]; // Add first pile
            piles_to_cover[(*num_piles_to_cover)++] = cur;          // Add current pile
            hash[needed] = NULL; // Clear the match from hash
        } else {
            hash[val] = cur; // Store this pile in hash for potential future match
        }
    }
    return piles_to_cover;
}

/**
 * Scans the pile list for a Jack (11), Queen (12), and King (13) to remove as a set.
 * Only takes the first occurrence of each value if multiple exist.
 * @param head Pointer to the start of the pile list.
 * @param count Pointer to store the number of piles found (0 or 3).
 * @return Array of 3 pile pointers if J, Q, K are found; NULL otherwise.
 */
Pile **jqk(Pile *head, int *count) {
    Pile *j = NULL, *q = NULL, *k = NULL;
    for (Pile *cur = head; cur; cur = cur->next) {
        if (cur->top->value == 11 && !j) j = cur; // Jack
        if (cur->top->value == 12 && !q) q = cur; // Queen
        if (cur->top->value == 13 && !k) k = cur; // King
    }
    if (j && q && k) { // All three found
        Pile **result = malloc(3 * sizeof(Pile *));
        result[0] = j; result[1] = q; result[2] = k;
        *count = 3;
        return result;
    }
    *count = 0; // No complete set found
    return NULL;
}

/**
 * Replaces the top cards of specified piles with new ones from the deck.
 * Frees the old cards and adds fresh ones, but only if deck has cards left.
 * @param piles Array of piles whose top cards will be replaced.
 * @param count Number of piles to process.
 * @param deck Pointer to the deck to draw from.
 * @param verbose If 1, print details (not used here but kept for consistency).
 */
void cover_cards(Pile **piles, int count, Deck *deck, int verbose) {
    if (!piles) return;
    for (int i = 0; i < count && deck->top < 52; i++) {
        int new_card = deck->cards[deck->top++]; // Draw next card
        free(piles[i]->top); // Remove old card
        piles[i]->top = malloc(sizeof(CardNode));
        piles[i]->top->value = new_card;
        piles[i]->top->next = NULL;
    }
}

/**
 * Sets up a new deck with 52 cards (1-13 repeated 4 times for suits).
 * Cards aren’t shuffled here—shuffle happens elsewhere if needed.
 * @return A fresh Deck structure with all 52 cards.
 */
Deck initialize_deck() {
    Deck deck;
    deck.top = 0; // Start at the beginning
    for (int i = 0; i < 52; i++) {
        deck.cards[i] = (i % 13) + 1; // Values 1-13, cycling every 13 cards
    }
    return deck;
}

/**
 * Starts the game by creating two initial piles from the deck.
 * Draws two cards and sets up the linked list with head and tail pointers.
 * @param deck Pointer to the deck to draw from.
 * @param head Pointer to the start of the pile list (will be set).
 * @param tail Pointer to the end of the pile list (will be set).
 */
void initialize_game(Deck *deck, Pile **head, Pile **tail) {
    deck->top = 0; // Reset deck position
    add_pile(head, tail, draw_from_deck(deck)); // First pile
    add_pile(head, tail, draw_from_deck(deck)); // Second pile
}

/**
 * Counts how many piles are currently in the game.
 * Simply walks the list and tallies each one.
 * @param head Pointer to the start of the pile list.
 * @return Total number of piles.
 */
int count_piles(Pile *head) {
    int count = 0;
    for (Pile *cur = head; cur; cur = cur->next) count++;
    return count;
}

/**
 * Runs the main game loop for Patience, following the rules:
 * - Cover pairs adding to 11 or JQK sets with new cards.
 * - Add a new pile if no matches are found.
 * Stops when 9 piles are reached or deck runs out.
 * @param deck Pointer to the deck to play with.
 * @param verbose If 1, show annotations for each move; if 0, just pile states.
 * @return Number of cards left in the deck (0 if all used).
 */
int play(Deck *deck, int verbose) {
    Pile *head = NULL, *tail = NULL;
    initialize_game(deck, &head, &tail); // Set up starting piles

    while (count_piles(head) < 9 && deck->top < 52) { // Loop until 9 piles or deck empty
        char annotation[256] = ""; // Buffer for move descriptions
        int action_taken = 0; // Track if we made a move this turn

        // First, check for pairs that add to 11
        int pair_count = 0;
        Pile **pairs = add_to_11(head, &pair_count);
        if (pair_count > 0) {
            if (verbose) { // Prepare a helpful note about the pair
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
            cover_cards(pairs, pair_count, deck, verbose); // Replace matched cards
            free(pairs);
            action_taken = 1;
        } else {
            // If no pairs, check for JQK set
            int jqk_count = 0;
            Pile **jqk_piles = jqk(head, &jqk_count);
            if (jqk_count == 3) {
                if (verbose) { // Note about JQK removal
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
                cover_cards(jqk_piles, 3, deck, verbose); // Replace JQK cards
                free(jqk_piles);
                action_taken = 1;
            }
        }

        // If no matches, add a new pile from the deck
        if (!action_taken && deck->top < 52) {
            int new_card = deck->cards[deck->top];
            if (verbose) snprintf(annotation, sizeof(annotation), "Cards don't add to 11; will start a new pile with %d", new_card);
            print_piles_verbose(head, annotation, verbose);
            add_pile(&head, &tail, draw_from_deck(deck));
        }
    }

    // Show the final game state with a summary
    if (verbose) {
        char final_annotation[256];
        snprintf(final_annotation, sizeof(final_annotation),
                 count_piles(head) == 9 ? "Game ended with 9 piles" : "Game ended with no cards left in deck");
        print_piles_verbose(head, final_annotation, verbose);
    }

    // Clean up all piles and return remaining card count
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
 * Plays the game multiple times and counts how often each number of cards remains.
 * Each game starts with a shuffled deck, and results are tallied in an array.
 * @param n Number of games to simulate.
 * @return Array where index is cards left, value is frequency of that outcome.
 */
int *many_plays(int n) {
    int *remaining = calloc(53, sizeof(int)); // Space for 0-52 cards left
    int seed = -1; // Start with random shuffle, then use fixed seed
    for (int i = 0; i < n; i++) {
        Deck deck = initialize_deck();
        shuffle(deck.cards, 52, seed); // Shuffle (random first time, then repeatable)
        seed = 0; // Fix seed after first game
        int left = play(&deck, 1); // Play with verbose output
        remaining[left]++; // Record how many cards were left
        printf("\n");
    }
    return remaining;
}

/**
 * Creates an array of all possible outcomes (0 to 52 cards left) for histogram use.
 * This ensures every possible result is represented, even if it didn’t occur.
 * @param num_labels Pointer to store the total number of labels (always 53).
 * @return Array with values 0 through 52.
 */
int *get_labels(int *num_labels) {
    *num_labels = 53; // Covers 0 to 52 cards left
    int *labels = malloc(*num_labels * sizeof(int));
    if (!labels) { 
        perror("Memory allocation failed for labels"); 
        exit(1); 
    }
    for (int i = 0; i < *num_labels; i++) {
        labels[i] = i; // Fill with 0, 1, 2, ..., 52
    }
    return labels;
}

/**
 * Turns raw frequencies into percentages for each possible number of cards left.
 * Divides each frequency by total games and multiplies by 100 for a percentage.
 * @param results Array of frequencies from many_plays.
 * @param num_games Total number of games simulated.
 * @param num_labels Number of possible outcomes (53 for 0-52).
 * @return Array of percentages corresponding to each outcome.
 */
double *get_percentages(int *results, int num_games, int num_labels) {
    double *percentages = malloc(num_labels * sizeof(double));
    if (!percentages) { 
        perror("Memory allocation failed for percentages"); 
        exit(1); 
    }
    for (int i = 0; i < num_labels; i++) {
        percentages[i] = (results[i] * 100.0) / num_games; // Convert to percentage
    }
    return percentages;
}