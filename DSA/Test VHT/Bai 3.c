#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int checkdoixung(char *s) {
    int len = strlen(s);
    for (int i = 0; i < len / 2; i++) {
        if (s[i] != s[len - i - 1]) {
            return 0; 
        }
    }
    return 1; 
}

int check(char *s){
    int n = strlen(s);
    int count = 0;
    if (checkdoixung(s) == 1){
        count = 0;
    }

    for (int i = 0; i < n/2; i++){
        while (s[i] != s[n-i-1]){
            if (s[i] > s[n-i-1]){
                s[i] -= 1;
            } 
            else {
                s[n-i-1] -= 1;
            }
            count += 1;
        }
    }
    return count;
}

int main(){
    int n, result;
    scanf("%d", &n);

    for(int i = 0; i < n; i++){
        char s[100];
        scanf("%s", s);
        result = check(s);
        printf("%d\n", result);
    }
    return 0; 
}

//15
