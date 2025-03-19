# Compiler and flags
CC = gcc
CFLAGS = -std=c99
MATH_LIB = -lm

# Use pkg-config for GSL
GSL_CFLAGS = $(shell pkg-config --cflags gsl)
GSL_LIBS = $(shell pkg-config --libs gsl)

# List of all targets
all: demo_histogram wordlengths pstatistics anaquery

# Object file rules
shuffle.o: shuffle.c
	$(CC) $(CFLAGS) -c shuffle.c -o shuffle.o

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

wordlengths.o: wordlengths.c
	$(CC) $(CFLAGS) -c wordlengths.c -o wordlengths.o

anagram.o: anagram.c anagram.h
	$(CC) $(CFLAGS) -c anagram.c -o anagram.o

patience.o: patience.c
	$(CC) $(CFLAGS) $(GSL_CFLAGS) -c patience.c -o patience.o

pstatistics.o: pstatistics.c
	$(CC) $(CFLAGS) $(GSL_CFLAGS) -c pstatistics.c -o pstatistics.o

histogram.o: histogram.c
	$(CC) $(CFLAGS) -c histogram.c -o histogram.o

anaquery.o: anaquery.c utils.h anagram.h
	$(CC) $(CFLAGS) -c anaquery.c -o anaquery.o

# Executable rules
demo_histogram: demo_histogram.c histogram.o
	$(CC) $(CFLAGS) demo_histogram.c histogram.o -o demo_histogram $(MATH_LIB)

wordlengths: wordlengths.o histogram.o utils.o
	$(CC) $(CFLAGS) wordlengths.o histogram.o utils.o -o wordlengths $(MATH_LIB)

pstatistics: pstatistics.o patience.o anagram.o histogram.o shuffle.o utils.o
	$(CC) $(CFLAGS) pstatistics.o patience.o anagram.o histogram.o shuffle.o utils.o -o pstatistics $(GSL_LIBS) $(MATH_LIB)

anaquery: anaquery.o utils.o anagram.o
	$(CC) $(CFLAGS) anaquery.o utils.o anagram.o -o anaquery $(MATH_LIB)

# Clean up generated files
clean:
	rm -f *.o demo_histogram wordlengths pstatistics anaquery