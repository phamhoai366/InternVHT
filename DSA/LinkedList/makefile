
main: main.o sortll.o
	gcc -o main main.o sortll.o

main.o: main.c sortll.h
	gcc -c main.c

sortll.o: sortll.c sortll.h
	gcc -c sortll.c

run: main
	./main

clean: rm *.o main 
