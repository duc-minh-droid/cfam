#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>  
#include "utils.h"    
#include "anagram.h"

/**
 * Searches the anagram list for a group matching a given sorted key.
 * This function walks through the list of anagram groups and checks each one’s sorted key
 * (e.g., "aet" for "eat" or "tea") against the provided key. If found, it returns that group.
 * @param list Pointer to the head of the anagram list (start of the chain of groups).
 * @param key The sorted key to look for (e.g., "aet").
 * @return Pointer to the matching nodePrimary if found, or NULL if no match exists.
 */
nodePrimary *find_group(nodePrimary *list, char *key) {
    nodePrimary *current = list;  // Start at the head of the list
    while (current != NULL) {     // Keep going until we reach the end
        if (strcmp(current->sorted_key, key) == 0) {  // Compare sorted keys
            return current;  // Match found, return this group
        }
        current = current->next;  // Move to the next group
    }
    return NULL;  // No match found after checking all groups
}

int main() {
    const char *filename = "words2.txt";  // File containing the list of words

    // Figure out how many words (lines) are in the file
    int num_words = get_file_size(filename);
    if (num_words == -1) {
        fprintf(stderr, "Failed to get file size\n"); 
        return 1;
    } else if (num_words == 0) {
        fprintf(stderr, "File is empty\n"); 
        return 1;
    }

    // Load all the words from the file into an array
    char **word_list = read_txt_file(filename, num_words);
    if (!word_list) {
        fprintf(stderr, "Failed to read words from file\n"); 
        return 1;
    }

    // Build the anagram list by grouping words with the same sorted letters
    nodePrimary *anagram_list = make_anagram_list(word_list, num_words);
    if (!anagram_list) {
        fprintf(stderr, "Failed to create anagram list\n");  
        // Clean up the word array before exiting
        for (int i = 0; i < num_words; i++) {
            free(word_list[i]);  
        }
        free(word_list);  
        return 1;
    }

    // Start an interactive loop to let the user query anagrams
    while (1) {
        printf("Enter a word (or press Enter to quit): ");  
        char input[100];  
        if (!fgets(input, sizeof(input), stdin)) {
            break;  // Exit if there’s an error or EOF (e.g., Ctrl+D/Ctrl+Z)
        }

        // Remove the trailing newline from the input
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) {
            break;  // User pressed Enter with no text, so quit
        }

        // Create a sorted version of the input word (e.g., "tea" → "aet")
        char *key = sorted(input);
        if (!key) {
            fprintf(stderr, "Failed to sort input word\n");  
            continue;  // Skip to next loop iteration
        }

        // Look for an anagram group matching the sorted key
        nodePrimary *group = find_group(anagram_list, key);
        printf("Anagrams of '%s': ", input);  // Show the word being queried
        if (group && group->words) {  // Check if a group exists with words
            int found = 0;  // Flag to track if we find any anagrams
            for (node *word_node = group->words; word_node != NULL; word_node = word_node->next) {
                // Skip the input word itself (case-insensitive comparison)
                if (word_node->word && strcasecmp(word_node->word, input) != 0) {
                    printf("%s ", word_node->word);
                    found = 1;  // Mark that we found at least one
                }
            }
            if (!found) {
                printf("None");  
            }
        } else {
            printf("None");  
        }
        printf("\n");  

        free(key);  // Free the sorted key to avoid a memory leak
    }

    // Clean up all allocated memory before exiting
    free_anagram_list(anagram_list);  // Free the anagram list and its contents
    for (int i = 0; i < num_words; i++) {
        free(word_list[i]);  // Free each word in the array
    }
    free(word_list);  

    return 0;  
}