#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <stdio.h> //DEBUG
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

void
MergeRecursiveSort (int* const   array,
                    const size_t elem_number);

void
MergeIterativeSort (int* const   array,
                    const size_t elem_number);

#endif /* MERGE_SORT_H */
