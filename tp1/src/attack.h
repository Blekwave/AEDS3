#ifndef _ATTACK_H_
#define _ATTACK_H_

typedef struct {
    int panzers;
    int x;
    int y;
} Attack;

long long attackCompar(const void *a, const void *b);

#endif