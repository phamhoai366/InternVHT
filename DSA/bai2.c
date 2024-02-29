#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int check(int a[], int b[], int n){
    int dem = 0;
    int count;
    for (int i = 0; i < n; i++){
            int count = 0;
        for(int j = 0; j < i; j++){            
            if(a[i] < a[j]){
                count += 1;
            }      
        }
        if(count < b[i]){
                swap(&a[i], &a[i+1]);
                swap(&b[i], &b[i+1]);
                dem += 1;
            }
        else if(count > b[i]){
            swap(&a[i], &a[i-1]); 
            swap(&b[i], &b[i-1]);
            dem += 1;             
        }
        else {
            continue;
        }
    }
    return dem;
}

int main(){
    int n;
    scanf("%d", &n);
    int a[n], b[n];

    for (int i = 0; i < n; i++){
        scanf("%d", &a[i]);
    }
    for (int i = 0; i < n; i++){
        scanf("%d", &b[i]);
    }

    while(check(a, b, n) != 0){
        for(int i = 0; i < n; i++){
            printf("%d ", a[i]);
        }
        printf("\n");
    }
    
    // check(a, b, n);
    // for(int i = 0; i < n; i++){
    //     printf("%d ", a[i]);
    // }

    printf("\n");
    return 0;
}


