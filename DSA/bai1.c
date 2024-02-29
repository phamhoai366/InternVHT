#include<stdio.h>
#include<string.h>

int main(){
    int n, a[100]; 
    scanf("%d", &n);
    for (int i = 0; i < n; i++){
        int b;
        scanf("%d", &b);
    }

    int sum = 0, count = 0;
    for (int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(a[i] <= a[j]){
                count += 1;
            }
            sum += count; 
            count = 0;
        }
    }
    printf("%d", sum);
    return 0;
}

//5