#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, q; // size of array and number of queries
    scanf("%d %d", &n, &q);

    int **arr = (int **)malloc(n * sizeof(int *)); // 2-dimensional array of n empty arrays

    for (int i = 0; i < n; i++) {
        arr[i] = NULL; // initialize each array as empty
    }

    int lastAnswer = 0;

    // array to store the result of each query of type 2
    int *answers = (int *)malloc(q * sizeof(int));

    for (int i = 0; i < q; i++) {
        int type, x, y;
        scanf("%d %d %d", &type, &x, &y);

        idx idx = (x + lastAnswer) % n;

        if (type == 1) {
            int currentSize = 0;
            if (arr[idx] != NULL) {
                // find the current size of the array
                while (arr[idx][currentSize] != 0) {
                    currentSize++;
                }
            }

            arr[idx] = (int *)realloc(arr[idx], (currentSize + 1) * sizeof(int));
            arr[idx][currentSize] = y; // append the integer y to arr[idx]
            arr[idx][currentSize + 1] = 0; // add a sentinel value 0 at the end
        } else if (type == 2) {
            int elementIdx = y % (sizeof(arr[idx]) / sizeof(arr[idx][0]));
            lastAnswer = arr[idx][elementIdx];
            answers[i] = lastAnswer;
            printf("%d\n", answers[i]);
        }
    }

    // Free memory
    for (int i = 0; i < n; i++) {
        free(arr[i]);
    }
    free(arr);
    free(answers);

    return 0;
}
