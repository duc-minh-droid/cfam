#define _POSIX_C_SOURCE 200809L
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
 * Frees all memory used by the anagram list, including the words and sorted keys.
 * This function walks through the primary linked list (anagram groups) and, for each
 * group, frees its list of words before freeing the group itself. It prevents memory leaks
 * by ensuring every dynamically allocated piece is released.
 * @param head Pointer to the head of the anagram list (the first group).
 */
void free_anagram_list(nodePrimary *head) {
    while (head) {
        nodePrimary *next_primary = head->next; // Save next group before freeing current
        node *current_word = head->words;       // Start of this group's word list
        while (current_word) {
            node *next_word = current_word->next; // Save next word before freeing current
            free(current_word->word);             // Free the word string
            free(current_word);                   // Free the word node
            current_word = next_word;             // Move to next word
        }
        free(head->sorted_key); // Free the sorted key string for this group
        free(head);             // Free the group node itself
        head = next_primary;    // Move to next group
    }
}

/**
 * Creates a new anagram group node with a sorted key and an empty word list.
 * This sets up a primary node to hold a group of anagrams. It includes a sorted key
 * (the letters of the anagrams in order) and starts with a sentinel node in the word list,
 * though this function isn't currently used in the code—it might be a leftover or for future use.
 * @param word The sorted key (e.g., "aet" for "eat" or "tea").
 * @return Pointer to the newly created group node, or exits if memory allocation fails.
 */
nodePrimary *create_node_primary(char *word) {
    nodePrimary *new_node = malloc(sizeof(nodePrimary));
    if (!new_node) {
        perror("Memory allocation failed");
        exit(1);
    }
    new_node->sorted_key = strdup(word); // Copy the sorted key
    new_node->next = NULL;               // No next group yet
    new_node->group_size = 1;            // Starts with one word (though none added yet)
    new_node->words = malloc(sizeof(node));
    if (!new_node->words) {
        perror("Memory allocation failed for words list");
        exit(1);
    }
    new_node->words->word = NULL;  // Sentinel node (empty placeholder)
    new_node->words->next = NULL;  // End of word list
    return new_node;
}

/**
 * Adds a word to the anagram list, grouping it with others that have the same sorted key.
 * This function keeps the list sorted by key length first, then alphabetically. If the sorted
 * key already exists, it adds the word to that group; otherwise, it creates a new group.
 * @param head Pointer to the pointer of the list's head (allows modification of head).
 * @param sorted_key Sorted version of the word (e.g., "aet" for "tea").
 * @param word The original word to add (e.g., "tea").
 */
void push_word(nodePrimary **head, char *sorted_key, char *word) {
    nodePrimary *cur = *head, *prev = NULL;
    size_t key_len = strlen(sorted_key);

    // Find where to insert: sorted by key length, then alphabetically
    while (cur) {
        size_t cur_len = strlen(cur->sorted_key);
        int cmp = (cur_len == key_len) ? strcmp(cur->sorted_key, sorted_key) : (cur_len < key_len ? -1 : 1);
        if (cmp >= 0) break; // Stop if current key is >= sorted_key
        prev = cur;
        cur = cur->next;
    }

    if (cur && strcmp(cur->sorted_key, sorted_key) == 0) {
        // Sorted key exists, add word to this group
        node *new_word = malloc(sizeof(node));
        new_word->word = strdup(word);
        new_word->next = cur->words->next; // Insert after the first word
        cur->words->next = new_word;       // Link it in
        cur->group_size++;                 // Increment group size
    } else {
        // No matching group, create a new one
        nodePrimary *new_group = malloc(sizeof(nodePrimary));
        new_group->sorted_key = strdup(sorted_key);
        new_group->next = cur;           // Insert before cur (or at end if cur is NULL)
        new_group->group_size = 1;       // New group starts with one word
        new_group->words = malloc(sizeof(node));
        new_group->words->word = strdup(word); // First word in group
        new_group->words->next = NULL;         // End of word list
        if (prev) prev->next = new_group; else *head = new_group; // Link into list
    }
}

/**
 * Creates a new node to hold a single word in a word list.
 * Allocates memory for the node and copies the word into it.
 * @param word The word to store in the node.
 * @return Pointer to the new word node, or exits if memory allocation fails.
 */
node *create_node(char *word) {
    node *new_node = malloc(sizeof(node));
    if (!new_node) {
        perror("Memory allocation failed");
        exit(1);
    }
    new_node->word = strdup(word); // Copy the word
    new_node->next = NULL;         // No next node yet
    return new_node;
}

/**
 * Adds a word to the end of a word list by updating the tail pointer.
 * Useful for building lists efficiently by tracking the end.
 * @param tail Pointer to the pointer of the list's tail (updated to new node).
 * @param word The word to append.
 */
void push(node **tail, char *word) {
    node *new_node = create_node(word);
    (*tail)->next = new_node; // Link current tail to new node
    *tail = new_node;         // Update tail to new end
}

/**
 * Prints all words in a linked list, showing the chain with arrows.
 * Useful for debugging or displaying a single group's words.
 * @param head Pointer to the first node in the word list.
 */
void print_list(node *head) {
    while (head) {
        printf("%s -> ", head->word);
        head = head->next;
    }
    printf("NULL\n");
}

