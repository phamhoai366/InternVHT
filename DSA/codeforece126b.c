#include <stdio.h>
#include <string.h>

char s[100];
int nex[100], i;

// KMP algorithm
void find_nex(int m){
	int i,j;
	j = 0, nex[1] = 0;
	i = 2;
	while (i <= m){
		while (j != 0 && s[j+1] != s[i]){
			j = nex[j];
		}
		if(s[j+1] == s[i]){
			j++;
		}
		nex[i++] = j;
	}
}

int num[100];
void solve(){
	int n = strlen(s+1);
	find_nex(n);
	for(i = n; i > 0; i--){
		num[i]++;
		num[nex[i]] += num[i];
	}
	int l = 0;
	for( i = n; i > 0; i = nex[i]){
		if (num[i] >= 3){
			l = i;
			break;
		}
	}
	
	if (l == 0){
		printf("Just a legend\n");
	} else{
		for(i = 1; i <= l; i++){
			printf("%c", s[i]);
//			printf("\n");
		}
	}
	return ;
}


int main(){
	printf("Nhap s: ");
	scanf("%s", s+1);
	solve();
	return 0;
}

