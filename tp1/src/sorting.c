#include <stdio.h>
#include <stdlib.h>
#include "attack.h"
#include "heap.h"
#include "sorting.h"

#define SWAP(x, y) do { __typeof__(x) SWAP = x; x = y; y = SWAP; } while (0)
#define FILENAME_BFSIZE 128

typedef struct {
    Attack a;
    int flag;
    int ins;
} Wrapper;

typedef struct {
    Attack a;
    int flag;
} WrapperBare;

long long wrapperCompar(const void *a, const void *b){
    int t;
    return (t = ((Wrapper *)a)->flag - ((Wrapper *)b)->flag) ? t :
           (t = attackCompar(a, b)) ? t :
           (((Wrapper *)a)->ins - ((Wrapper *)b)->ins);
}

long long attackInsCompar(const void *a, const void *b){
    int t;
    return (t = attackCompar(a, b)) ? t :
           (t = ((Wrapper *)a)->flag - ((Wrapper *)b)->flag) ? t :
           (((Wrapper *)a)->ins - ((Wrapper *)b)->ins);
}


void closeAllFiles(FILE **read_files, FILE **write_files, int ways){
    int i;
    for (i = 0; i < ways; i++){
        if (read_files[i])
            fclose(read_files[i]);
        if (write_files[i])
            fclose(write_files[i]);
    }
}

void openAllFiles(FILE **read_files, char *r_prefix,
                   FILE **write_files, char *w_prefix, int ways){
    int i, prefix_len;
    char buffer[FILENAME_BFSIZE];
    sprintf(buffer, "%s", r_prefix);
    prefix_len = strlen(r_prefix);
    for (i = 0; i < ways; i++){
        sprintf(buffer + prefix_len, "%04d.tmp", i);
        read_files[i] = fopen(buffer, "rb");
    }
    sprintf(buffer, "%s", w_prefix);
    prefix_len = strlen(w_prefix);
    for (i = 0; i < ways; i++){
        sprintf(buffer + prefix_len, "%04d.tmp", i);
        write_files[i] = fopen(buffer, "wb");
    }
}

/**
 * Reads the attacks from a file and splits them into sorted blocks, which are,
 * then, put into files together with information about which block they belong
 * to.
 * @param  addr        String containing the address of the input file.
 * @param  write_files Array of open files in which the blocks shall be written.
 * @param  ways        Number of ways of the sorting procedure.
 * @return             Number of blocks generated.
 *
 * Information about the data in the new temporary files:
 * Each block of attacks contains Wrappers, which wrap the attacks themselves
 * and the identifier of the block they're in. Due to the fact that the blocks
 * are written sequentially to the files, this means that all blocks in the file
 * with index I have id I mod WAYS. e.g. (3 ways):
 * File 0: Blocks 0, 3, 6
 * File 1: Blocks 1, 4
 * File 2: Blocks 2, 5
 * This fact is useful for better comprehension of additionalPass().
 */
int initialPass(char *addr, FILE **write_files, size_t available_mem, int ways){
    // Initial pass
    Attack r_cache;
    Wrapper w_cache;

    int heap_len = available_mem / sizeof(Wrapper);
    //heap_len = 2;
    Heap *h = hInit(sizeof(Wrapper), heap_len, wrapperCompar);
    int ins = 0;
    FILE *input = fopen(addr, "rb");

    // Fills the heap with the first elements of the file
    w_cache.flag = 0;
    while (hNum(h) < heap_len && fread(&r_cache, sizeof(Attack), 1, input)){
        w_cache.a = r_cache;
        w_cache.ins = ins;
        hInsert(h, &w_cache);
        ins++;
    }

    // Reads remaining elements
    while (fread(&r_cache, sizeof(Attack), 1, input)){
        hPop(h, &w_cache);
        // printf("Inserting %d: %d,%d in %d\n", w_cache.a.panzers, w_cache.a.x, w_cache.a.y, w_cache.flag);
        fwrite(&w_cache, sizeof(WrapperBare), 1, write_files[w_cache.flag % ways]);
        if (attackCompar(&r_cache, &w_cache) < 0)
            w_cache.flag++; // Next block.
        w_cache.a = r_cache;
        w_cache.ins = ins;
        hInsert(h, &w_cache);
        ins++;
    }

    while (hNum(h) > 0){
        hPop(h, &w_cache);
        fwrite(&w_cache, sizeof(WrapperBare), 1, write_files[w_cache.flag % ways]);
    }

    hDelete(h);
    fclose(input);
    return w_cache.flag + 1; // Number of sorted blocks
}

int additionalPass(FILE **read_files, FILE **write_files, int ways){
    Wrapper r_cache, w_cache;
    Heap *h = hInit(sizeof(Wrapper), ways, attackInsCompar);
    int ins = 0;

    int i;
    for (i = 0; i < ways; i++){
        if (fread(&r_cache, sizeof(WrapperBare), 1, read_files[i])){
            r_cache.ins = ins;
            hInsert(h, &r_cache);
            ins++;
        }
    }

    Heap *next = hInit(sizeof(Wrapper), ways, attackInsCompar);
    int ins_next = 0;

    int cur_file = 0;
    while (hNum(h)){
        while (hNum(h)){
            hPop(h, &w_cache);
            if (fread(&r_cache, sizeof(WrapperBare), 1,
                      read_files[w_cache.flag % ways])){
                if (r_cache.flag == w_cache.flag){
                    r_cache.ins = ins;
                    hInsert(h, &r_cache);
                    ins++;
                } else {
                    r_cache.ins = ins_next;
                    hInsert(next, &r_cache);
                    ins_next++;
                }
            }
            w_cache.flag = cur_file;
            fwrite(&w_cache, sizeof(WrapperBare), 1,
                   write_files[cur_file % ways]);
        }
        SWAP(h, next);
        ins = ins_next;
        ins_next = 0;
        cur_file++;
    }

    hDelete(h);
    hDelete(next);
    return cur_file;
}

void saveToAddr(char *addr, char *w_prefix, size_t available_mem, int ways){
    char filename_buffer[FILENAME_BFSIZE];
    sprintf(filename_buffer, "%s%04d.tmp", w_prefix, 0);
    FILE *input = fopen(filename_buffer, "rb");
    FILE *output = fopen(addr, "wb");
    WrapperBare cache;
    while (fread(&cache, sizeof(WrapperBare), 1, input)){
        fwrite(&cache, sizeof(Attack), 1, output);
    }
    fclose(input);
    fclose(output);
}

void sortAttackList(char *addr, size_t filesize, size_t available_mem,
    int ways, char *r_prefix, char *w_prefix){
    FILE **read_files = calloc(sizeof(FILE *), ways);
    FILE **write_files = calloc(sizeof(FILE *), ways);

    openAllFiles(read_files, r_prefix, write_files, w_prefix, ways);
    int blocks = initialPass(addr, write_files, available_mem, ways);
    closeAllFiles(read_files, write_files, ways);

    while (blocks > 1){
        SWAP(r_prefix, w_prefix);
        openAllFiles(read_files, r_prefix, write_files, w_prefix, ways);
        blocks = additionalPass(read_files, write_files, ways);
        closeAllFiles(read_files, write_files, ways);
    }

    saveToAddr(addr, w_prefix, available_mem, ways);

    free(read_files);
    free(write_files);
}
