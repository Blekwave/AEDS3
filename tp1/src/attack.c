#include "attack.h"
#include "extern.h"

#define DeltaAX ((Attack *)a)->x - base_x
#define DeltaAY ((Attack *)a)->y - base_y
#define DeltaBX ((Attack *)b)->x - base_x
#define DeltaBY ((Attack *)b)->y - base_y


int attackCompar(const void *b, const void *a){
    int t;
    return
        (t = ((Attack *)a)->panzers - ((Attack *)b)->panzers) ? t :
        (DeltaAX * DeltaAX + DeltaAY * DeltaAY)
        - (DeltaBX * DeltaBX + DeltaBY * DeltaBY);
}