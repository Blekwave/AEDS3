#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "attack.h"
#include "sorting.h"
#include "msort.h"

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
    int max_pos = 0, i;
    for (i = 1; i < ways; i++)
        if (attackCompar(&cache[max_pos], &cache[i]) > 0)
            max_pos = i;
    return max_pos;
}

void loadSortAndSave(char *addr, size_t filesize){
    Attack *cache = malloc(filesize);
    FILE *input = fopen(addr, "rb");
    int items_read = fread(cache, sizeof(Attack), filesize / sizeof(Attack),
                           input);
    fclose(input);
    msort(cache, items_read, sizeof(Attack), attackCompar);
    FILE *output = fopen(addr, "wb");
    fwrite(cache, items_read, sizeof(Attack), output);
    fclose(output);
}

static int mergePass(FILE **read_files, FILE *write_file, Attack *cache,
    int ways, int *way_pos, int items_in_block){
    int ways_done = 0, items_written = 0;
    int i;
    // Read first element of each block in column to cache
    for (i = 0; i < ways; i++){
        if (!fread(&cache[i], sizeof(Attack), 1, read_files[i])){
            way_pos[i] = items_in_block;
            cache[i].panzers = DONE_WITH_WAY_FLAG;
            ways_done++;
        } else {
            way_pos[i] = 0;
        }
    }
    while (ways_done < ways){
        int max_pos = getMaxIndex(cache, ways);
        // Write item in max priority way to current write file
        fwrite(&cache[max_pos], sizeof(Attack), 1,
               write_file);
        items_written++;
        // Increment count of items read from each way
        way_pos[max_pos]++;
        if (way_pos[max_pos] < items_in_block){
            if (!fread(&cache[max_pos], sizeof(Attack), 1,
                read_files[max_pos])){
                way_pos[max_pos] = items_in_block;
                cache[max_pos].panzers = DONE_WITH_WAY_FLAG;
                ways_done++;
            }
        }
        else {
            cache[max_pos].panzers = DONE_WITH_WAY_FLAG;
            ways_done++;
        }
    }
    return items_written;
}

void sortAttackList(char *addr, size_t filesize, size_t available_mem,
    int ways){
    if (filesize <= available_mem){
        loadSortAndSave(addr, filesize);
        return;
    }

    int items_in_block = available_mem / (sizeof(Attack) * 2); // 2: merge sort compensation
    int cache_length = items_in_block;
    int items_in_file = filesize / sizeof(Attack);

    Attack *cache = malloc(cache_length * sizeof(Attack));
    
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
        msort(cache, items_read, sizeof(Attack), attackCompar);
        fwrite(cache, sizeof(Attack), items_read, write_files[cur_wr_file]);
        cur_wr_file = (cur_wr_file + 1) % ways;
    }
    
    fclose(input);

    closeWriteFiles(write_files, ways);
    openAllFiles(read_files, write_files, ways);

    int *way_pos = malloc(sizeof(int) * ways);
    cur_wr_file = 0;

    while (items_in_block < items_in_file){
        int items_written = 0;
        while (items_written < items_in_file){            
            items_written += mergePass(read_files, write_files[cur_wr_file],
                                       cache, ways, way_pos, items_in_block);
            cur_wr_file = (cur_wr_file + 1) % ways;
        }
        items_in_block *= ways;
        cur_wr_file = 0;

        closeAllFiles(read_files, write_files, ways);
        openAllFiles(read_files, write_files, ways);
    }

    FILE *output = fopen(addr, "wb");
    while ((items_read = fread(cache, sizeof(Attack), cache_length,
            read_files[0]))){
        fwrite(cache, sizeof(Attack), items_read, output);
    }
    fclose(output);

    closeAllFiles(read_files, write_files, ways);

    free(cache);
    free(read_files);
    free(write_files);
    free(way_pos);
}