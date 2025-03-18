#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include "anagram.h"
#include "histogram.h"
#include <time.h>
#include <ctype.h>
#include "utils.h"

/**
 * Frees the anagram list, including all words and sorted keys.
 * @param head Pointer to the head of the anagram list.
 */
void free_anagram_list(nodePrimary *head) {
    while (head) {
        nodePrimary *next_primary = head->next;
        node *current_word = head->words;  // Start from the first word (not a sentinel)
        while (current_word) {
            node *next_word = current_word->next;
            free(current_word->word);
            free(current_word);
            current_word = next_word;
        }
        free(head->sorted_key);
        free(head);
        head = next_primary;
    }
}

/**
 * Creates a new primary node with a sorted key.
 * @param word Sorted key for the anagram group.
 * @return Pointer to the new node, or exits on failure.
 */
nodePrimary *create_node_primary(char *word) {
    nodePrimary *new_node = malloc(sizeof(nodePrimary));
    if (!new_node) {
        perror("Memory allocation failed");
        exit(1);
    }
    new_node->sorted_key = strdup(word);
    new_node->next = NULL;
    new_node->group_size = 1;
    new_node->words = malloc(sizeof(node));
    if (!new_node->words) {
        perror("Memory allocation failed for words list");
        exit(1);
    }
    new_node->words->word = NULL;  // Sentinel node
    new_node->words->next = NULL;
    return new_node;
}

/**
 * Adds a word to the anagram list under its sorted key.
 * @param head Pointer to the head of the list.
 * @param sorted_key Sorted version of the word.
 * @param word Original word to add.
 */
void push_word(nodePrimary **head, char *sorted_key, char *word) {
    clock_t start_time = clock();
    static int calls = 0;
    static double total_time = 0.0;
    static int total_steps = 0;
    calls++;

    nodePrimary *cur = *head, *prev = NULL;
    size_t key_len = strlen(sorted_key);
    int step_count = 0;

    // Find insertion point (sorted by length, then lexicographically)
    while (cur) {
        size_t cur_len = strlen(cur->sorted_key);
        int cmp = (cur_len == key_len) ? strcmp(cur->sorted_key, sorted_key) : (cur_len < key_len ? -1 : 1);
        if (cmp >= 0) break;
        prev = cur;
        cur = cur->next;
        step_count++;
    }
    total_steps += step_count;

    if (cur && strcmp(cur->sorted_key, sorted_key) == 0) {
        // Add to existing group
        node *new_word = malloc(sizeof(node));
        new_word->word = strdup(word);
        new_word->next = cur->words->next;  // Insert after first word
        cur->words->next = new_word;
        cur->group_size++;
    } else {
        // Create new group
        nodePrimary *new_group = malloc(sizeof(nodePrimary));
        new_group->sorted_key = strdup(sorted_key);
        new_group->next = cur;
        new_group->group_size = 1;
        new_group->words = malloc(sizeof(node));
        new_group->words->word = strdup(word);
        new_group->words->next = NULL;
        if (prev) prev->next = new_group; else *head = new_group;
    }

    // Timing stats
    clock_t end_time = clock();
    total_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;
    if (calls % 10000 == 0) {
        printf("TIMING: Processed %d words in %.2f seconds (avg: %.6f seconds/word)\n", 
               calls, total_time, total_time / calls);
        printf("TIMING: Average traversal steps: %.2f\n", (double)total_steps / calls);
    }
}

/**
 * Creates a new word node.
 * @param word Word to store.
 * @return Pointer to the new node, or exits on failure.
 */
node *create_node(char *word) {
    node *new_node = malloc(sizeof(node));
    if (!new_node) {
        perror("Memory allocation failed");
        exit(1);
    }
    new_node->word = strdup(word);
    new_node->next = NULL;
    return new_node;
}

/**
 * Appends a word node to the end of a list.
 * @param tail Pointer to the tail of the list.
 * @param word Word to append.
 */
void push(node **tail, char *word) {
    node *new_node = create_node(word);
    (*tail)->next = new_node;
    *tail = new_node;
}

/**
 * Prints a linked list of words.
 * @param head Head of the word list.
 */
void print_list(node *head) {
    while (head) {
        printf("%s -> ", head->word);
        head = head->next;
    }
    printf("NULL\n");
}

/**
 * Returns a sorted version of a word (alphabetic characters only).
 * @param word Input word.
 * @return Sorted string, dynamically allocated.
 */
