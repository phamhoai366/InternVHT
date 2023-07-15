#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Data structure to represent a stack
struct stack
{
	int maxsize;	// define max capacity of the stack
	int top;
	char *items;
};

// Utility function to initialize the stack
struct stack* newStack(int capacity){
	struct stack *pt = (struct stack*)malloc(sizeof(struct stack));

	pt->maxsize = capacity;
	pt->top = -1;
	pt->items = (int*)malloc(sizeof(int) * capacity);

	return pt;
}

// Utility function to return the size of the stack
int size(struct stack *pt) {
	return pt->top + 1;
}

// Utility function to check if the stack is empty or not
int isEmpty(struct stack *pt) {
	return pt->top == -1;   				// or return size(pt) == 0;
}

// Utility function to check if the stack is full or not
int isFull(struct stack *pt) {
	return pt->top == pt->maxsize - 1;  	// or return size(pt) == pt->maxsize;
}

// Utility function to add an element `x` to the stack
void push(struct stack *pt, char x)
{
	// check if the stack is already full. Then inserting an element would
	// lead to stack overflow
	if (isFull(pt))
	{
		printf("Overflow\nProgram Terminated\n");
	}

	printf("Inserting %c\n", x);

	// add an element and increment the top's index
	pt->items[++pt->top] = x;
}

// Utility function to return the top element of the stack
char peek(struct stack *pt)
{
	// check for an empty stack
	if (!isEmpty(pt)) {
		return pt->items[pt->top];
	}
	else {
		printf("overflow");
	}
}

// Utility function to pop a top element from the stack
char pop(struct stack *pt)
{
	// check for stack underflow
	if (isEmpty(pt))
	{
		printf("Underflow\nProgram Terminated\n");	
	}

	printf("Removing %c\n", peek(pt));

	// decrement stack size by 1 and (optionally) return the popped element
	return pt->items[pt->top--];
}

int main()
{
	// create a stack of capacity 5
	struct stack *pt = newStack(5);

	push(pt, 'h');
	push(pt, 'o');
	push(pt, 'a');
	push(pt, 'i');

	printf("The top element is %c\n", peek(pt));
	printf("The stack size is %d\n", size(pt));

	// pop(pt);
	// pop(pt);
	// // pop(pt);

	if (isEmpty(pt)) {
		printf("The stack is empty");
	}
	else {
		printf("The stack is not empty");
	}

	return 0;
}