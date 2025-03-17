#include <stdio.h>
#include "patience.h"
#include "shuffle/shuffle.h"
#include "histogram.h"

int draw_from_deck(Deck* deck) {
    if (deck->top >= 52) { 
        printf("Error: No more cards in the deck!\n");
        return -1; // Indicate failure
    }
    return deck->cards[deck->top++];
}

void add_pile(Pile** head, Pile** tail, int card) {
    Pile* new_pile = (Pile*)malloc(sizeof(Pile));
    if (!new_pile) {
        perror("Failed to allocate pile");
        exit(EXIT_FAILURE);
    }

    new_pile->top = (CardNode*)malloc(sizeof(CardNode));
    if (!new_pile->top) {
        free(new_pile);
        perror("Failed to allocate card node");
        exit(EXIT_FAILURE);
    }

    new_pile->top->value = card;
    new_pile->top->next = NULL;
    new_pile->next = NULL;

    if (*head == NULL) {
        *head = new_pile;
        *tail = new_pile;
    } else {
        (*tail)->next = new_pile;
        *tail = new_pile;
    }
}

Pile** add_to_11(Pile *visible_piles, int *num_piles_to_cover) {
    Pile* hash[14] = {NULL};
    Pile** piles_to_cover = malloc(sizeof(Pile*) * 2 * 9);
    *num_piles_to_cover = 0;
    Pile* cur = visible_piles;
    while (cur != NULL) {
        int val = cur->top->value;
        int needed = 11 - val;

        if (needed > 0 && needed <= 10 && hash[needed] != NULL) {
            piles_to_cover[(*num_piles_to_cover)++] = hash[needed];
            piles_to_cover[(*num_piles_to_cover)++] = cur;
            hash[needed] = NULL;
        } else {    
            hash[val] = cur;
        }

        cur = cur->next;
    }
    return piles_to_cover;
}

Pile** jqk(Pile *head, int *count) {
    Pile *j = NULL, *q = NULL, *k = NULL;
    Pile* cur = head;
    
    while (cur) {
        if (cur->top->value == 11 && !j) j = cur;
        if (cur->top->value == 12 && !q) q = cur;
        if (cur->top->value == 13 && !k) k = cur;
        cur = cur->next;
    }
    
    if (j && q && k) {
        Pile** result = malloc(3 * sizeof(Pile*));
        result[0] = j;
        result[1] = q;
        result[2] = k;
        *count = 3;
        return result;
    }
    *count = 0;
    return NULL;
}

void cover_cards(Pile** piles, int count, Deck* deck, int verbose) {
    if (!piles) return;
    
    for (int i = 0; i < count; i++) {
        if (deck->top >= 52) break;
        
        int new_card = deck->cards[deck->top++];
        
        // Free old top card
        CardNode* old = piles[i]->top;
        piles[i]->top = malloc(sizeof(CardNode));
        piles[i]->top->value = new_card;
        piles[i]->top->next = NULL;
        free(old);
        
        if (verbose) printf("Covered pile %d with %d\n", i+1, new_card);
    }
}

Deck initialize_deck() {
    Deck deck;
    deck.top = 0;
    for (int i = 0; i < 52; i++) {
        deck.cards[i] = (i % 13) + 1; 
    }
    shuffle(deck.cards, 52, -1); 
    return deck;
}

void initialize_game(Deck* deck, Pile** head, Pile** tail) {
    deck->top = 0; 

    int card1 = draw_from_deck(deck);
    int card2 = draw_from_deck(deck);

    add_pile(head, tail, card1);
    add_pile(head, tail, card2);
}

int count_piles(Pile *head) {
    int count = 0;
    Pile* cur = head;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }
    return count;
}

void print_piles(Pile* head, int verbose) {
    if (!head) return;
    
    Pile* cur = head;
    while (cur) {
        if (verbose) printf("[%d] ", cur->top->value);
        else printf("%d ", cur->top->value);
        cur = cur->next;
    }
    if (verbose) printf("← deck\n");
    else printf("\n");
}

