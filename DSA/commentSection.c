#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    int N;
    char keyword[100];

    scanf("%d", &N);
    scanf("%s", keyword);

    // Tạo một biến đếm để đếm số lượng comment chứa keyword
    int count = 0;

    // Duyệt qua từng comment
    char comment[100];
    for (int i = 0; i < N; i++) {
        getchar(); 
        fgets(comment, sizeof(comment), stdin);
  
        // Loại bỏ kí tự xuống dòng
        comment[strcspn(comment, "\n")] = '\0';
        
        for (int j = 0; j < strlen(comment); j++) {
            comment[j] = tolower(comment[j]);
        }
        
        for (int j = 0; j < strlen(keyword); j++) {
            keyword[j] = tolower(keyword[j]);
        }

        // Nếu comment chứa keyword thì tăng biến đếm lên 1
        if (strstr(comment, keyword) != NULL) {
            count++;
        }
    }

    printf("%d\n", count);

    return 0;
}
