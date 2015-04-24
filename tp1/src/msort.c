#include "msort.h"
#include <stdlib.h>

/**
 * Performs a merge sort in an array. Merge sort is a stable sort that runs in
 * O(n log n).
 * @param base   The array to be sorted.
 * @param num    Number of elements in the array.
 * @param size   Size of each element in the array.
 * @param compar Function that takes as its parameters the address of two ele-
 *               ments and determines how to place them in the right order.
 *               @param  a Address of the first element.
 *               @param  b Address of the second element.
 *               @return   Integer value r so that if:
 *                                 r < 0: a should be placed before b
 *                                 r == 0: order doesn't matter
 *                                 r > 0: a should be placed after b
 */
void msort(void *base, size_t num, size_t size,
           int (*compar)(const void *a, const void *b)){
    // An auxiliary array with equal size to the input array is used to store
    // the output.
    void *aux_array = malloc(size * num);
    void *r_first = base, *w_first = aux_array;
    int bl_len = 1;

    // Iterates through the array, sorting it by dividing it in bl_len-sized
    // blocks and merging them with an adjacent block. bl_len is doubled in
    // every iteration, until it becomes larger than the number of elements in
    // the array.
    while (bl_len < num){
        void *r = r_first; // Address of the first element of the block pair
        void *w = w_first; // Keeps track of where to write the next element
        int whole_bpairs = num / (bl_len * 2);
        int cur_bpair = 0, even_pos, odd_pos;

        // Passes by every whole block pair. A block pair, in this algorithm,
        // means a chunk with bl_len * 2 values that's properly aligned: its
        // first element's index is 0 mod (bl_len * 2).
        while (cur_bpair < whole_bpairs){
            // In a block pair, there are two bl_len-sized blocks. The even
            // block is the one that comes first. It's followed by the odd block.
            even_pos = 0, odd_pos = 0;
            // This loop takes the smallest element from the block pair by ana-
            // lyzing the first element of each block.
            while (even_pos < bl_len && odd_pos < bl_len){
                if (compar(r + size * even_pos, r + size * (bl_len + odd_pos)) <= 0){
                    memcpy(w, r + size * even_pos, size);
                    even_pos++;
                } else {
                    memcpy(w, r + size * (bl_len + odd_pos), size);
                    odd_pos++;
                }
                w += size;
            }
            // When one of the position trackers gets to bl_len, all of the
            // block's elements have already been written. Therefore, the
            // other block's elements are copied to the output array.
            if (even_pos >= bl_len){
                memcpy(w, r + size * (bl_len + odd_pos), size * (bl_len - odd_pos));
                w += size * (bl_len - odd_pos);
            } else {
                memcpy(w, r + size * even_pos, size * (bl_len - even_pos));
                w += size * (bl_len - even_pos);
            }
            cur_bpair++;
            r += bl_len * size * 2;
        }

        // Now, there might be elements left that couldn't make a whole bpair.
        // These are considered leftover elements. The procedure here works si-
        // milarly to the way a blockpair is parsed. However, the program has to
        // account for the fact that the odd block will either have less than
        // bl_len elements or simply not exist, in which case the even block
        // may or may not be incomplete.
        int leftovers = num - whole_bpairs * bl_len * 2;
        if (leftovers){
            even_pos = 0;
            odd_pos = 0;
            while (even_pos < bl_len && odd_pos < (leftovers - bl_len)){
                if (compar(r + size * even_pos, r + size * (bl_len + odd_pos)) <= 0){
                    memcpy(w, r + size * even_pos, size);
                    even_pos++;
                } else {
                    memcpy(w, r + size * (bl_len + odd_pos), size);
                    odd_pos++;
                }
                w += size;
            }
            if (odd_pos >= (leftovers - bl_len)){
                memcpy(w, r + size * even_pos, size * ((bl_len > leftovers ? leftovers : bl_len) - even_pos));
            } else {
                memcpy(w, r + size * (bl_len + odd_pos), size * ((leftovers - bl_len) - odd_pos));
            }
        }

        // Swaps the output and the input arrays for the next step.
        void *swap_aux = r_first; r_first = w_first; w_first = swap_aux;
        bl_len *= 2;
    }

    // Makes sure the last iteration's progress has been stored in the original
    // array.
    if (r_first != base)
        memcpy(base, r_first, size * num);
    free(aux_array);
}
