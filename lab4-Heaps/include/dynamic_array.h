#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* How many times to increase or decrease the capacity of dynamic array */
extern const size_t DYNAMIC_ARRAY_RESIZE_MULTIPLIER;

/* Used to describe sizes, equals to 0 */
extern const size_t DYNAMIC_ARRAY_NULL_SIZE;

/* Used to fill struct fields when destroyed, or array place after pop. Equals to 0 */
extern const char   DYNAMIC_ARRAY_POISON_BYTE;

/**
 * @brief Error status for functions' return type.
 */
enum dynamic_array_error_status
{
    DYNAMIC_ARRAY_SUCCESS = 0,  ///< No errors occured, succes
    DYNAMIC_ARRAY_ERROR   = 1   ///< Some error occured
};

/**
 * @brief Adds possibility to disable or enable realloc in dynamic array.
 * It can be useful for static array storage, so struct can
 * be used both for dynamic array and static array with
 * useful information inside.
 * If realloc is disabled, beware of push/pop being out of range.
 */
enum dynmaic_array_enable_realloc
{
    DYNAMIC_ARRAY_REALLOC_DISABLED = false, ///< Realloc is disabled.
    DYNAMIC_ARRAY_REALLOC_ENABLED  = true   ///< Realloc is enabled.
};

/**
 * @brief Adds possibility to save data_array from dynamic_array struct.
 * If true, DynamicArrayDestructor() doesn't destroy void* data_array,
 * so you can use it even after the struct was destroyed.
 */
enum dynamic_array_save_array
{
    DYNAMIC_ARRAY_DESTROY = false,  ///< Array will  be destroyed and freed.
    DYNAMIC_ARRAY_SAVE    = true    ///< Array won't be destroyed and freed.
};

/**
 * @brief Return type for functions with dynamic_array_error_status values.
*/
typedef int dynamic_array_error_t;

/**
 * @brief Main struct.
 */
typedef
struct dynamic_array
{
    void*  array;           ///< Pointer to array with all elements.
    size_t elem_size;       ///< Size of each element in bytes.

    size_t array_size;      ///< Number of elements.
    size_t capacity;        ///< Current max number of elements in array.

    bool enable_realloc;    ///< Enable or disable realloc during push/pop.
    bool save_array;        ///< Save or destroy data_array in DynamicArrayDestructor().
} dynamic_array;

/**
 * @brief Makes the struct and returns a pointer to it.
 * @param initial_data_array_capacity Sets data_array_capacity field. If 0, data_array = NULL.
 * @param elem_size Size of each element. Sets elem_size field in struct If 0, data_array = NULL.
 * @param enable_realloc Sets enable_realloc field in struct.
 * @param save_array Sets save_array field in struct.
 * @return Struct with filled fields.
*/
dynamic_array*
DynamicArrayConstructor  (const size_t initial_data_array_capaity,
                          const size_t elem_size,
                          const bool   enable_realloc,
                          const bool   save_array);

/**
 * @brief Destroys the struct and clear its fields.
 * If save_array field is set to DYNAMIC_ARRAY_SAVE, doesn't destroy
 * the data_array field. Otherwise, destroyes and frees it.
 * @param d_array Pointer to dynamic array struct.
 * @return NULL
*/
dynamic_array*
DynamicArrayDestructor   (dynamic_array* const d_array);

/**
 * @brief Checks whether array needs realloc and calls DynamicArrayRealloc() if true.
 * Does nothing if enable_realloc == DYNAMIC_ARRAY_REALLOC_DISABLED
 * @param d_array Pointer to dynamic array struct.
 * @return Error status.
 */
dynamic_array_error_t
DynamicArrayReallocCheck (dynamic_array* const d_array);

/**
 * @brief Reallocs data_array field up and down.
 * @param d_array Pointer to dynamic array struct.
 * @return Error status.
 */
dynamic_array_error_t
DynamicArrayRealloc      (dynamic_array* const d_array);

/**
 * @brief Pushes value from push_buffer in the end of the array.
 * Calls for DynamicArrayCheckRealloc() before pushing the value.
 * @param d_array Pointer to dynamic array struct.
 * @param push_buffer A pointer to element to copy in the array.
 * @return Error status.
 */
dynamic_array_error_t
DynamicArrayPush         (dynamic_array* const d_array,
                          const void*    const push_buffer);

/**
 * @brief Pops value from the end of the array.
 * Calls for DynamicArrayCheckRealloc() after poping the value.
 * @param d_array Pointer to dynamic array struct.
 * @return Error status.
 */
dynamic_array_error_t
DynamicArrayPop          (dynamic_array* const d_array);

/**
 * @brief Gets pointer to element with given index.
 * @param d_array Pointer to dynamic array struct.
 * @param elem_index Element index to get.
 * @return Pointer to element. NULL if index is out of range.
 */
void*
DynamicArrayGetElemPtrByIndex (dynamic_array* const d_array,
                               const size_t elem_index);

#endif /* DYNAMIC_ARRAY_H */
