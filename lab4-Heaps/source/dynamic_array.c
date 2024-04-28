#include "../include/dynamic_array.h"

const size_t DYNAMIC_ARRAY_RESIZE_MULTIPLIER = 2;
const size_t DYNAMIC_ARRAY_NULL_SIZE   = 0;
const char   DYNAMIC_ARRAY_POISON_BYTE = 0;

/**
 * @brief Checks whether the struct pointers are OK.
 * @param d_array Pointer to struct.
 * @return Error status.
 */
static dynamic_array_error_t
DynamicArrayCheck (dynamic_array* const d_array);

dynamic_array*
DynamicArrayConstructor (const size_t initial_data_array_capaity,
                         const size_t elem_size,
                         const bool   enable_realloc,
                         const bool   save_array)
{
    dynamic_array* const new_dynamic_array =
        (dynamic_array*) malloc (sizeof (dynamic_array));
    if (new_dynamic_array == NULL)
        return NULL;

    void* new_data_array = NULL;

    if (initial_data_array_capaity != DYNAMIC_ARRAY_NULL_SIZE &&
        elem_size                  != DYNAMIC_ARRAY_NULL_SIZE)
    {
        new_data_array = calloc (initial_data_array_capaity, elem_size);
        if (new_data_array == NULL)
        {
            free (new_dynamic_array);
            return NULL;
        }
    }

    new_dynamic_array->array          = new_data_array;
    new_dynamic_array->capacity       = initial_data_array_capaity;
    new_dynamic_array->array_size     = DYNAMIC_ARRAY_NULL_SIZE;
    new_dynamic_array->elem_size      = elem_size;
    new_dynamic_array->enable_realloc = enable_realloc;
    new_dynamic_array->save_array     = save_array;

    return new_dynamic_array;
}

dynamic_array*
DynamicArrayDestructor (dynamic_array* const d_array)
{
    if (d_array == NULL) return NULL;

    /* destroy data_array */
    if (d_array->array != NULL &&
        d_array->save_array == DYNAMIC_ARRAY_DESTROY)
    {
        memset (d_array->array, DYNAMIC_ARRAY_POISON_BYTE,
                d_array->array_size * d_array->elem_size);

        free (d_array->array);
    }

    memset (d_array, DYNAMIC_ARRAY_POISON_BYTE, sizeof (dynamic_array));

    free (d_array);

    return NULL;
}

dynamic_array_error_t
DynamicArrayReallocCheck (dynamic_array* const d_array)
{
    assert (d_array);
    assert (d_array->array);

    if (d_array->enable_realloc == DYNAMIC_ARRAY_REALLOC_DISABLED)
        return DYNAMIC_ARRAY_SUCCESS;

    /* realloc up */
    if (d_array->array_size == d_array->capacity)
    {
        d_array->capacity *= DYNAMIC_ARRAY_RESIZE_MULTIPLIER;

        if (DynamicArrayRealloc (d_array) == DYNAMIC_ARRAY_ERROR)
            return DYNAMIC_ARRAY_ERROR;
    }

    /* realloc down */
    else if (d_array->array_size <=
             d_array->capacity / DYNAMIC_ARRAY_RESIZE_MULTIPLIER /
                                            DYNAMIC_ARRAY_RESIZE_MULTIPLIER)
    {
        d_array->capacity /= DYNAMIC_ARRAY_RESIZE_MULTIPLIER;

        if (DynamicArrayRealloc (d_array) == DYNAMIC_ARRAY_ERROR)
            return DYNAMIC_ARRAY_ERROR;
    }

    return DYNAMIC_ARRAY_SUCCESS;
}

dynamic_array_error_t
DynamicArrayRealloc (dynamic_array* const d_array)
{
    assert (d_array);
    assert (d_array->array);

    const size_t num_of_bytes =
        d_array->capacity * d_array->elem_size;

    void* new_data_array = realloc (d_array->array, num_of_bytes);
    if (new_data_array == NULL)
        return DYNAMIC_ARRAY_ERROR;

    d_array->array = new_data_array;

    /* poison new bytes */
    for (size_t i = d_array->array_size;
                i < d_array->capacity; ++i)
    {
        memset (DynamicArrayGetElemPtrByIndex (d_array, i),
                DYNAMIC_ARRAY_POISON_BYTE, d_array->elem_size);
    }

    return DYNAMIC_ARRAY_SUCCESS;
}

dynamic_array_error_t
DynamicArrayPush (dynamic_array* const d_array,
                  const void*    const push_buffer)
{
    if (DynamicArrayCheck (d_array) == DYNAMIC_ARRAY_ERROR ||
        push_buffer == NULL)
        return DYNAMIC_ARRAY_ERROR;

    if (DynamicArrayReallocCheck (d_array) == DYNAMIC_ARRAY_ERROR)
        return DYNAMIC_ARRAY_ERROR;

    void* const dest_ptr =
        DynamicArrayGetElemPtrByIndex (d_array, d_array->array_size);

    if (dest_ptr == NULL)
        return DYNAMIC_ARRAY_ERROR;

    memcpy (dest_ptr, push_buffer, d_array->elem_size);
    d_array->array_size++;

    return DYNAMIC_ARRAY_SUCCESS;
}

dynamic_array_error_t
DynamicArrayPop (dynamic_array* const d_array)
{
    if (DynamicArrayCheck (d_array) == DYNAMIC_ARRAY_ERROR)
        return DYNAMIC_ARRAY_ERROR;

    if (d_array->array_size == DYNAMIC_ARRAY_NULL_SIZE)
        return DYNAMIC_ARRAY_ERROR;

    void* const dest_ptr =
        DynamicArrayGetElemPtrByIndex (d_array, d_array->array_size - 1);

    if (dest_ptr == NULL)
        return DYNAMIC_ARRAY_ERROR;

    memset (dest_ptr, DYNAMIC_ARRAY_POISON_BYTE, d_array->elem_size);
    d_array->array_size--;

    if (DynamicArrayReallocCheck (d_array) == DYNAMIC_ARRAY_ERROR)
        return DYNAMIC_ARRAY_ERROR;

    return DYNAMIC_ARRAY_SUCCESS;
}

void*
DynamicArrayGetElemPtrByIndex (dynamic_array* const d_array,
                               const size_t elem_index)
{
    if (DynamicArrayCheck (d_array) == DYNAMIC_ARRAY_ERROR ||
        d_array->capacity <= elem_index)
        return NULL;

    return (void*) ((char*) d_array->array +
                            d_array->elem_size * elem_index);
}

static dynamic_array_error_t
DynamicArrayCheck (struct dynamic_array* const d_array)
{
    if (d_array == NULL || d_array->array == NULL)
        return DYNAMIC_ARRAY_ERROR;

    return DYNAMIC_ARRAY_SUCCESS;
}
