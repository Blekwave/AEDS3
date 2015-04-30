#include "attack.h"
#include "extern.h"

#define sqr(X) ((X) * (X))
#define AtX(X) (((Attack *)(X))->x)
#define AtY(X) (((Attack *)(X))->y)

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
long long attackCompar(const void *a, const void *b){
    int t;
    return
        (t = ((Attack *)b)->panzers - ((Attack *)a)->panzers) ? t :
        (sqr((long long)AtX(a) - base_x) + sqr((long long)AtY(a) - base_y))
        - (sqr((long long)AtX(b) - base_x) + sqr((long long)AtY(b) - base_y));
}