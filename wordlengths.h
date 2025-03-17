#ifndef WORDLENGTHS_H
#define WORDLENGTHS_H

#include <stdio.h>
#include <stdlib.h>

int get_file_size(char *file_path);

char **read_txt_file(char *file_path, int size);

int *get_indexes(double *H, int size);

// int find_max_width(int *lengths, int size); // Uncomment if needed

void free_words(char **words, int size);

void wordlengths(char *file_path);

#endif // WORDLENGTHS_H
