#ifndef QSORT_H
#define QSORT_H

#include <stdlib.h>
#include <assert.h>

void
QuickLomutoSort (int* const   array,
                 const size_t elem_number);

void
QuickHoareSort (int* const   array,
                const size_t elem_number);

void
QuickThickSort (int* const   array,
                const size_t elem_number);

#endif /* QSORT_H */
