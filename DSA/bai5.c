#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int check(char* s, int begin, int n){
    char target[n+1];
    int a[1000];
    int dem = 0;
    for(int i = 0; i < strlen(s); i += n){
        strncpy(target, s + begin, n);
        target[n] = '\0';
        printf("%s ", target);
        begin += n;
        a[dem] = atoi(target);
        dem++;
    }
    // for (int i = 0; i < dem; i++){
    //     printf("%d ", a[i]);
    // } 
    return a;
}

int main() {
    int n; 
    scanf("%d", &n);
    char s[32];
    int a[10000];
    for (int i = 0; i < n; i++) {
        scanf("%s ", s);
        check(s, 0, 2); 
    }

    // char s[100] = "23456789";
    // // printf("len: %ld\n", strlen(s));
    // check(s, 0, 5);
    printf("\n");
    return 0;
}