#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

#define TEMP_FILE "unordered.bin"
#define RESERVED_MEM 1048576 // 1MB

int main(int argc, char const *argv[])
{
    int max_mem;
    int base_x, base_y;
    scanf("%d\n%d %d", &max_mem, &base_x, &base_y);
    parseData(TEMP_FILE, base_x, base_y);
    unpackData(TEMP_FILE);
    return 0;
}