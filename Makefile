.PHONY: all clean
FLAGS = -Wall -Wextra -Werror -m32

all: program

program: main.o functions.o
	gcc $(FLAGS) functions.o main.o -o program -lm

main.o: main.c
	gcc $(FLAGS) -c main.c -o main.o -lm

functions.o: functions.asm
	nasm -f elf32 -o functions.o functions.asm

clean:
	rm -rf *.o