/**
 * Creates a sorted version of a word using only its alphabetic characters.
 * Ignores case (converts to lowercase) and non-letters, returning a string of sorted letters.
 * This serves as the key for grouping anagrams (e.g., "Tea2" → "aet").
 * @param word The input word to sort.
 * @return A new, dynamically allocated sorted string.
 */
char *sorted(char *word) {
    int count[26] = {0}, sanitized_len = 0;

    // Count lowercase alphabetic characters only
    for (int i = 0; word[i]; i++) {
        char c = tolower(word[i]);
        if (c >= 'a' && c <= 'z') {
            count[c - 'a']++;
            sanitized_len++;
        }
    }

    // Build the sorted string from counts
    char *sorted_word = malloc(sanitized_len + 1);
    int pos = 0;
    for (int i = 0; i < 26; i++)
        while (count[i]--) sorted_word[pos++] = 'a' + i;
    sorted_word[pos] = '\0'; // Null-terminate

    return sorted_word;
}

/**
 * Prints all anagram groups, showing each sorted key and its words.
 * Displays the list in a readable format, one group per line.
 * @param head Pointer to the first anagram group.
 */
void print_anagram_groups(nodePrimary *head) {
    for (nodePrimary *cur = head; cur; cur = cur->next) {
        printf("[%s] → ", cur->sorted_key); // Show the sorted key
        for (node *word = cur->words; word; word = word->next)
            printf("%s ", word->word); // List all words in group
        printf("\n");
    }
}

/**
 * Finds the number of words in the largest anagram group.
 * Scans the list to find the group with the most anagrams.
 * @param head Pointer to the first anagram group.
 * @return The size of the largest group, or INT_MIN if list is empty.
 */
int get_largest_variants(nodePrimary *head) {
    int max = INT_MIN;
    for (nodePrimary *cur = head; cur; cur = cur->next)
        if (cur->group_size > max) max = cur->group_size;
    return max;
}

/**
 * Counts the number of words in a word list.
 * Useful for verifying the size of an anagram group’s word list.
 * @param head Pointer to the first word node.
 * @return The total number of words in the list.
 */
int size_sec_list(node *head) {
    int size = 0;
    for (node *cur = head; cur; cur = cur->next) size++;
    return size;
}

/**
 * Finds the longest pair of anagrams and prints them.
 * Looks for the group with the longest words (all words in a group have the same length)
 * and picks the first two. Only considers groups with at least two words.
 * @param head Pointer to the first anagram group.
 * @param word1 Pointer to store the first word of the pair.
 * @param word2 Pointer to store the second word of the pair.
 */
void get_longest_pair(nodePrimary *head, char **word1, char **word2) {
    int max_length = 0;
    *word1 = *word2 = NULL;
    for (nodePrimary *cur = head; cur; cur = cur->next) {
        if (cur->words && cur->words->next && strlen(cur->words->word) > max_length) {
            max_length = strlen(cur->words->word); // Update max length
            *word1 = cur->words->word;             // First word
            *word2 = cur->words->next->word;       // Second word
        }
    }
    if (*word1 && *word2)
        printf("Longest anagram pair: %s and %s (%d letters)\n", *word1, *word2, max_length);
    else
        printf("No valid anagram pairs found.\n");
}

/**
 * Prepares data for a histogram of anagram group sizes (only groups of 2 or more).
 * Counts how many groups have each size, then stores sizes and log10 of their frequencies
 * in arrays for plotting. Ignores groups of size 1 (not true anagrams).
 * @param head Pointer to the first anagram group.
 * @param x Pointer to array for group sizes (allocated here).
 * @param H Pointer to array for log10 of frequencies (allocated here).
 * @param n Pointer to store the number of valid group sizes.
 */
void process(nodePrimary *head, int **x, double **H, int *n) {
    int max_group_size = 0;
    for (nodePrimary *cur = head; cur; cur = cur->next)
        if (cur->group_size > max_group_size) max_group_size = cur->group_size;

    int *freq = calloc(max_group_size + 1, sizeof(int)); // Frequency of each size
    for (nodePrimary *cur = head; cur; cur = cur->next)
        freq[cur->group_size]++; // Count groups of each size

    *n = 0; // Count sizes >= 2 with at least one group
    for (int i = 2; i <= max_group_size; i++)
        if (freq[i] > 0) (*n)++;

    *x = malloc(*n * sizeof(int));    // Array of group sizes
    *H = malloc(*n * sizeof(double)); // Array of log10(frequencies)
    int index = 0;
    for (int i = 2; i <= max_group_size; i++)
        if (freq[i] > 0) {
            (*x)[index] = i;           // Store group size
            (*H)[index++] = log10(freq[i]); // Store log10 of frequency
        }
    free(freq); // Clean up temporary array
}

/**
 * Builds a list of anagram groups from an array of words.
 * For each word, computes its sorted key and adds it to the appropriate group.
 * Groups are sorted by key length and then alphabetically.
 * @param words Array of input words.
 * @param n Number of words in the array.
 * @return Pointer to the head of the anagram list.
 */
nodePrimary *make_anagram_list(char **words, int n) {
    nodePrimary *head = NULL;
    for (int i = 0; i < n; i++) {
        char *sorted_word = sorted(words[i]);     // Get sorted key
        push_word(&head, sorted_word, words[i]);  // Add to list
        free(sorted_word);                        // Free temporary key
    }
    return head;
}