#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

int contains(int *x, int num, int n);
int get_index(int *x, int num, int n);
int get_file_size(const char *file_path);
int *get_indexes(double *H, int size);
char **read_txt_file(const char *file_path, int size);
void free_words(char **words, int size);

#endif 