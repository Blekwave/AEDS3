#include "proc.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    char texto[1000];
    char padrao[1000];
    gets(texto);
    gets(padrao);
    printf("%d\n", kmp(texto, padrao));
    return 0;
}