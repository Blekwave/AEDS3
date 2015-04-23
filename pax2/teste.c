#include <stdio.h>

void quick_read(int *acc){
    *acc = 0;
    char c = getchar();
    while (c >= '0' && c <= '9'){
        *acc = (*acc << 1) + (*acc << 3) + c - '0';
        c = getchar();
    }
}

int main(int argc, char const *argv[])
{
    int a, b;
    quick_read(&a);
    quick_read(&b);
    printf("%d %d\n", a, b);
    return 0;
}