# Compiler and flags
CC = gcc
CFLAGS = -std=c99
GSL_INCLUDE = -I/opt/homebrew/include
GSL_RPATH = -Wl,-rpath,/opt/homebrew/lib
GSL_LIBS = /opt/homebrew/lib/libgsl.dylib /opt/homebrew/lib/libgslcblas.dylib
MATH_LIB = -lm

# List of all targets
all: demo_histogram wordlengths anagram pstatistics anaquery

# Object file rules
utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

wordlengths.o: wordlengths.c
	$(CC) $(CFLAGS) -c wordlengths.c -o wordlengths.o

anagram.o: anagram.c anagram.h
	$(CC) $(CFLAGS) -c anagram.c -o anagram.o

patience.o: patience.c
	$(CC) $(CFLAGS) $(GSL_INCLUDE) -c patience.c -o patience.o

pstatistics.o: pstatistics.c
	$(CC) $(CFLAGS) $(GSL_INCLUDE) -c pstatistics.c -o pstatistics.o

histogram.o: histogram.c
	$(CC) $(CFLAGS) -c histogram.c -o histogram.o

anaquery.o: anaquery.c utils.h anagram.h
	$(CC) $(CFLAGS) -c anaquery.c -o anaquery.o

# Executable rules
demo_histogram: demo_histogram.c histogram.o
	$(CC) $(CFLAGS) demo_histogram.c histogram.o -o demo_histogram

wordlengths: demo_wordlengths.c wordlengths.o histogram.o utils.o
	$(CC) $(CFLAGS) demo_wordlengths.c wordlengths.o histogram.o utils.o -o wordlengths

anagram: demo_anagram.c anagram.o histogram.o utils.o
	$(CC) $(CFLAGS) demo_anagram.c anagram.o histogram.o utils.o -o anagram

pstatistics: pstatistics.o patience.o anagram.o histogram.o wordlengths.o shuffle.o utils.o
	$(CC) $(CFLAGS) pstatistics.o patience.o anagram.o histogram.o wordlengths.o shuffle.o utils.o $(GSL_RPATH) $(GSL_LIBS) $(MATH_LIB) -o pstatistics

anaquery: anaquery.o utils.o anagram.o
	$(CC) $(CFLAGS) anaquery.o utils.o anagram.o -o anaquery

# Clean up generated files
clean:
	rm -f *.o demo_histogram wordlengths anagram pstatistics anaquery