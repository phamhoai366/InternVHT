#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_ANIMAL 100
int i;

typedef union {
        char dogcat_price[20];
        int pig_price: 20;
}price_info;

typedef struct{
	char type[20];
	price_info price; 
	void (*action)();		
} animal_t;


void cat_speak(animal_t animal) {
    printf("meow meow\n");
}

void dog_speak(animal_t animal) {
    printf("bow bow\n");
}

void pig_speak(animal_t animal) {
    printf("my price is %d\n", animal.price.pig_price);
}

void create_animals(animal_t animals[], int count) {
    srand(time(NULL));
    
    for (i = 0; i < count; i++) {
        int random_type = rand() % 3;
        
        if (random_type == 0) {
            strcpy(animals[i].type, "cat");
            strcpy(animals[i].price.dogcat_price, "not for sale");
            animals[i].action == cat_speak;
        } else if (random_type == 1) {
            strcpy(animals[i].type, "dog");
            strcpy(animals[i].price.dogcat_price, "i am your friend");
            animals[i].action = dog_speak;
        } else {
            strcpy(animals[i].type, "pig");
            animals[i].price.pig_price = 500000;
            animals[i].action = pig_speak;
        }
    }
}

void write_to_file(char* output) {
    FILE *fp;
    fp = fopen("output.txt", "w");
    fprintf(fp, "%s\n", output);
    fclose(fp);
}


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