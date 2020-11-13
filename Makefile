main: *.c
	gcc -Wall -O3 *.c -o main -lm -lcurses
