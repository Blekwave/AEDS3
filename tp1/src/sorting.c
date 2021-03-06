#include <stdio.h>
#include <stdlib.h>
#include "attack.h"
#include "heap.h"
#include "sorting.h"

// Generic swap macro
#define SWAP(x, y) do { __typeof__(x) SWAP = x; x = y; y = SWAP; } while (0)

// Number of ways of the sorting procedure
#define SORT_WAYS 17

#define MEGABYTE (1024 * 1024)

/**
 * This wrapper contains an Attack and two extra variables. 'flag' is used in
 * the sorting procedures to distinguish between Attacks that belong to diff-
 * erent input/output intermediate files. 'ins' is used to store the order of
 * insertion of elements in the heaps, so as to make it stable.
 */
typedef struct {
    Attack a;
    int flag;
    int ins;
} Wrapper;

/**
 * No WrapperBares are actually defined, but they're written and read to inter-
 * mediate files. By calling fwrite to an address that contains a regular Wrap-
 * per but only printing sizeof(WrapperBare) bytes, the program doesn't save the
 * 'ins' field, which is useless outside of the auxiliary heaps. This leads to
 * less bytes read and written to files, which improves the program's performan-
 * ce.
 */
typedef struct {
    Attack a;
    int flag;
} WrapperBare;

/**
 * This comparison function sorts two Wrappers by the following criteria:
 * - Ascending flag order
 * - Descending panzer count order
 * - Ascending distance from base order
 * - Ascending insertion in heap order
 * @param  a Wrapper to be compared
 * @param  b Wrapper to be compared
 * @return   An integer that is:
 *              - < 0 if A should come before B
 *              - = 0 if A and B are equivalent
 *              - > 0 if B should come before A
 */
int wrapperCompar(const void *a, const void *b){
    const Wrapper *A = a, *B = b;
    if (A->flag - B->flag){
        return A->flag - B->flag;
    } else if (attackCompar(a, b)){
        return attackCompar(a, b);
    } else {
        return A->ins - B->ins;
    }
}

/**
 * This comparison function sorts two Wrappers by the following criteria:
 * - Descending panzer count order
 * - Ascending distance from base order
 * - Ascending flag order
 * - Ascending insertion in heap order
 * @param  a Wrapper to be compared
 * @param  b Wrapper to be compared
 * @return   An integer that is:
 *              - < 0 if A should come before B
 *              - = 0 if A and B are equivalent
 *              - > 0 if B should come before A
 */
int attackInsCompar(const void *a, const void *b){
    if (attackCompar(a, b)){
        return attackCompar(a, b);
    } else {
        const Wrapper *A = a, *B = b;
        if (A->flag - B->flag){
            return A->flag - B->flag;
        } else {
            return A->ins - B->ins;
        }
    }
}

/**
 * Closes all input and output intermediate files.
 * @param read_files  Array of file pointers.
 * @param write_files Array of file pointers.
 * @param ways        Number of ways of the sorting operation.
 */
void closeAllFiles(FILE **read_files, FILE **write_files){
    int i;
    for (i = 0; i < SORT_WAYS; i++){
        if (read_files[i])
            fclose(read_files[i]);
        if (write_files[i])
            fclose(write_files[i]);
    }
}

/**
 * Creates a batch of tmpfiles, which are assigned to an array of files to be
 * written. The files which may have been written before are rewound and assig-
 * ned to an array of files to be read.
 * @param read_files  Files to be read.
 * @param write_files Files to be written.
 * @param ways        Number of ways of the sorting operation.
 */
