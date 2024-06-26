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
KHeapInsert      (k_heap_t*         const heap,
                  const k_heap_key* const key);

k_heap_error_t
KHeapInsertNoRealloc (k_heap_t*         const heap,
                      const k_heap_key* const key);

k_heap_key*
KHeapGetRoot     (k_heap_t* const heap);

k_heap_error_t
KHeapExtractRoot (k_heap_t* const heap);

k_heap_error_t
KHeapSiftDown    (k_heap_t* const heap,
                  const size_t index);
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// K-heap key functions
//-----------------------------------------------------------------------------
//-------------------------------------
// Сonstructor and destructor
k_heap_key*
KHeapKeyConstructor   (const void* const key,
                       const size_t key_len);

k_heap_key*
KHeapKeyDestructor    (k_heap_key* const key);
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------