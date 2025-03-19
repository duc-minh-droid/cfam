#ifndef ANAGRAM_H
#define ANAGRAM_H

typedef struct node {
    char *word;         // Word in the node
    struct node *next;  // Next word node
} node;

typedef struct nodePrimary {
    char *sorted_key;         // Sorted key for the anagram group
    struct node *words;       // List of words in the group
    int group_size;           // Number of words in the group
    struct nodePrimary *next; // Next anagram group
} nodePrimary;

void free_anagram_list(nodePrimary *head);
nodePrimary *create_node_primary(char *word);
void push_word(nodePrimary **head, char *sorted_key, char *word);
node *create_node(char *word);
void push(node **tail, char *word);
void print_list(node *head);
char *sorted(char *word);
void print_anagram_groups(nodePrimary *head);
int get_largest_variants(nodePrimary *head);
int size_sec_list(node *head);
void get_longest_pair(nodePrimary *head, char **word1, char **word2);
void process(nodePrimary *head, int **x, double **H, int *n);
nodePrimary *make_anagram_list(char **words, int n);

#endif 