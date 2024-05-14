#include "../include/d_array.h"



const size_t DYNAMIC_ARRAY_RESIZE_MULTIPLIER = 2;

typedef bool d_array_realloc_status_t;



//-----------------------------------------------------------------------------
// Static functions prototypes
//-----------------------------------------------------------------------------
//-------------------------------------
// Realloc functions
static d_array_error_t
DynamicArrayRealloc (d_array_t* const array);

static d_array_realloc_status_t
DynamicArrayCheckRealloc (d_array_t* const array);

static d_array_error_t
DynamicArrayExecuteRealloc (d_array_t* const array);
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Dynamic array functions' prototypes
//-----------------------------------------------------------------------------
//-------------------------------------
// Constructor and destructor
d_array_t*
DynamicArrayConstructor (const size_t element_size)
{
    d_array_t* const array = (d_array_t*) calloc(1, sizeof(d_array_t));
    if (array == NULL)
        return NULL;

    array->element_size = element_size;
    array->size         = 0;
    array->capacity     = 0;

    return array;
}

d_array_t*
DynamicArrayDestructor (d_array_t* const array)
{
    if (array == NULL)
        return NULL;

    free(array->data);
    free(array);

    return NULL;
}
//-------------------------------------

//-------------------------------------
// Interface
d_array_error_t
DynamicArrayPush (d_array_t*  const array,
                  const void* const element)
{
    if (array   == NULL ||
        element == NULL)
        return DYNAMIC_ARRAY_ERROR;

    d_array_error_t error_status = DYNAMIC_ARRAY_SUCCESS;

    error_status = error_status || DynamicArrayRealloc (array);

    error_status = error_status || DynamicArrayPushNoRealloc (array, element);

    return error_status;
}

d_array_error_t
DynamicArrayPushNoRealloc (d_array_t*  const array,
                           const void* const element)
{
    if (array       == NULL ||
        array->size == array->capacity)
        return DYNAMIC_ARRAY_ERROR;

    void* const push_ptr = DynamicArrayGetElemPtrByIndex (array, array->size);
    if (push_ptr == NULL)
        return DYNAMIC_ARRAY_ERROR;

    memcpy (push_ptr, element, array->element_size);
    ++array->size;

    return DYNAMIC_ARRAY_SUCCESS;
}

d_array_error_t
DynamicArrayPop (d_array_t* const array)
{
    d_array_error_t error_status = DYNAMIC_ARRAY_SUCCESS;

    error_status = error_status || DynamicArrayPopNoRealloc (array);

    error_status = error_status || DynamicArrayRealloc (array);

    return error_status;
}

d_array_error_t
DynamicArrayPopNoRealloc (d_array_t* const array)
{
    if (array       == NULL ||
        array->size == 0)
        return DYNAMIC_ARRAY_ERROR;

    --array->size;

    return DYNAMIC_ARRAY_SUCCESS;
}

void*
DynamicArrayGetElemPtrByIndex (d_array_t* const array,
                               const size_t index)
{
    if (array == NULL ||
        index >= array->capacity)
        return NULL;

    return (char*)array->data + (index * array->element_size);
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Static functions implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Realloc functions
static d_array_error_t
DynamicArrayRealloc (d_array_t* const array)
{
    if (DynamicArrayCheckRealloc (array) == true)
        return DynamicArrayExecuteRealloc (array);

    return DYNAMIC_ARRAY_SUCCESS;
}

static d_array_realloc_status_t
DynamicArrayCheckRealloc (d_array_t* const array)
{
    const size_t size     = array->size;
    const size_t capacity = array->capacity;

    if (capacity == 0)
    {
        array->capacity = 1;
        return true;
    }

    else if (size == capacity)
    {
        array->capacity *= DYNAMIC_ARRAY_RESIZE_MULTIPLIER;
        return true;
    }

    else if (size < capacity / DYNAMIC_ARRAY_RESIZE_MULTIPLIER / 
                               DYNAMIC_ARRAY_RESIZE_MULTIPLIER)
    {
        array->capacity /= DYNAMIC_ARRAY_RESIZE_MULTIPLIER;
        return true;
    }

    return false;
}

static d_array_error_t
DynamicArrayExecuteRealloc (d_array_t* const array)
{
    if (array == NULL) return DYNAMIC_ARRAY_ERROR;

    const size_t num_of_bytes = array->capacity * array->element_size;

    void* const new_ptr = realloc (array->data, num_of_bytes);
    if (new_ptr == NULL)
        return DYNAMIC_ARRAY_ERROR;

    array->data = new_ptr;
    return DYNAMIC_ARRAY_SUCCESS;
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------