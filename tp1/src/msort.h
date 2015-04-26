#ifndef _MSORT_H_
#define _MSORT_H_

#include <string.h>

void msort(void *base, size_t num, size_t size,
           long long (*compar)(const void *, const void *));

#endif