#include <stdio.h>
#include <string.h>

#define STR_SIZE 105 // Actually 50, but forum posts say it's safer this way
                     // http://www.urionlinejudge.com.br/forum/viewtopic.php?f=4&t=239

int largest_substr(char *a, char *b){
    int len_a = strlen(a), len_b = strlen(b), max = 0, i, j;
    for (i = 0; i < len_a; i++){
        for (j = 0; j < len_b; j++){
            int length = 0;
            while ((i + length) < len_a && (j + length) < len_b &&
                a[i + length] == b[j + length]){
                length++;
            }
            if (length > max)
                max = length;
        }
    }
    return max;
}

int main(int argc, char const *argv[])
{
    char a[STR_SIZE + 1], b[STR_SIZE + 1];
    while (fgets(a, STR_SIZE, stdin)){
        fgets(b, STR_SIZE, stdin);
        char *a_lfskip = strtok(a, "\n");
        char *b_lfskip = strtok(b, "\n");
        printf("%d\n", largest_substr(a_lfskip, b_lfskip));
    }
    return 0;
}