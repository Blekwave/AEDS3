#include "attack.h"
#include "extern.h"

#define sqr(X) ((X) * (X))
#define AtX(X) (((Attack *)(X))->x)
#define AtY(X) (((Attack *)(X))->y)

#include <stdio.h>

int attackCompar(const void *a, const void *b){
    int t;
    return
        (t = ((Attack *)b)->panzers - ((Attack *)a)->panzers) ? t :
        (sqr(AtX(a) - base_x) + sqr(AtY(a) - base_y))
        - (sqr(AtX(b) - base_x) + sqr(AtY(b) - base_y));
}