void assignTmpfiles(FILE **read_files, FILE **write_files){
    int i;
    for (i = 0; i < SORT_WAYS; i++){
        if (read_files[i])
            fclose(read_files[i]);

        read_files[i] = write_files[i];

        if (read_files[i])
            rewind(read_files[i]);
        
        write_files[i] = tmpfile();
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
int initialDistribution(char *addr, FILE **write_files, int available_mem){
    // Initial pass
    Attack r_cache;
    Wrapper w_cache;

    unsigned long long heap_len =
        (((unsigned long long)available_mem) * MEGABYTE) / sizeof(Wrapper);
    Heap *h = hInit(sizeof(Wrapper), heap_len, wrapperCompar);
    if (!h){
        fprintf(stderr, "ERROR: Not enough memory. (1)\n");
        exit(1);
    }
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

    // Reads remaining elements from the input
    while (fread(&r_cache, sizeof(Attack), 1, input)){
        hPop(h, &w_cache);
        fwrite(&w_cache, sizeof(WrapperBare), 1, write_files[w_cache.flag % SORT_WAYS]);
        if (attackCompar(&r_cache, &w_cache.a) < 0){
            w_cache.flag++; // If the newly read attack should be placed before
                            // the one that's been written, it belongs to the
                            // next block.
        }
        w_cache.a = r_cache;
        w_cache.ins = ins;
        hInsert(h, &w_cache);
        ins++;
    }

    // Writes the remaining elements in the heap to the write files.
    while (hNum(h) > 0){
        hPop(h, &w_cache);
        fwrite(&w_cache, sizeof(WrapperBare), 1, write_files[w_cache.flag % SORT_WAYS]);
    }

    hDelete(h);
    fclose(input);
    return w_cache.flag + 1; // Number of sorted blocks
}

/**
 * Merges the sorted blocks in the read files into larger blocks, which are,
 * then, stored in the write files. Should be called repeatedly until a single
 * block remains.
 * @param  read_files  Array of file pointers where the blocks to be read are.
 * @param  write_files Array of file pointers where the new blocks should be
 *                     written.
 * @param  ways        Number of ways of the sorting operation.
 * @return             Number of new blocks created.
 */
int multiwayMerge(FILE **read_files, FILE **write_files){
    Wrapper r_cache, w_cache;
    Heap *h = hInit(sizeof(Wrapper), SORT_WAYS, attackInsCompar);
    if (!h){
        fprintf(stderr, "ERROR: Not enough memory. (2)\n");
        exit(2);
    }
    int ins = 0; // Insertion order tracker

    // Adds an element from each read file to the heap.
    int i;
    for (i = 0; i < SORT_WAYS; i++){
        if (fread(&r_cache, sizeof(WrapperBare), 1, read_files[i])){
            r_cache.ins = ins;
            hInsert(h, &r_cache);
            ins++;
        }
    }

    // Initializes an auxiliary heap, into which the attacks that belong to the
    // next row of blocks will be inserted.
    Heap *next = hInit(sizeof(Wrapper), SORT_WAYS, attackInsCompar);
    if (!h){
        fprintf(stderr, "ERROR: Not enough memory. (3)\n");
        exit(3);
    }
    int ins_next = 0;

    int cur_file = 0;
    while (hNum(h)){
        while (hNum(h)){
            hPop(h, &w_cache);
            if (fread(&r_cache, sizeof(WrapperBare), 1,
                      read_files[w_cache.flag % SORT_WAYS])){
                if (r_cache.flag == w_cache.flag){
                    // The attack which was read belongs to the same block as
                    // the one which was removed.
                    r_cache.ins = ins;
                    hInsert(h, &r_cache);
                    ins++;
                } else {
                    // The attack which was read belongs to the next block row.
                    r_cache.ins = ins_next;
                    hInsert(next, &r_cache);
                    ins_next++;
                }
            }
            w_cache.flag = cur_file;
            fwrite(&w_cache, sizeof(WrapperBare), 1,
                   write_files[cur_file % SORT_WAYS]);
        }
        // Swaps the current heap with the auxiliary heap, which effectively
        // begins the operation in the next block row.
        SWAP(h, next);
        ins = ins_next;
        ins_next = 0;
        cur_file++;
    }

    hDelete(h);
    hDelete(next);
    return cur_file;
}

/**
 * Moves all Attacks after finishing the sorting operation from the last inter-
 * mediate write file to the original input file.
 * @param addr     Address of the original file.
 * @param w_prefix Prefix of the write files.
 */
void saveToAddr(char *addr, FILE *input){
    FILE *output = fopen(addr, "wb");
    WrapperBare cache;
    while (fread(&cache, sizeof(WrapperBare), 1, input)){
        fwrite(&cache, sizeof(Attack), 1, output);
    }
    fclose(output);
}

/**
 * Sorts a file which contains a list of Attacks through an external mergesort
 * performed with an auxiliary heap.
 * @param addr          Address of the file to be sorted.
 * @param filesize      Size, in bytes, of the file to be sorted.
 * @param available_mem Amount of memory available to the cache heap, in MB.
 * @param ways          Number of ways of the sorting operation.
 * @param r_prefix      Prefix to be used in the intermediate read files.
 * @param w_prefix      Prefix to be used in the intermediate write files.
 */
void sortAttacks(char *addr, size_t filesize, int available_mem){
    FILE *read_files[SORT_WAYS];
    FILE *write_files[SORT_WAYS];

    int i;
    for (i = 0; i < SORT_WAYS; i++){
        read_files[i] = write_files[i] = 0;
    }

    assignTmpfiles(read_files, write_files);
    int blocks = initialDistribution(addr, write_files, available_mem);

    while (blocks > 1){
        assignTmpfiles(read_files, write_files);
        blocks = multiwayMerge(read_files, write_files);
    }

    rewind(write_files[0]);
    saveToAddr(addr, write_files[0]);
    closeAllFiles(read_files, write_files);
}
