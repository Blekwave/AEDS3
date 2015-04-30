#include <stdio.h>
#include <stdlib.h>
#include "extern.h"
#include "parser.h"
#include "sorting.h"
#include "attack.h"

#define TEMP_FILE "unordered.bin"
#define RESERVED_MEM 1048576 // 1MB
#define SORT_WAYS 8

int base_x, base_y;

char *r_prefix = "tmp\\A";
char *w_prefix = "tmp\\B";

int main(int argc, char const *argv[])
{
    int max_mem;
    scanf("%d\n%d %d", &max_mem, &base_x, &base_y);
    size_t filesize = parseData(TEMP_FILE);
    sortAttackList(TEMP_FILE, filesize, (max_mem << 20) - RESERVED_MEM,
        SORT_WAYS, r_prefix, w_prefix);
    unpackData(TEMP_FILE);
    return 0;
}