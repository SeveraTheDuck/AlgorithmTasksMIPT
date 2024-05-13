#pragma once



#include <stdbool.h>
#include <stdlib.h>
#include <string.h>



//-----------------------------------------------------------------------------
// Dynamic array struct
//-----------------------------------------------------------------------------
/**
 * @brief Dynamic array structure
 */
typedef struct dynamic_array
{
    void*  data;            ///< Pointer to array with data
    size_t size;            ///< Current nuumber of elements in the array
    size_t capacity;        ///< Current capacity of the array
    size_t element_size;    ///< Size of each element in the array in bytes
}
d_array_t;

/**
 * @brief Dynamic array error statuses
 */
enum d_array_errors
{
    DYNAMIC_ARRAY_SUCCESS = 0,  ///< No errors occurred, success
    DYNAMIC_ARRAY_ERROR   = 1   ///< Error occurred
};

typedef size_t d_array_error_t;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Dynamic array functions' prototypes
//-----------------------------------------------------------------------------
//-------------------------------------
// Constructor and destructor
/**
 * @brief Constructor for dynamic array structure
 * 
 * @param element_size size of each element in bytes
 * 
 * @return pointer to the structure if success, NULL if error
 * 
 * Allocates memory for the dynamic array structure, initializes and returns it.
 * array->data field is NULL at this point.
 */
d_array_t*
DynamicArrayConstructor       (const size_t element_size);

/**
 * @brief Destructor for dynamic array structure
 * 
 * @param array pointer to dynamic array structure
 * 
 * @return NULL
 * 
 * If array == NULL, returns NULL.
 * Otherwise, frees array->data field and array itself.
 */
d_array_t*
DynamicArrayDestructor        (d_array_t* const array);
//-------------------------------------

//-------------------------------------
// Interface
/**
 * @brief Push function with reallocation of array
 * 
 * @param array pointer to dynamic array structure
 * @param element pointer to element buffer to push
 * 
 * @return DYNAMIC_ARRAY_SUCCESS on success, DYNAMIC_ARRAY_ERROR on error
 * 
 * 1. Calls for DynamicArrayRealloc() function to check and perform realloc if needed
 * 2. Calls for DynamicArrayPushNoRealloc() function after that to perform push operation
 */
d_array_error_t
DynamicArrayPush              (d_array_t*  const array,
                               const void* const element);

/**
 * @brief Push function without reallocation of array
 * 
 * @param array pointer to dynamic array structure
 * @param element pointer to element buffer to push
 * 
 * @return DYNAMIC_ARRAY_SUCCESS on success, DYNAMIC_ARRAY_ERROR on error
 * 
 * Does nothing if array->size == array->capacity, returns DYNAMIC_ARRAY_ERROR status.
 * Otherwise, copies value of array->element_size bytes from element buffer
 * to the end of array. Increases array->size.
 */
d_array_error_t
DynamicArrayPushNoRealloc     (d_array_t* const array,
                               const void* const element);
                  
/**
 * @brief Pop function with reallocation of array
 * 
 * @param array pointer to dynamic array structure
 * 
 * @return DYNAMIC_ARRAY_SUCCESS on success, DYNAMIC_ARRAY_ERROR on error
 * 
 * 1. Calls for DynamicArrayPopNoRealloc() function to perform pop operation
 * 2. Calls for DynamicArrayRealloc() function to check and perform realloc if needed
 */
d_array_error_t
DynamicArrayPop               (d_array_t* const array);

/**
 * @brief Pop function without reallocation of array
 * 
 * @param array pointer to dynamic array structure
 *
 * @return DYNAMIC_ARRAY_SUCCESS on success, DYNAMIC_ARRAY_ERROR on error
 * 
 * Does nothing if array->size == 0, returns DYNAMIC_ARRAY_ERROR status.
 * Otherwise, decreases array->size.
 */
d_array_error_t
DynamicArrayPopNoRealloc      (d_array_t* const array);

/**
 * @brief Gets pointer to the element of the dynamic array by its index
 * 
 * @param array pointer to dynamic array structure
 * 
 * @return pointer on success, NULL on error
 * 
 * Returns NULL if index out of range.
 * Otherwise, returns pointer to the element with void* pointer type.
 */
void*
DynamicArrayGetElemPtrByIndex (d_array_t* const array,
                               const size_t index);
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------