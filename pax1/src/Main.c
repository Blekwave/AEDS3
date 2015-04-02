#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_SIZE 51

int main(int argc, char const *argv[])
{
    char input[INPUT_SIZE];
    bool *happened = malloc(sizeof(bool) * 26);
    happened -= 'a' - 1;
    int i, num_ranges;
    while (gets(input)){
        int input_len = strlen(input);
        for (i = 'a'; i <= 'z'; i++)
            happened[i] = false;
        for (i = 0; i < input_len; i++)
            if (input[i] >= 'a' && input[i] <= 'z')
                happened[input[i]] = true;

        num_ranges = 0;
        bool prev = false;
        for (i = 'a'; i <= 'z'; i++){
            if (happened[i] && !prev){
                if (num_ranges++)
                    printf(", ");
                printf("%c:", i);
                prev = true;
            }
            else if (!happened[i] && prev){
                printf("%c", i - 1);
                prev = false;
            }
        }
        if (prev)
            printf("z");
        printf("\n");
    }
    free(happened + ('a' - 1));
    return 0;
}
