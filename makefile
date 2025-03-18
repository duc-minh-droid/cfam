utils.o: utils.c utils.h
	gcc -std=c17 -c utils.c -o utils.o

histogram: histogram.c
	gcc -std=c17 histogram.c -o histogram

demo_histogram: demo_histogram.c histogram.c
	gcc -std=c17 demo_histogram.c histogram.c -o demo_histogram

wordlengths: wordlengths.c histogram.c utils.o
	gcc -std=c17 wordlengths.c histogram.c utils.o -o wordlengths

demo_wordlengths: demo_wordlengths.c wordlengths.c histogram.c utils.o
	gcc -std=c17 demo_wordlengths.c wordlengths.c histogram.c utils.o -o demo_wordlengths

anagram: anagram.c histogram.c utils.o
	gcc -std=c17 anagram.c histogram.c utils.o -o anagram

patience: patience.c histogram.c shuffle.c utils.o
	gcc -std=c17 -o patience patience.c histogram.c shuffle.o utils.o -I/opt/homebrew/include -Wl,-rpath,/opt/homebrew/lib /opt/homebrew/lib/libgsl.dylib /opt/homebrew/lib/libgslcblas.dylib -lm

demo_anagram: demo_anagram.c anagram.c histogram.c utils.o
	gcc -std=c17 demo_anagram.c anagram.c histogram.c utils.o -o demo_anagram

pstatistics: pstatistics.c patience.c histogram.c shuffle.c utils.o
	gcc -std=c17 -o pstatistics pstatistics.c patience.c anagram.c histogram.c wordlengths.c shuffle.c utils.o -I/opt/homebrew/include -Wl,-rpath,/opt/homebrew/lib /opt/homebrew/lib/libgsl.dylib /opt/homebrew/lib/libgslcblas.dylib -lm