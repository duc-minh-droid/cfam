#ifndef ANAGRAM_H
#define ANAGRAM_H

#include <stdio.h>
#include <stdlib.h>

// ✅ Structs for the linked list representation of anagrams
typedef struct SecondaryListNode {
    char *word;
    struct SecondaryListNode *next;
} node;

typedef struct PrimaryListNode {
    char *sorted_key;
    node *words;
    struct PrimaryListNode *next;
    int group_size;
} nodePrimary;

// ✅ Function Declarations

// Creates a new primary node for storing an anagram group
nodePrimary *create_node_primary(char *word);

// Adds a word to the correct anagram group in the linked list
void push_word(nodePrimary **head, char *sorted_key, char *word);

// Prints all anagram groups stored in the linked list
void print_anagram_groups(nodePrimary *head);

// Finds and returns the largest number of anagram variants
int get_largest_variants(nodePrimary *head);

// Finds the longest anagram pair (two longest words that are anagrams)
void get_longest_pair(nodePrimary *head, char **word1, char **word2);

// Sorts the letters of a word alphabetically and returns the sorted string
char *sorted(char *word);

// Returns the number of nodes in the primary linked list
int getPrimaryListSize(nodePrimary *head);

// Processes the primary linked list to generate histogram data
void process(nodePrimary *head, int **x, double **H, int *n);

// Contains function to check if a number exists in an array
int contains(int *x, int num, int *n);

// Finds the index of a number in an array
int get_index(int *x, int num, int n);

// Frees all allocated memory in the linked list
void free_anagram_list(nodePrimary *head);

nodePrimary *make_anagram_list(char **words, int size);

#endif
