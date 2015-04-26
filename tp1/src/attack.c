#include "attack.h"
#include "extern.h"

#define sqr(X) ((X) * (X))
#define AtX(X) (((Attack *)(X))->x)
#define AtY(X) (((Attack *)(X))->y)

#include <stdio.h>

long long attackCompar(const void *a, const void *b){
    int t;
    return
        (t = ((Attack *)b)->panzers - ((Attack *)a)->panzers) ? t :
        (sqr((long long)AtX(a) - base_x) + sqr((long long)AtY(a) - base_y))
        - (sqr((long long)AtX(b) - base_x) + sqr((long long)AtY(b) - base_y));
}