#ifndef ACTION_H
#define ACTION

# include <stdio.h>
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

void cat_speak(animal_t animal);
void dog_speak(animal_t animal);
void pig_speak(animal_t animal);
void create_animals(animal_t animals[], int count);
void write_to_file(char* output);

#endif




