#ifndef QUADRATIC_H
#define QUADRATIC_H

#include <stdlib.h>
#include <assert.h>
#include "common.h"

void
BubbleSort (int* const   array,
            const size_t elem_number);

void
InsertionSort (int* const   array,
               const size_t elem_number);

void
SelectionSort (int* const   array,
               const size_t elem_number);

void
ShellSort (int* const   array,
           const size_t elem_number);

#endif /* QUADRATIC_H */
