#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include "anagram.h"
#include "wordlengths.h"
#include "histogram.h"
#include <time.h>
#include <ctype.h>

void free_anagram_list(nodePrimary *head) {
    while (head) {
        nodePrimary *next_primary = head->next;
        node *current_word = head->words->next;
        while (current_word) {
            node *next_word = current_word->next;
            free(current_word->word);
            free(current_word);
            current_word = next_word;
        }
        free(head->words);
        free(head->sorted_key);
        free(head);
        head = next_primary;
    }
}

nodePrimary *create_node_primary(char *word)
{
    nodePrimary *new_node = (nodePrimary *)malloc(sizeof(nodePrimary));
    if (!new_node)
    {
        perror("Memory allocation failed");
        exit(1);
    }
    new_node->sorted_key = strdup(word);
    new_node->next = NULL;
    new_node->group_size = 1;
    new_node->words = (node *)malloc(sizeof(node));
    if (!new_node->words) {
        perror("Memory allocation failed for words list");
        exit(1);
    }
    new_node->words->word = NULL; 
    new_node->words->next = NULL; 
    return new_node;
}

void push_word(nodePrimary **head, char *sorted_key, char *word) {
    clock_t start_time = clock();
    static int calls = 0;
    static double total_time = 0.0;
    static int total_steps = 0;
    
    calls++;
    int step_count = 0;
    
    nodePrimary *cur = *head;
    nodePrimary *prev = NULL;

    size_t key_len = strlen(sorted_key);
    while (cur != NULL) {
        size_t cur_len = strlen(cur->sorted_key);
        if (cur_len < key_len) {
            prev = cur;
            cur = cur->next;
            step_count++;
        } else if (cur_len == key_len) {
            int cmp = strcmp(cur->sorted_key, sorted_key);
            if (cmp < 0) {
                prev = cur;
                cur = cur->next;
                step_count++;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    total_steps += step_count;
    
    if (cur != NULL && strcmp(cur->sorted_key, sorted_key) == 0) {
        node *new_word = (node *)malloc(sizeof(node));
        new_word->word = strdup(word);
        new_word->next = cur->words->next;
        cur->words->next = new_word;
        cur->group_size++;
    } else {
        nodePrimary *new_group = (nodePrimary *)malloc(sizeof(nodePrimary));
        new_group->sorted_key = strdup(sorted_key);
        new_group->next = cur;
        new_group->group_size = 1;
        new_group->words = (node *)malloc(sizeof(node));
        new_group->words->word = strdup(word);
        new_group->words->next = NULL;
        
        if (prev == NULL) {
            *head = new_group;
        } else {
            prev->next = new_group;
        }
    }
    
    clock_t end_time = clock();
    double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    total_time += elapsed;
    
    if (calls % 10000 == 0) {
        printf("TIMING: Processed %d words in %.2f seconds (avg: %.6f seconds/word)\n", 
               calls, total_time, total_time / calls);
        printf("TIMING: Average linked list traversal steps: %.2f\n", 
               (double)total_steps / calls);
    }
}

node *create_node(char *word)
{
    node *new_node = (node *)malloc(sizeof(node));
    if (!new_node)
    {
        perror("Memory allocation failed");
        exit(1);
    }
    new_node->word = strdup(word);
    new_node->next = NULL;
    return new_node;
}

void push(node **tail, char *word) {
    node *new_node = create_node(word);
    (*tail)->next = new_node;
    *tail = new_node;
}

void print_list(node *head) {
    while (head) {
        printf("%s -> ", head->word);
        head = head->next;
    }
    printf("NULL\n");
}

char *sorted(char *word) {
    clock_t start_time = clock();

    int count[26] = {0};
    int sanitized_len = 0;
    for (int i = 0; word[i]; i++) {
        char c = tolower(word[i]);
        if (c >= 'a' && c <= 'z') {
            count[c - 'a']++;
            sanitized_len++;
        }
    }
    
    char *sorted_word = malloc(sanitized_len + 1);
    int pos = 0;
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < count[i]; j++) {
            sorted_word[pos++] = 'a' + i;
        }
    }
    sorted_word[pos] = '\0';
    
    clock_t end_time = clock();
    static int sort_calls = 0;
    static double total_time = 0.0;
    
    sort_calls++;
    double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    total_time += elapsed;
    
    if (sort_calls % 10000 == 0) {
        printf("TIMING: Sorted %d words in %.2f seconds (avg: %.6f seconds/word)\n", 
               sort_calls, total_time, total_time / sort_calls);
    }
    
    return sorted_word;
}

void print_anagram_groups(nodePrimary *head) {
    nodePrimary *cur = head;
    while (cur != NULL) {
        printf("[%s] → ", cur->sorted_key);
        node *word_cur = cur->words;
        while (word_cur != NULL) {
            printf("%s ", word_cur->word);
            word_cur = word_cur->next;
        }
        printf("\n");
        cur = cur->next;
    }
}

int get_largest_variants(nodePrimary *head) {
    int max = INT_MIN;
    nodePrimary *cur = head;
    while (cur != NULL) {
        if (cur->group_size > max) {
            max = cur->group_size;
        }
        cur = cur->next;
    }
    return max;
}

int size_sec_list(node *head) {
    int size = 0;
    node *cur = head;
    while (cur != NULL) {
        size++;
        cur = cur->next;
    }
    return size;
}

void get_longest_pair(nodePrimary *head, char **word1, char **word2) {
    int max_length = 0;
    *word1 = NULL;
    *word2 = NULL;
    nodePrimary *cur = head;
    while (cur != NULL) {
        if (cur->words && cur->words->next && strlen(cur->words->word) > max_length) {
            max_length = strlen(cur->words->word);
            *word1 = cur->words->word;
            *word2 = cur->words->next->word;
        }
        cur = cur->next;
    }
    if (*word1 && *word2) {
        printf("Longest anagram pair: %s and %s (%d letters)\n", *word1, *word2, max_length);
    } else {
        printf("No valid anagram pairs found.\n");
    }
}

int contains(int *x, int num, int *n) {
    for (int i = 0; i < *n; i++) {
        if (x[i] == num) {
            return 1;
        }
    }
    return 0;
}

int get_index(int *x, int num, int n) {
    for (int i = 0; i < n; i++) {
        if (x[i] == num) {
            return i;
        }
    }
    return -1;
}

/**
 * Processes anagram groups to generate histogram data.
 * Skips groups of size 1; only groups of size >=2 are recorded.
 *
 * @param head Head of the anagram group list.
 * @param x Pointer to store group sizes.
 * @param H Pointer to store histogram values.
 * @param n Number of valid group sizes.
 */
void process(nodePrimary *head, int **x, double **H, int *n) {
    int max_group_size = 0;
    nodePrimary *cur = head;
    while (cur != NULL) {
        if (cur->group_size > max_group_size) {
            max_group_size = cur->group_size;
        }
        cur = cur->next;
    }
    int *freq = calloc(max_group_size + 1, sizeof(int));
    cur = head;
    while (cur != NULL) {
        freq[cur->group_size]++;
        cur = cur->next;
    }
    *n = 0;
    for (int i = 2; i <= max_group_size; i++) {  // skip group size 1
        if (freq[i] > 0) (*n)++;
    }
    *x = malloc(*n * sizeof(int));
    *H = malloc(*n * sizeof(double));
    int index = 0;
    for (int i = 2; i <= max_group_size; i++) {  // skip group size 1
        if (freq[i] > 0) {
            (*x)[index] = i;
            (*H)[index] = log10(freq[i]);
            index++;
        }
    }
    free(freq);
}

nodePrimary *make_anagram_list(char **words, int n) {
    nodePrimary *head = NULL;
    for (int i = 0; i < n; i++) {
        char *word = words[i];
        char *sorted_word = sorted(word);
        push_word(&head, sorted_word, word);
        free(sorted_word);
    }
    return head;
}