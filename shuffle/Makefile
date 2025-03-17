CC = gcc

demo_shuffle: demo_shuffle.o shuffle.o
	$(CC) -o demo_shuffle demo_shuffle.o shuffle.o  -lgsl -lgslcblas -lm


shuffle.o:  shuffle.c shuffle.h

demo_shuffle.o: demo_shuffle.c shuffle.h

clean:
	rm -f demo_shuffle.o shuffle.o
	rm -f demo_shuffle
