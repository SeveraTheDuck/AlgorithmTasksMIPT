#ifndef COMMON_H
#define COMMON_H

#include <assert.h>

enum int_compare
{
    LESS    = -1,
    EQUAL   =  0,
    GREATER =  1
};

int 
int_cmp (void* elem1,
         void* elem2);

void
int_swap (int* first, int* second);

#endif /* COMMON_H */