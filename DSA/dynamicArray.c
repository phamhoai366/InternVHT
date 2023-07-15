#include <stdio.h>
#include <stdlib.h>
int i, j;

int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int findGCD(int arr[], int n) {
    int result = arr[0];

    for (i = 1; i < n; i++) {
        result = gcd(result, arr[i]);
    }
    return result;
}


int main() {
    int Q;
    scanf("%d", &Q);

    int* arr = NULL;
    int size = 0;

    while (Q--) {
        int K;
        scanf("%d", &K);

        if (K == 1) {
            int x;
            scanf("%d", &x);

            size++;
            arr = realloc(arr, size * sizeof(int));
            arr[size - 1] = x;
        } else if (K == 2) {
            int x;
            scanf("%d", &x);

            int i = 0;
            while (i < size && arr[i] != x)
                i++;

            if (i < size) {
                for (j = i; j < size - 1; j++)
                    arr[j] = arr[j + 1];
                size--;
                arr = realloc(arr, size * sizeof(int));
            }
        } else if (K == 3) {
            int x;
            scanf("%d", &x);

            if (x <= size) {
                int* temp = malloc(size * sizeof(int));
                for (i = 0; i < size; i++){
                    temp[i] = arr[i];
                }

                for (i = 0; i < size - 1; i++) {
                    for (j = 0; j < size - i - 1; j++) {
                        if (temp[j] > temp[j + 1]) {
                            int tempVal = temp[j];
                            temp[j] = temp[j + 1];
                            temp[j + 1] = tempVal;
                        }
                    }
                }
                printf("%d\n", temp[x - 1]);
                free(temp);
            }
        } else if (K == 4) {
            int L, R;
            scanf("%d %d", &L, &R);

			int count = 0;
			int* temp = malloc(size * sizeof(int));
			for (i = 0; i < size; i++){
				if(arr[i] >= L && arr[i] <= R){
					temp[i] = arr[i];
					count++;
				}
			}

			int gcdResult = findGCD(temp, count);
            printf("%d\n", gcdResult);
        }
    }
    free(arr);
    return 0;
}
