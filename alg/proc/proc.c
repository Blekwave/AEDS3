#include "proc.h"
#include <limits.h>
#include <string.h>
#include <stdlib.h>

int bmhs(char *text, char *pattern){
    int text_len = strlen(text);
    int pat_len = strlen(pattern);
    // Pre-processing: prefix table
    int prefixes[CHAR_MAX + 1];
    int i;
    for (i = 0; i < CHAR_MAX + 1; i++)
        prefixes[i] = pat_len + 1;
    for (i = 0; i < pat_len; i++)
        prefixes[pattern[i]] = pat_len - i;

    // Actual procedure
    int check_pos, pos = 0;
    while (pos <= text_len - pat_len){
        check_pos = pat_len - 1;
        while(check_pos >= 0 && pattern[check_pos] == text[pos + check_pos])
            check_pos--;
        if (check_pos == -1)
            return pos;
        pos += prefixes[text[pos + check_pos + 1]];
    }

    return -1;
}

int bmh(char *text, char *pattern){
    int text_len = strlen(text);
    int pat_len = strlen(pattern);
    // Pre-processing: prefix table
    int prefixes[CHAR_MAX + 1];
    int i;
    for (i = 0; i < CHAR_MAX + 1; i++)
        prefixes[i] = pat_len;
    for (i = 0; i < pat_len - 1; i++)
        prefixes[pattern[i]] = pat_len - i - 1;

    // Actual procedure
    int check_pos, pos = 0;
    while (pos <= text_len - pat_len){
        check_pos = pat_len - 1;
        while(check_pos >= 0 && pattern[check_pos] == text[pos + check_pos])
            check_pos--;
        if (check_pos == -1)
            return pos;
        pos += prefixes[text[pos + check_pos]];
    }

    return -1;
}


int shift_and(char *text, char *pattern){
    int pat_len = strlen(pattern);
    int text_len = strlen(text);
    // Pre-processing: mask table
    long long state = 0, masks[CHAR_MAX + 1];
    int i;
    for (i = 0; i < CHAR_MAX + 1; i++)
        masks[i] = 0;
    for (i = 0; i < pat_len; i++)
        masks[pattern[i]] |= 1 << (pat_len - i - 1);

    // Actual procedure
    for (i = 0; i < text_len && !(state & 1); i++)
        state = ((1 << pat_len - 1) | (state >> 1)) & masks[text[i]];

    if (state & 1)
        return i - pat_len;

    return -1;
}


int kmp(char *text, char *pattern){
    int pat_len = strlen(pattern);
    int text_len = strlen(text);
    
    int *prefixes = malloc(sizeof(int) * pat_len);
    prefixes[0] = -1;
    prefixes[1] = 0;
    int i, j, prefix_len;
    for (i = 2; i <= pat_len; i++){
        for (prefix_len = pat_len - 1; prefix_len > 0; prefix_len--)
            for (j = 0; j < (pat_len - prefix_len + 1) && ; j++)
        prefixes[i] = j;
    }

    for (i = 0; i < text_len; i += j - prefixes[j - 1]){
        for (j = 0; j < pat_len; j++)
            if (text[i + j] != pattern[j])
                break;
        if (j == pat_len){
            free(prefixes - 1);
            return i;
        }
    }

    free(prefixes - 1);
    return -1;
}