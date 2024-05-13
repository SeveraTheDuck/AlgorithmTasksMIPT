#pragma once



#include "d_array.h"



//-----------------------------------------------------------------------------
// K-heap structure
//-----------------------------------------------------------------------------
typedef struct k_heap_key
{
    void*  key;
    size_t key_len;
}
k_heap_key;

typedef struct k_heap_value
{
    void*  value;
    size_t value_len;
}
k_heap_value;

typedef struct k_heap_item
{
    k_heap_key*   key;
    k_heap_value* value;
}
k_heap_item;

typedef struct k_heap
{
    d_array_t*  array;
    size_t      k;
    k_heap_key* infinity_key;

    int (*key_cmp) (const k_heap_key* const,
                    const k_heap_key* const);
}
k_heap_t;

enum k_heap_error_status
{
    K_HEAP_SUCCESS = 0,
    K_HEAP_ERROR   = 1
};

enum k_heap_cmp_status
{
    K_HEAP_CMP_LESS    = -1,
    K_HEAP_CMP_EQUAL   =  0,
    K_HEAP_CMP_GREATER =  1
};

typedef size_t k_heap_error_t;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// K-heap functions prototypes
//-----------------------------------------------------------------------------
//-------------------------------------
// Constructor and destructor
k_heap_t*
KHeapConstructor (const size_t k,
                  const k_heap_key* const infinity_key,
                  int (*key_cmp)   (const k_heap_key* const,
                                    const k_heap_key* const));

k_heap_t*
KHeapDestructor  (k_heap_t* const heap);
//-------------------------------------

//-------------------------------------
// Interface
k_heap_error_t
KHeapInsert      (k_heap_t*           const heap,
                  const k_heap_key*   const key,
                  const k_heap_value* const value);

k_heap_value*
KHeapGetRoot     (k_heap_t* const heap);

k_heap_error_t
KHeapExtractRoot (k_heap_t* const heap);
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// K-heap key, value and item functions
//-----------------------------------------------------------------------------
//-------------------------------------
// Сonstructor and destructor
k_heap_key*
KHeapKeyConstructor   (const void* const key,
                       const size_t key_len);

k_heap_key*
KHeapKeyDestructor    (k_heap_key* const key);

k_heap_value*
KHeapValueConstructor (const void* const value,
                       const size_t value_len);

k_heap_value*
KHeapValueDestructor  (k_heap_value* const value);

k_heap_item*
KHeapItemConstructor (const k_heap_key*   const key,
                      const k_heap_value* const value);

k_heap_item*
KHeapItemDestructor  (k_heap_item* const item);
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------