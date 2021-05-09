main: main.c
	gcc -c main.c -o main.o
	gcc main.o -lm -o main
