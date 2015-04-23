#ifndef _MSORT_H_
#define _MSORT_H_

#include <string.h>

void msort(void *base, size_t num, size_t size,
           int (*compar)(const void *, const void *));

#endif