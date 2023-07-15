#include "action.h"


int main(){
    animal_t animals[MAX_ANIMAL];
    int n;
    
    printf("Enter the number of animals: ");
    scanf("%d", &n);
    
    if (n < 1 || n > MAX_ANIMAL) {
        printf("Invalid number of animals.\n");
    }
    
    create_animals(animals, n);
    
	FILE *fp;
	fp = fopen("output.txt", "w");
    for (i = 0; i < n; i++) {
        if (strcmp(animals[i].type, "cat") == 0) {
            // animals[i].action();
            fprintf(fp, "%s\n", "meow meow");
        } else if (strcmp(animals[i].type, "dog") == 0) {
            // animals[i].action();
            fprintf(fp, "%s\n", "bow bow");
        } else if (strcmp(animals[i].type, "pig") == 0) {
            char output[50];
            sprintf(output, "my price is %d", animals[i].price.pig_price);
            // animals[i].action();
            fprintf(fp, "%s\n", output);
        }
    }

	fclose(fp);  
    return 0;
}