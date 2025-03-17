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

int main() {
    clock_t start_time, end_time;
    double processing_time;
    printf("Starting anagram processing...\n");
    start_time = clock();
    printf("Counting words in file...\n");
    int file_size = get_file_size("words2.txt");
    printf("File contains %d words\n", file_size);
    FILE *file = fopen("words2.txt", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    char **words = malloc(file_size * sizeof(char *));
    char word[255];
    int words_processed = 0;
    printf("Reading words from file...\n");
    while (fgets(word, sizeof(word), file)) {
        word[strcspn(word, "\n")] = '\0';
        words[words_processed] = strdup(word);
        words_processed++;
    }
    fclose(file);
    printf("\nProcessing words into anagram groups...\n");
    nodePrimary *head = make_anagram_list(words, file_size);
    for (int i = 0; i < file_size; i++) {
        free(words[i]);
    }
    free(words);
    end_time = clock();
    processing_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("\nProcessing complete!\n");
    printf("Total processing time: %.2f seconds\n", processing_time);
    printf("Average processing speed: %.2f words/second\n", file_size / processing_time);
    printf("\nCalculating statistics...\n");
    start_time = clock();
    int lv = get_largest_variants(head);
    end_time = clock();
    processing_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Largest number of variants: %d (calculated in %.2f seconds)\n", lv, processing_time);
    start_time = clock();
    char *l1 = NULL, *l2 = NULL;
    get_longest_pair(head, &l1, &l2);
    end_time = clock();
    processing_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Longest pair calculation took %.2f seconds\n", processing_time);
    printf("\nGenerating histogram...\n");
    start_time = clock();
    int *x = NULL;
    double *H = NULL;
    int n = 0;
    process(head, &x, &H, &n);
    end_time = clock();
    processing_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Histogram data generation took %.2f seconds\n", processing_time);
    printf("\nAnagram Group Size Distribution:\n");
    histogram(x, H, n, 50);
    printf("\nCleaning up memory...\n");
    free(x);
    free(H);
    free_anagram_list(head);
    printf("Program complete!\n");
    return 0;
}
