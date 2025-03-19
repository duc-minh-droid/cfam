#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>  
#include "utils.h"    
#include "anagram.h"

/**
 * Finds the anagram group corresponding to a sorted key.
 * @param list Pointer to the head of the anagram list.
 * @param key The sorted key to search for.
 * @return Pointer to the nodePrimary with the matching sorted_key, or NULL if not found.
 */
nodePrimary *find_group(nodePrimary *list, char *key) {
    nodePrimary *current = list;  
    while (current != NULL) {     // Traverse until the end of the list
        if (strcmp(current->sorted_key, key) == 0) {  
            return current;  // Found a match, return the node
        }
        current = current->next;  
    }
    return NULL; 
}

int main() {
    const char *filename = "words2.txt";

    // Get the number of words (lines) in the file
    int num_words = get_file_size(filename);
    if (num_words == -1) {
        fprintf(stderr, "Failed to get file size\n");
        return 1;
    } else if (num_words == 0) {
        fprintf(stderr, "File is empty\n");
        return 1;
    }

    // Read words from the file
    char **word_list = read_txt_file(filename, num_words);
    if (!word_list) {
        fprintf(stderr, "Failed to read words from file\n");
        return 1;
    }

    // Build the anagram list
    nodePrimary *anagram_list = make_anagram_list(word_list, num_words);
    if (!anagram_list) {
        fprintf(stderr, "Failed to create anagram list\n");
        for (int i = 0; i < num_words; i++) {
            free(word_list[i]);
        }
        free(word_list);
        return 1;
    }

    // Interactive loop to query anagrams
    while (1) {
        printf("Enter a word (or press Enter to quit): ");
        char input[100];
        if (!fgets(input, sizeof(input), stdin)) {
            break;  // Exit on EOF or error
        }

        input[strcspn(input, "\n")] = '\0';  // Remove newline
        if (strlen(input) == 0) {
            break;  // Exit on empty input
        }

        // Get the sorted key for the input
        char *key = sorted(input);
        if (!key) {
            fprintf(stderr, "Failed to sort input word\n");
            continue;
        }

        // Find and display anagrams
        nodePrimary *group = find_group(anagram_list, key);
        printf("Anagrams of '%s': ", input);
        if (group && group->words) {
            int found = 0;
            for (node *word_node = group->words; word_node != NULL; word_node = word_node->next) {
                if (word_node->word && strcasecmp(word_node->word, input) != 0) {
                    printf("%s ", word_node->word);
                    found = 1;
                }
            }
            if (!found) {
                printf("None");
            }
        } else {
            printf("None");
        }
        printf("\n");

        free(key);  // Clean up sorted key
    }

    // Clean up memory
    free_anagram_list(anagram_list);
    for (int i = 0; i < num_words; i++) {
        free(word_list[i]);
    }
    free(word_list);

    return 0;
}