int play(Deck* deck, int verbose) {
    Pile* head = NULL;
    Pile* tail = NULL;

    initialize_game(deck, &head, &tail);

    if (verbose) {
        printf("\n🔹 Game Started!\n");
        print_piles(head, verbose);
    }

    while (count_piles(head) < 9 && deck->top < 52) {
        int action_taken = 0;
        
        // Check for 11 pairs
        int pair_count = 0;
        Pile** pairs = add_to_11(head, &pair_count);
        if (pair_count > 0) {
            if (verbose) printf("Processing %d pairs...\n", pair_count/2);
            cover_cards(pairs, pair_count, deck, verbose);
            free(pairs);
            action_taken = 1;
        }
        
        // Check for J/Q/K if no pairs found
        if (!action_taken) {
            int jqk_count = 0;
            Pile** jqk_piles = jqk(head, &jqk_count);
            if (jqk_count == 3) {
                if (verbose) printf("Processing J/Q/K...\n");
                cover_cards(jqk_piles, 3, deck, verbose);
                free(jqk_piles);
                action_taken = 1;
            }
        }
        
        // Add new pile if no actions taken
        if (!action_taken) {
            int new_card = draw_from_deck(deck);
            if (verbose) printf("Adding new pile: %d\n", new_card);
            add_pile(&head, &tail, new_card);
        }
        
        if (verbose) print_piles(head, verbose);
        else print_piles(head, 0);
    }

    // if (verbose) printf("\n");

    int result = (deck->top == 52) ? 0 : (52 - deck->top);
    // if (result == 0) {
    //     if (verbose) printf("\n🎉 Player Wins! Deck is empty.\n");
    // } else {
    //     if (verbose) {
    //         printf("%d and %d add to 11, but %d is the only card left in deck\n",
    //                head->top->value, head->next->top->value, head->next->next->top->value);
    //     }
    // }

    // Free all piles and card nodes here
    Pile* current = head;
    while (current != NULL) {
        Pile* next_pile = current->next;
        
        // Free all card nodes in the pile
        CardNode* current_card = current->top;
        while (current_card != NULL) {
            CardNode* next_card = current_card->next;
            free(current_card);
            current_card = next_card;
        }
        
        free(current);
        current = next_pile;
    }

    return result;
}

int* many_plays(int n) {
    int* remaining = calloc(53, sizeof(int)); 
    for (int i = 0; i < n; i++) {
        Deck deck = initialize_deck();
        int remaining_cards = play(&deck,0);
        remaining[remaining_cards]++;  
    }
    return remaining;  
}

int* get_labels(int* results, int* num_labels) {
    *num_labels = 0;

    // First, count how many labels we need
    for (int i = 0; i <= 52; i++) {
        if (results[i] > 0) {
            (*num_labels)++;
        }
    }

    // Allocate memory for labels
    int* labels = (int*)malloc(*num_labels * sizeof(int));
    if (!labels) {
        perror("Memory allocation failed for labels");
        exit(1);
    }

    // Fill labels array
    int index = 0;
    for (int i = 0; i <= 52; i++) {
        if (results[i] > 0 && index < *num_labels) {
            labels[index++] = i;
        }
    }

    return labels;
}

double* get_percentages(int* results, int num_games, int num_labels) {
    double* percentages = malloc(num_labels * sizeof(double));
    int index = 0;
    for (int i = 0; i <= 52; i++) {
        if (results[i] > 0 && index < num_labels) {
            percentages[index++] = (results[i] / (double)num_games) * 100;
        }
    }
    return percentages;
}

int main() {
    int n = 10000; 
    int num_labels = 0;
    int* matches = many_plays(n);
    
    int* labels = get_labels(matches, &num_labels);
    double* percentages = get_percentages(matches, n, num_labels); 
    
    histogram(labels, percentages, num_labels, 30); 
    
    free(labels);
    free(percentages);
    free(matches);
}