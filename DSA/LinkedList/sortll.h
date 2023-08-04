#ifndef SORTLL_h
#define SORTLL

#include <stdio.h>
#include <stdlib.h>

// Creating structure
struct Node {
	int data;
	struct Node* next;
};

int isEmpty(struct Node *head);
void insert(struct Node** head, int value);
void deletenode(struct Node **head, int key);
void display(struct Node* head);
void sort (struct Node *head);

struct Node* partition(struct Node* first, struct Node* last);
struct Node* last_node(struct Node* head);
void quick_sort(struct Node* first, struct Node* last);

#endif
