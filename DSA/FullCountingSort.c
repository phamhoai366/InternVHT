#include <stdio.h>
#include <string.h>

#define MAXN 1000000
#define MAXNUM 100
#define MAXSTR 10

int main() {
    
    long i, j, n, num[MAXN];
    static char str[MAXN/2][MAXSTR];
    scanf("%ld", &n);
    
    for ( i = 0; i < n/2; i++ ) {
        scanf("%ld", &num[i]);
        scanf("%*s");
    }
    
    for ( i = n/2; i < n; i++ ) {
        scanf("%ld", &num[i]);
        scanf("%s", str[i-n/2]);
    }
    
    for ( i = 0; i < MAXNUM; i++ ) {
        for ( j = 0; j < n/2; j++ ) {
            if ( num[j] == i ) {
                printf("- ");
            }
        }
        for ( j = n/2; j < n; j++ ) {
            if ( num[j] == i ) {
                printf("%s ", str[j-n/2]);
            }
        }
    }
    
    return 0;
}