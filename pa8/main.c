#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100002

int main(int argc, char const *argv[])
{
    int n, d;
    char buffer[BUFFER_SIZE];
    scanf("%d %d", &n, &d);
    while (n){
        getchar();
        fgets(buffer, n + 2, stdin);
        int i = 0, deleted = 0, len = 0;
        while (deleted < d && i < n){
            while (deleted < d && len > 0 && buffer[len - 1] < buffer[i]){
                deleted++;
                len--;
            }
            if (len < n - d){
                buffer[len] = buffer[i];
                len++;
            }
            i++;
        }
        memmove(buffer + len, buffer + i, n - i * sizeof(char));
        buffer[n - d] = '\0';
        printf("%s\n", buffer);
        scanf("%d %d", &n, &d);
    }
    return 0;
}