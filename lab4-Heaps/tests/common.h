#pragma once
#include <stdio.h>
#include <time.h>



enum int_cmp_status
{
    INT_CMP_LESS    = -1,
    INT_CMP_EQUAL   =  0,
    INT_CMP_GREATER =  1
};



int CompareInt (const void* const a, const void* const b);

int*
ReadIntArray (const size_t elem_number);
