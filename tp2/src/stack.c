#include <stdlib.h>
#include "stack.h"

Stack *stInit(int max){
    Stack *new = malloc(sizeof(Stack));
    new->max = max;
    new->num = 0;
    new->data = calloc(sizeof(int), max);
    return new;
}

void stPush(Stack *s, int val){
    if (s->num >= s->max){
        fprintf(stderr, "ERROR: Attempting to push value to full stack");
        exit(1);
    }
    s->data[s->num] = val;
    s->num++;
}

int stPop(Stack *s, int val){
    if (s->num <= 0){
        fprintf(stderr, "ERROR: Attempting to pop value from empty stack");
        exit(2);
    }
    s->num--;
    return s->data[s->num];
}