char *sorted(char *word) {
    clock_t start_time = clock();
    int count[26] = {0}, sanitized_len = 0;

    // Count alphabetic characters
    for (int i = 0; word[i]; i++) {
        char c = tolower(word[i]);
        if (c >= 'a' && c <= 'z') {
            count[c - 'a']++;
            sanitized_len++;
        }
    }

    // Build sorted string
    char *sorted_word = malloc(sanitized_len + 1);
    int pos = 0;
    for (int i = 0; i < 26; i++)
        while (count[i]--) sorted_word[pos++] = 'a' + i;
    sorted_word[pos] = '\0';

    // Timing stats
    clock_t end_time = clock();
    static int sort_calls = 0;
    static double total_time = 0.0;
    total_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;
    if (++sort_calls % 10000 == 0) {
        printf("TIMING: Sorted %d words in %.2f seconds (avg: %.6f seconds/word)\n", 
                sort_calls, total_time, total_time / (double)sort_calls);
    }
    return sorted_word;
}

/**
 * Prints all anagram groups.
 * @param head Head of the anagram list.
 */
void print_anagram_groups(nodePrimary *head) {
    for (nodePrimary *cur = head; cur; cur = cur->next) {
        printf("[%s] → ", cur->sorted_key);
        for (node *word = cur->words; word; word = word->next)
            printf("%s ", word->word);
        printf("\n");
    }
}

/**
 * Finds the size of the largest anagram group.
 * @param head Head of the anagram list.
 * @return Largest group size.
 */
int get_largest_variants(nodePrimary *head) {
    int max = INT_MIN;
    for (nodePrimary *cur = head; cur; cur = cur->next)
        if (cur->group_size > max) max = cur->group_size;
    return max;
}

/**
 * Counts words in a secondary list.
 * @param head Head of the word list.
 * @return Number of words.
 */
int size_sec_list(node *head) {
    int size = 0;
    for (node *cur = head; cur; cur = cur->next) size++;
    return size;
}

/**
 * Finds and prints the longest anagram pair.
 * @param head Head of the anagram list.
 * @param word1 Pointer to store first word.
 * @param word2 Pointer to store second word.
 */
void get_longest_pair(nodePrimary *head, char **word1, char **word2) {
    int max_length = 0;
    *word1 = *word2 = NULL;
    for (nodePrimary *cur = head; cur; cur = cur->next) {
        if (cur->words && cur->words->next && strlen(cur->words->word) > max_length) {
            max_length = strlen(cur->words->word);
            *word1 = cur->words->word;
            *word2 = cur->words->next->word;
        }
    }
    if (*word1 && *word2)
        printf("Longest anagram pair: %s and %s (%d letters)\n", *word1, *word2, max_length);
    else
        printf("No valid anagram pairs found.\n");
}

/**
 * Processes anagram groups for histogram data (groups >= 2).
 * @param head Head of the anagram list.
 * @param x Pointer to store group sizes.
 * @param H Pointer to store log10 frequencies.
 * @param n Pointer to store number of valid groups.
 */
void process(nodePrimary *head, int **x, double **H, int *n) {
    int max_group_size = 0;
    for (nodePrimary *cur = head; cur; cur = cur->next)
        if (cur->group_size > max_group_size) max_group_size = cur->group_size;

    int *freq = calloc(max_group_size + 1, sizeof(int));
    for (nodePrimary *cur = head; cur; cur = cur->next)
        freq[cur->group_size]++;

    *n = 0;
    for (int i = 2; i <= max_group_size; i++)
        if (freq[i] > 0) (*n)++;

    *x = malloc(*n * sizeof(int));
    *H = malloc(*n * sizeof(double));
    int index = 0;
    for (int i = 2; i <= max_group_size; i++)
        if (freq[i] > 0) {
            (*x)[index] = i;
            (*H)[index++] = log10(freq[i]);
        }
    free(freq);
}

/**
 * Builds an anagram list from an array of words.
 * @param words Array of words.
 * @param n Number of words.
 * @return Head of the anagram list.
 */
nodePrimary *make_anagram_list(char **words, int n) {
    nodePrimary *head = NULL;
    for (int i = 0; i < n; i++) {
        char *sorted_word = sorted(words[i]);
        push_word(&head, sorted_word, words[i]);
        free(sorted_word);
    }
    return head;
}