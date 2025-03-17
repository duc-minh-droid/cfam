#include "wordlengths.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
 
    wordlengths(argv[1]);
 
    return 0;
 }