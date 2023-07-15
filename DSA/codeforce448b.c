#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void sort(char s[]){
	int i, j;
	char temp;
	for (i = 0; i < strlen(s); i++){
		for (j = 0; j < (strlen(s)-1); j++){
			if (s[j] > s[j+1]){
				temp = s[j];
				s[j] = s[j+1];
				s[j+1] = temp;
			}
		}
	}
}

bool array(char a[], char b[]){
	sort(a);
	sort(b);
	if (strcmp(a,b) == 0) return true;
	return false; 
}

bool automation(char a[], char b[]){
	int n = 0, i;
	for (i = 0; i < strlen(a); i++){
		if(a[i] == b[n]){
			n++;
		}
		if (n > strlen(b))	return false;
	}
	if ( n != strlen(b)) return false;
	return true;
}

bool both (char a[], char b[]){
	int i, j, p = -1;
	for (i = 0; i < strlen(b); i++){
		for (j = 0; j < strlen(a); j++){
			if (a[j] == b[i]){
				p = j; 
				break;
			}
		}
		if (p == -1) return false;
		a[p] = '0';
	}
	return true;
}

int main(){
	char a[100], b[100];
	scanf("%s %s", &a, &b);
	if (array(a,b)) printf("array\n");
	else if (automation(a,b)) printf("automation\n");
	else if (both(a,b)) printf("both");
	else printf("need tree");

	return 0;
}