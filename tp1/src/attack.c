#include "attack.h"
#include "extern.h"

/**
 * The Attack type is used widely across this program. It contains the coordina-
 * tes to the attack as well as the number of panzers involved in it.
 **

typedef struct {
    int panzers;
    int x;
    int y;
} Attack;
 */

#define sqr(X) (((long long)(X)) * (X))
#define distbase(X) (sqr((X)->x - base_x) + sqr((X)->y - base_y))

/**
 * Comparison function that determines the order of two Attacks. Given two at-
 * tacks A and B, A comes before B if the number of panzers of A is greater than
 * the number of panzers in B. If they're the same, then the distance from the 
 * base to the attacks' coordinates is taken in consideration. If the distance
 * from A to the base is smaller than B's, then it should come first. Lastly,
 * if neither of these criteria are enough, then they're considered equivalent.
 * @param  a Attack to be compared.
 * @param  b Attack to be compared.
 * @return   An integer that is:
 *              - < 0 if A should come before B
 *              - = 0 if A and B are equivalent
 *              - > 0 if B should come before A
 */
int attackCompar(const void *a, const void *b){
    const Attack *A = a, *B = b;
    if (B->panzers - A->panzers){
        return B->panzers - A->panzers;
    } else {
        long long dist = distbase(A) - distbase(B);
        return dist > 0 ? 1 : (dist < 0 ? -1 : 0);
    }
}