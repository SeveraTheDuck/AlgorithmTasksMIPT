#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

extern const size_t DYNAMIC_ARRAY_RESIZE_MULTIPLIER;
extern const size_t DYNAMIC_ARRAY_NULL_SIZE;
extern const char   DYNAMIC_ARRAY_POISON_BYTE;

enum dynamic_array_error_status
{
    DYNAMIC_ARRAY_SUCCESS = 0,
    DYNAMIC_ARRAY_ERROR   = 1
};

/**
 * Adds possibility to disable realloc in dynamic array.
 * It can be useful for static array storage, so struct can
 * be used both for dynamic array and static array with
 * handly information inside.
 */
enum dynmaic_array_enable_realloc
{
    DYNAMIC_ARRAY_REALLOC_DISABLED = false,
    DYNAMIC_ARRAY_REALLOC_ENABLED  = true
};

/**
 * Adds possibility to save data_array from dynamic_array struct
 * in DynamicArrayDestructor(), so you can use it even
 * after the struct was destroyed.
 */
enum dynamic_array_save_array
{
    DYNAMIC_ARRAY_DESTROY = false,
    DYNAMIC_ARRAY_SAVE    = true
};

typedef int dynamic_array_error_t;

struct dynamic_array
{
    void* data_array;
    size_t elem_size;

    size_t data_array_size;
    size_t data_array_capacity;

    bool enable_realloc;
    bool save_array;
};

struct dynamic_array*
DynamicArrayConstructor  (const size_t initial_data_array_capaity,
                          const size_t elem_size,
                          const bool enable_realloc,
                          const bool save_array);

struct dynamic_array*
DynamicArrayDestructor   (struct dynamic_array* const d_array);

dynamic_array_error_t
DynamicArrayReallocCheck (struct dynamic_array* const d_array);

dynamic_array_error_t
DynamicArrayRealloc      (struct dynamic_array* const d_array);

dynamic_array_error_t
DynamicArrayPush         (struct dynamic_array* const d_array,
                         const  void*          const push_buffer);

dynamic_array_error_t
DynamicArrayPop          (struct dynamic_array* const d_array);

void*
DynamicArrayGetElemPtrByIndex (struct dynamic_array* const d_array,
                               const  size_t elem_index);

#endif /* DYNAMIC_ARRAY_H */
