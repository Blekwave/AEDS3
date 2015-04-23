void msort(void *base, size_t num, size_t size,
           int (*compar)(const void *, const void *)){
    void *r_first = base, *w_first = malloc(size * num);
    int bl_len = 1;
    while (bl_len < num){
        void *r = r_first; void *w = w_first;

        void *swap_aux = r_first; r_first = w_first; w_first = swap_aux; // Swap
        bl_len *= 2;
    }
}