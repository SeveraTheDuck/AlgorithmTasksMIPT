#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include "../lib/include/k_heap.h"
#include <limits.h>

enum int_compare
{
    LESS    = -1,
    GREATER =  1
};

enum heap_k_value
{
    K_2 = 2,
    K_3 = 3,
    K_4 = 4,
    K_5 = 5,
    K_6 = 6
};

void
HeapSort (int* const   array,
          const size_t elem_number,
          const size_t heap_k_value);

#endif
