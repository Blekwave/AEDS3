#ifndef _STACK_H_
#define _STACK_H_

typedef struct {
    int *data;
    int max;
    int num;
} Stack;

Stack *stInit(int max);

void stPush(Stack *s, int val);

int stPop(Stack *s, int val);

#endif