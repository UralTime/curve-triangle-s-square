.PHONY: all clean
FLAGS = -Wall -Wextra -Werror -m32 -lm

all: program

program: main.o functions.o
	gcc $(FLAGS) functions.o main.o -o program

main.o: main.c
	gcc $(FLAGS) -c main.c -o main.o

functions.o: functions.asm
	nasm -f elf32 -o functions.o functions.asm

clean:
	rm -rf program *.o