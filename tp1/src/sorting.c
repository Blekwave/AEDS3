#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include "attack.h"
#include "sorting.h"

#define FILENAME_BFSIZE 128

#define DONE_WITH_WAY_FLAG -1

static char pref_write = 'a';
static char pref_read = 'b';

void openAllFiles(FILE **read_files, FILE **write_files, int ways){
    char temp = pref_write;
    pref_write = pref_read;
    pref_read = temp;
    char buffer[FILENAME_BFSIZE];
    int i;
    for (i = 0; i < ways; i++){
        sprintf(buffer, "%c%03d.temp", pref_read, i);
        read_files[i] = fopen(buffer, "rb");
        buffer[0] = pref_write;
        write_files[i] = fopen(buffer, "wb");
    }
}

void closeAllFiles(FILE **read_files, FILE **write_files, int ways){
    int i;
    for (i = 0; i < ways; i++){
        fclose(read_files[i]);
        fclose(write_files[i]);
    }
}

void openWriteFiles(FILE **write_files, int ways){
    char buffer[FILENAME_BFSIZE];
    int i;
    for (i = 0; i < ways; i++){
        sprintf(buffer, "%c%03d.temp", pref_write, i);
        write_files[i] = fopen(buffer, "wb");
    }
}

void closeWriteFiles(FILE **write_files, int ways){
    int i;
    for (i = 0; i < ways; i++){
        fclose(write_files[i]);
    }
}

int getMaxIndex(Attack *cache, int ways){
    int max_pos;
    max_pos = --ways;
    while (ways--)
        if (attackCompar(&cache[ways], &cache[max_pos]) > 0)
            max_pos = ways;
    return max_pos;
}

void sortAttackList(char *addr, size_t filesize, size_t available_mem,
    int ways){
    int items_in_block = available_mem / sizeof(Attack);
    int items_in_file = filesize / sizeof(Attack);

    Attack *cache = malloc(items_in_block * sizeof(Attack));
    FILE **read_files = malloc(sizeof(FILE *) * ways);
    FILE **write_files = malloc(sizeof(FILE *) * ways);

    openWriteFiles(write_files, ways);

    // First pass: loads mem-sized blocks to the cache, sorts them and outputs
    // them sequentially to each output file
    FILE *input = fopen(addr, "rb");
    int items_read;
    int cur_wr_file = 0;
    while ((items_read = fread(cache, sizeof(Attack), items_in_block,
            input))){
        qsort(cache, items_read, sizeof(Attack), attackCompar);
        fwrite(cache, sizeof(Attack), items_read, write_files[cur_wr_file]);
        cur_wr_file = (cur_wr_file + 1) % ways;
    }
    fclose(input);

    closeWriteFiles(write_files, ways);
    openAllFiles(read_files, write_files, ways);

    int *items_read_in_way = malloc(sizeof(int) * ways);
    cur_wr_file = 0;

    while (items_in_block < items_in_file){
        int items_read_in_pass = ways;
        while (items_read_in_pass < items_in_file){
            int i;
            for (i = 0; i < ways; i++){ // Read first element of each block in column to cache
                fread(&cache[i], sizeof(Attack), 1, read_files[i]);
                items_read_in_way[i] = 0;
            }
            
            int finished_ways = 0;
            while (finished_ways < ways){
                int max_pos = getMaxIndex(cache, ways);
                // Write item in max priority way to current write file
                fwrite(&cache[max_pos], sizeof(Attack), 1,
                       write_files[cur_wr_file]);
                items_read_in_pass++;

                // Increment count of items read from each way
                items_read_in_way[max_pos]++;
                if (items_read_in_way[max_pos] < items_in_block){
                    if (!fread(&cache[max_pos], sizeof(Attack), 1,
                        read_files[max_pos])){
                        items_read_in_way[max_pos] = items_in_block;
                        cache[max_pos].panzers = DONE_WITH_WAY_FLAG;
                        finished_ways++;
                    }
                }
                else {
                    cache[max_pos].panzers = DONE_WITH_WAY_FLAG;
                    finished_ways++;
                }
            }
            cur_wr_file = (cur_wr_file + 1) % ways;
        }
        items_in_block *= ways;
        cur_wr_file = 0;

        closeAllFiles(read_files, write_files, ways);
        openAllFiles(read_files, write_files, ways);
    }

    FILE *output = fopen(addr, "wb");
    while ((items_read = fread(cache, sizeof(Attack), items_in_file,
            read_files[0]))){
        fwrite(cache, items_read, sizeof(Attack), output);
    }
    fclose(output);

    closeAllFiles(read_files, write_files, ways);

    free(cache);
    free(read_files);
    free(write_files);
    free(items_read_in_way);
}