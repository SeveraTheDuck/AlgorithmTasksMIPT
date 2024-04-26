#ifndef K_HEAP_H
#define K_HEAP_H

#include "dynamic_array.h"

extern const size_t K_HEAP_NULL_SIZE;
extern const size_t K_HEAP_ROOT_INDEX;
extern const size_t K_HEAP_INITIAL_DATA_ARRAY_CAPACITY;

enum k_heap_error_status
{
    K_HEAP_SUCCESS = 0,
    K_HEAP_ERROR   = 1
};

typedef int k_heap_error_t;

typedef
struct k_heap
{
    size_t k;
    const void* infinity_value;
    int (*cmp) (void*, void*);
    struct dynamic_array* d_array;
} k_heap;

k_heap*
KHeapConstructor (dynamic_array* d_array,
                  const size_t k,
                  const size_t elem_size,
                  const void* const infinity_value,
                  int (*comparator) (void*, void*));

k_heap*
KHeapDestructor (k_heap* const heap);

k_heap_error_t
KHeapSiftUp (k_heap* const heap,
             const size_t key_index);

k_heap_error_t
KHeapSiftDown (k_heap* const heap,
               const size_t key_index);

k_heap_error_t
KHeapInsert (k_heap* const heap,
             const void* const insert_buffer);

k_heap_error_t
KHeapExtractRoot (k_heap* const heap,
                  void*   const get_buffer);

k_heap_error_t
KHeapDeleteKey (k_heap* const heap,
                const size_t key_index);

void*
KHeapGetElemPtrByIndex (k_heap* const heap,
                        const size_t key_index);

#endif /* K_HEAP_H */
