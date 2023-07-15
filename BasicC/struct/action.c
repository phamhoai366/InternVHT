#include "action.h"


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
