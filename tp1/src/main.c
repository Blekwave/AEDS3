#include <stdio.h>
#include <stdlib.h>
#include "extern.h"
#include "parser.h"
#include "sorting.h"
#include "attack.h"

#define TEMP_FILE "unordered.bin"
#define RESERVED_MEM 1 // 1MB

int base_x, base_y;

char *r_prefix = "A";
char *w_prefix = "B";

int main(int argc, char const *argv[])
{
    int max_mem;
    scanf("%d\n%d %d", &max_mem, &base_x, &base_y);
    size_t filesize = parseData(TEMP_FILE);
    sortAttackList(TEMP_FILE, filesize, max_mem - RESERVED_MEM);
    unpackData(TEMP_FILE);
    return 0;
}