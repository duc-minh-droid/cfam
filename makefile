# Compiler and flags
CC = gcc
CFLAGS = -std=c99
GSL_INCLUDE = -I/opt/homebrew/include
GSL_RPATH = -Wl,-rpath,/opt/homebrew/lib
GSL_LIBS = /opt/homebrew/lib/libgsl.dylib /opt/homebrew/lib/libgslcblas.dylib
MATH_LIB = -lm

# List of all targets
all: histogram demo_histogram wordlengths demo_wordlengths anagram demo_anagram patience pstatistics

# Object file rules
utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

histogram.o: histogram.c
	$(CC) $(CFLAGS) -c histogram.c -o histogram.o

wordlengths.o: wordlengths.c
	$(CC) $(CFLAGS) -c wordlengths.c -o wordlengths.o

anagram.o: anagram.c
	$(CC) $(CFLAGS) -c anagram.c -o anagram.o

shuffle.o: shuffle.c
	$(CC) $(CFLAGS) -c shuffle.c -o shuffle.o

patience.o: patience.c
	$(CC) $(CFLAGS) $(GSL_INCLUDE) -c patience.c -o patience.o

pstatistics.o: pstatistics.c
	$(CC) $(CFLAGS) $(GSL_INCLUDE) -c pstatistics.c -o pstatistics.o

# Executable rules
histogram: histogram.o
	$(CC) $(CFLAGS) histogram.o -o histogram

demo_histogram: demo_histogram.c histogram.o
	$(CC) $(CFLAGS) demo_histogram.c histogram.o -o demo_histogram

wordlengths: wordlengths.o histogram.o utils.o
	$(CC) $(CFLAGS) wordlengths.o histogram.o utils.o -o wordlengths

demo_wordlengths: demo_wordlengths.c wordlengths.o histogram.o utils.o
	$(CC) $(CFLAGS) demo_wordlengths.c wordlengths.o histogram.o utils.o -o demo_wordlengths

anagram: anagram.o histogram.o utils.o
	$(CC) $(CFLAGS) anagram.o histogram.o utils.o -o anagram

demo_anagram: demo_anagram.c anagram.o histogram.o utils.o
	$(CC) $(CFLAGS) demo_anagram.c anagram.o histogram.o utils.o -o demo_anagram

patience: patience.o histogram.o shuffle.o utils.o
	$(CC) $(CFLAGS) patience.o histogram.o shuffle.o utils.o $(GSL_RPATH) $(GSL_LIBS) $(MATH_LIB) -o patience

pstatistics: pstatistics.o patience.o anagram.o histogram.o wordlengths.o shuffle.o utils.o
	$(CC) $(CFLAGS) pstatistics.o patience.o anagram.o histogram.o wordlengths.o shuffle.o utils.o $(GSL_RPATH) $(GSL_LIBS) $(MATH_LIB) -o pstatistics

# Clean up generated files
clean:
	rm -f *.o histogram demo_histogram wordlengths demo_wordlengths anagram demo_anagram patience pstatistics