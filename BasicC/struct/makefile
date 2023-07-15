# CC = gcc
# CFLAGS = -g -Wall

main: main.o action.o
	gcc -o main main.o action.o

main.o: main.c action.h
	gcc -c main.c

action.o: action.c action.h
	gcc -c action.c

run: main
	./main

clean: 
	rm *.o main output.txt