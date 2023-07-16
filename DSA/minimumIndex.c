#include<stdio.h>
#include <stdlib.h>

int i;

int tinhTong(int n){
	int sum = 0, c;
	while(n > 0){
		c = n % 10;
		sum += c;
		n = n / 10;
	}
	return sum;
}

int main(){
    int n,q;
    scanf("%d %d", &n, &q);

    int A[n+1], F[n+1];

    for(i = 1; i <= n; i++){
    	int d;
        scanf("%d", &d);
        A[i] = d;
        F[i] = tinhTong(A[i]);
    }

    while(q--){
        int x;
        scanf("%d", &x);
        int flag = 0,a = 0;

        for(i = x+1; i<=n; i++){
            if(A[x] < A[i] && F[x] > F[i]){
                flag = 1;
                a = i;
                break;
            }
        }

        if(flag == 0){
  	        printf("-1\n");
        }
        else{
            printf("%d\n", a);
        }
    }
    
    return 0;
}