histogram: histogram.c
	gcc -std=c17 histogram.c -o histogram

demo_histogram: demo_histogram.c histogram.c
	gcc -std=c17 demo_histogram.c histogram.c -o demo_histogram

wordlengths: wordlengths.c histogram.c
	gcc -std=c17 wordlengths.c histogram.c -o wordlengths

anagram: anagram.c histogram.c wordlengths.c
	gcc -std=c17 anagram.c histogram.c wordlengths.c -o anagram

patience: patience.c anagram.c histogram.c wordlengths.c shuffle.c
	gcc -std=c17 patience.c anagram.c histogram.c wordlengths.c shuffle.c -lgsl -lgslcblas -lm -o patience

demo_anagram: demo_anagram.c anagram.c histogram.c wordlengths.c
	gcc -std=c17 demo_anagram.c anagram.c histogram.c wordlengths.c -o demo_anagram

demo_patience: demo_patience.c patience.c anagram.c histogram.c wordlengths.c shuffle.c
	gcc -std=c17 demo_patience.c patience.c anagram.c histogram.c wordlengths.c shuffle.c -lgsl -lgslcblas -lm -o demo_patience