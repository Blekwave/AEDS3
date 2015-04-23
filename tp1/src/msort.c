#include "msort.h"
void msort(void *base, size_t num, size_t size,
           int (*compar)(const void *, const void *)){
    void *r_first = base, *w_first = malloc(size * num);
    int bl_len = 1;
    while (bl_len < num){
        void *r = r_first; void *w = w_first;
        int whole_blocks = (num*size)/bl_len;
        int cur_bpair = 0, even_pos, odd_pos;
        while (cur_bpair < whole_blocks){
            int even_pos = 0, odd_pos = 0;
            while (even_pos < bl_len && odd_pos < bl_len){
                if (compar(&r[size * even_pos], &r[size * (bl_len + odd_pos)])){
                    memcpy(w, &r[size * even_pos], size);
                    even_pos++;
                } else {
                    memcpy(w, &r[size * (bl_len + odd_pos)], size);
                    odd_pos++;
                }
                w += size;
            }
            if (even_pos >= bl_len){
                memcpy(w, &r[size * (bl_len + odd_pos)], size * (bl_len - odd_pos));
                w += size * (bl_len - odd_pos);
            } else {
                memcpy(w, &r[size * even_pos], size * (bl_len - even_pos));
                w += size * (bl_len - even_pos);
            }
            cur_bpair += 2;
            r += bl_len * size * 2;
        }
        int leftovers = num - whole_blocks * bl_len * 2;
        even_pos = 0;
        odd_pos = 0;
        while (even_pos < bl_len && odd_pos < (leftovers - bl_len)){
            if (compar(&r[size * even_pos], &r[size * (bl_len + odd_pos)])){
                memcpy(w, &r[size * even_pos], size);
                even_pos++;
            } else {
                memcpy(w, &r[size * (bl_len + odd_pos)], size);
                odd_pos++;
            }
            w += size;
        }
        if (odd_pos >= (leftovers - bl_len)){
            memcpy(w, &r[size * even_pos], size * (bl_len - even_pos));
        } else {
            memcpy(w, &r[size * (bl_len + odd_pos)], size * ((leftovers - bl_len) - odd_pos));
        }
        void *swap_aux = r_first; r_first = w_first; w_first = swap_aux; // Swap
        bl_len *= 2;
    }
}