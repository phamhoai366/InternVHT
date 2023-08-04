#include "sortll.h"

int main()
{
	struct Node* head = NULL;

	insert(&head, 37);
	insert(&head, 9);
	insert(&head, 7);
	insert(&head, 21);
	insert(&head, 28);
	insert(&head, 65);
	insert(&head, 14);
	insert(&head, 38);
	insert(&head, 22);
	insert(&head, 11);
	printf("\n linked list: ");
	display(head);

	// deletenode (&head, 14);
	// printf("\n linked list after delete: ");
	// display(head);


    // printf("\nafter sort: ");
    // display(head);

	quick_sort(head, last_node(head));
	printf("\n After quickSort: ");
	display(head);
	return 0;
}
