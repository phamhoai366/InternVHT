#include "sortll.h"

// Isempty 
int isEmpty(struct Node *head){
	if (head == NULL) return 1;
	return 0;
}

// Add new node at end of linked list
void insert(struct Node** head, int value)
{
	// Create dynamic node
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
	if (node == NULL) {
		// checking memory overflow
		printf("Memory overflow\n");
	}
	else {
		node->data = value;
		node->next = NULL;
		if (*head == NULL) {
			*head = node;
		}
		else {
			struct Node* temp = *head;

			// finding last node
			while (temp->next != NULL) {
				temp = temp->next;
			}

			// adding node at last position
			temp->next = node;
		}
	}
}

void deletenode(struct Node **head, int key){
	struct Node *temp = *head, *prev;
	if (temp != NULL && temp->data == key){
		*head = temp->next;
		free(temp);
		return ;
	}

	    while (temp != NULL && temp->data != key)
    {
        prev = temp;
        temp = temp->next;
    }
 
    // If key was not present in linked list
    if (temp == NULL) return;
 
    // Unlink the node from linked list
    prev->next = temp->next;
 
    free(temp);  // Free memory
}


// Displaying linked list element
void display(struct Node* head)
{
	if (head == NULL) {
		printf("Empty linked list");
		return;
	}
	struct Node* temp = head;

	while (temp != NULL) {
		printf(" %d", temp->data);
		temp = temp->next;
	}
}

//sort O(N^2)
void sort (struct Node *head){
    struct Node *i, *j;
    int temp;
    for (i = head; i->next != NULL; i = i->next){
        for (j = i->next; j != NULL; j = j->next){
            if (i->data < j->data){
                temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
}

// Finding last node of linked list
struct Node* last_node(struct Node* head)
{
	struct Node* temp = head;
	while (temp != NULL && temp->next != NULL) {
		temp = temp->next;
	}
	return temp;
}

// We are Setting the given last node position to its proper position
struct Node* partition(struct Node* first, struct Node* last)
{
	// Get first node of given linked list
	struct Node* pivot = first;
	struct Node* front = first;
	int temp = 0;
	while (front != NULL && front != last) {
		if (front->data < last->data) {
			pivot = first;

			// Swapping node values
			temp = first->data;
			first->data = front->data;
			front->data = temp;

			// Visiting the next node
			first = first->next;
		}

		// Visiting the next node
		front = front->next;
	}

	// Change last node value to current node
	temp = first->data;
	first->data = last->data;
	last->data = temp;
	return pivot;
}

// Performing quick sort in the given linked list
void quick_sort(struct Node* first, struct Node* last)
{
	if (first == last) {
		return;
	}
	struct Node* pivot = partition(first, last);

	if (pivot != NULL && pivot->next != NULL) {
		quick_sort(pivot->next, last);
	}

	if (pivot != NULL && first != pivot) {
		quick_sort(first, pivot);
	}
}
