#include "parser.h"
#include "attack.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define DIGIT_OFFSET 48

void resetFSM(Attack *a, char *state){
    a->panzers = 1;
    a->x = 0;
    a->y = 0;
    *state = 0;
}

size_t parseData(char *addr){
    FILE *out = fopen(addr, "wb");
    char c, state = 0;
    Attack a = {1, 0, 0};
    while ((c = getchar()) != EOF){
        if (state < 6){ // Initial po+int statement
            if ((c == 'p' && state == 0) ||
                (c == 'o' && state == 1) ||
                (c == 'i' && state == 2) ||
                (c == 'n' && state == 3) ||
                (c == 't' && state == 4) ||
                (c == '(' && state == 5))
                state++;
            else if (c == 'o' && (state == 2)){
                a.panzers++;
            }
        } else if (state < 10) { // Getting coordinates
            if (c >= '0' && c <= '9'){
                if (state < 8){
                    state = 7;
                    a.x = (a.x << 3) + (a.x << 1) + c - DIGIT_OFFSET; // * 10 + char
                    if (a.x >= 100000)
                        resetFSM(&a, &state);
                } else {
                    state = 9;
                    a.y = (a.y << 3) + (a.y << 1) + c - DIGIT_OFFSET; // * 10 + char
                    if (a.y >= 100000)
                        resetFSM(&a, &state);
                }
            }
            else if (c == ','){
                if (state == 7){
                    state++;
                }
                else {
                    resetFSM(&a, &state);
                }
            }
            else if (c == ')' && state == 9){ // Done and valid, output to file
                fwrite(&a, sizeof(Attack), 1, out);
                resetFSM(&a, &state);
            }
            else if (c == '('){
                resetFSM(&a, &state);
            }
        }
    }
    size_t size = ftell(out);
    fclose(out);
    return size;
}

void unpackData(char *addr){
    FILE *in = fopen(addr, "rb");
    Attack a;
    while (fread(&a, sizeof(Attack), 1, in))
        printf("%d;(%d,%d)\n", a.panzers, a.x, a.y);
    fclose(in);
}