#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>



//-----------------------------------------------------------------------------
// Dynamic array consts
//-----------------------------------------------------------------------------
const size_t DYNAMIC_ARRAY_RESIZE_MULTIPLIER = 2;
const size_t DYNAMIC_ARRAY_NULL_SIZE   = 0;
const char   DYNAMIC_ARRAY_POISON_BYTE = 0;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Stack consts
//-----------------------------------------------------------------------------
const size_t STACK_RESIZE_MULTIPLIER = 2;
const size_t STACK_NULL_SIZE   = 0;
const char   STACK_POISON_BYTE = 0;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Dynamic array types and structs
//-----------------------------------------------------------------------------
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
struct dynamic_array
{
    void* data_array;           ///< Pointer to array with all elements.
    size_t elem_size;           ///< Size of each element in bytes.

    size_t data_array_size;     ///< Number of elements.
    size_t data_array_capacity; ///< Current max number of elements in array.

    bool enable_realloc;        ///< Enable or disable realloc during push/pop.
    bool save_array;            ///< Save or destroy data_array in DynamicArrayDestructor().
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Stack types and structs
//-----------------------------------------------------------------------------
enum stack_error_status
{
    STACK_SUCCESS = 0,
    STACK_ERROR   = 1
};

typedef int stack_error_t;

struct stack
{
    struct dynamic_array* d_array;
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Dynamic array functions' prototypes
//-----------------------------------------------------------------------------
/**
 * @brief Makes the struct and returns a pointer to it.
 * @param initial_data_array_capacity Sets data_array_capacity field. If 0, data_array = NULL.
 * @param elem_size Size of each element. Sets elem_size field in struct If 0, data_array = NULL.
 * @param enable_realloc Sets enable_realloc field in struct.
 * @param save_array Sets save_array field in struct.
 * @return Struct with filled fields.
*/
struct dynamic_array*
DynamicArrayConstructor  (const size_t initial_data_array_capaity,
                          const size_t elem_size,
                          const bool enable_realloc,
                          const bool save_array);

/**
 * @brief Destroys the struct and clear its fields.
 * If save_array field is set to DYNAMIC_ARRAY_SAVE, doesn't destroy
 * the data_array field. Otherwise, destroyes and frees it.
 * @param d_array Pointer to dynamic array struct.
 * @return NULL
*/
struct dynamic_array*
DynamicArrayDestructor   (struct dynamic_array* const d_array);

/**
 * @brief Checks whether data_array needs realloc and calls DynamicArrayRealloc() if true.
 * Does nothing if enable_realloc == DYNAMIC_ARRAY_REALLOC_DISABLED
 * @param d_array Pointer to dynamic array struct.
 * @return Error status.
 */
dynamic_array_error_t
DynamicArrayReallocCheck (struct dynamic_array* const d_array);

/**
 * @brief Reallocs data_array field up and down.
 * @param d_array Pointer to dynamic array struct.
 * @return Error status.
 */
dynamic_array_error_t
DynamicArrayRealloc      (struct dynamic_array* const d_array);

/**
 * @brief Pushes value from push_buffer in the end of the array.
 * Calls for DynamicArrayCheckRealloc() before pushing the value.
 * @param d_array Pointer to dynamic array struct.
 * @param push_buffer A pointer to element to copy in the array.
 * @return Error status.
 */
dynamic_array_error_t
DynamicArrayPush         (struct dynamic_array* const d_array,
                          const  void*          const push_buffer);

/**
 * @brief Pops value from the end of the array.
 * Calls for DynamicArrayCheckRealloc() after poping the value.
 * @param d_array Pointer to dynamic array struct.
 * @return Error status.
 */
dynamic_array_error_t
DynamicArrayPop          (struct dynamic_array* const d_array);

/**
 * @brief Gets pointer to element with given index.
 * @param d_array Pointer to dynamic array struct.
 * @param elem_index Element index to get.
 * @return Pointer to element. NULL if index is out of range.
 */
void*
DynamicArrayGetElemPtrByIndex (struct dynamic_array* const d_array,
                               const  size_t elem_index);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Stack functions' prototypes
//-----------------------------------------------------------------------------
struct stack*
StackConstructor (const size_t initial_data_array_capaity,
                  const size_t elem_size);

struct stack*
StackDestructor  (struct stack* const stk);

stack_error_t
StackPush        (struct stack* const stk,
                  const  void*  const push_buffer);

stack_error_t
StackPop         (struct stack* const stk);

stack_error_t
StackTop         (struct stack* const stk,
                  void*  const buffer);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Dynamic array funstions
//-----------------------------------------------------------------------------
/**
 * @brief Checks whether the struct pointers are OK.
 * @param d_array Pointer to struct.
 * @return Error status.
 */
static dynamic_array_error_t
DynamicArrayCheck (struct dynamic_array* const d_array);

struct dynamic_array*
DynamicArrayConstructor (const size_t initial_data_array_capaity,
                         const size_t elem_size,
                         const bool enable_realloc,
                         const bool save_array)
{
    struct dynamic_array* new_dynamic_array =
        (struct dynamic_array*) calloc (1, sizeof (struct dynamic_array));
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

    new_dynamic_array->data_array          = new_data_array;
    new_dynamic_array->data_array_capacity = initial_data_array_capaity;
    new_dynamic_array->data_array_size     = DYNAMIC_ARRAY_NULL_SIZE;
    new_dynamic_array->elem_size           = elem_size;
    new_dynamic_array->enable_realloc      = enable_realloc;
    new_dynamic_array->save_array          = save_array;

    return new_dynamic_array;
}

struct dynamic_array*
DynamicArrayDestructor (struct dynamic_array* const d_array)
{
    if (d_array == NULL) return NULL;

    /* destroy data_array */
    if (d_array->data_array != NULL &&
        d_array->save_array == DYNAMIC_ARRAY_DESTROY)
    {
        for (size_t i = 0; i < d_array->data_array_capacity; ++i)
        {
            memset (DynamicArrayGetElemPtrByIndex (d_array, i),
                    DYNAMIC_ARRAY_POISON_BYTE, d_array->elem_size);
        }

        free (d_array->data_array);
    }

    memset (d_array, DYNAMIC_ARRAY_POISON_BYTE, sizeof (struct dynamic_array));

    free (d_array);

    return NULL;
}

dynamic_array_error_t
DynamicArrayReallocCheck (struct dynamic_array* const d_array)
{
    assert (d_array);
    assert (d_array->data_array);

    if (d_array->enable_realloc == DYNAMIC_ARRAY_REALLOC_DISABLED)
        return DYNAMIC_ARRAY_SUCCESS;

    /* realloc up */
    if (d_array->data_array_size == d_array->data_array_capacity)
    {
        d_array->data_array_capacity *= DYNAMIC_ARRAY_RESIZE_MULTIPLIER;

        if (DynamicArrayRealloc (d_array) == DYNAMIC_ARRAY_ERROR)
            return DYNAMIC_ARRAY_ERROR;
    }

    /* realloc down */
    else if (d_array->data_array_size <
             d_array->data_array_capacity / DYNAMIC_ARRAY_RESIZE_MULTIPLIER /
                                            DYNAMIC_ARRAY_RESIZE_MULTIPLIER)
    {
        d_array->data_array_capacity /= DYNAMIC_ARRAY_RESIZE_MULTIPLIER;

        if (DynamicArrayRealloc (d_array) == DYNAMIC_ARRAY_ERROR)
            return DYNAMIC_ARRAY_ERROR;
    }

    return DYNAMIC_ARRAY_SUCCESS;
}

dynamic_array_error_t
DynamicArrayRealloc (struct dynamic_array* const d_array)
{
    assert (d_array);
    assert (d_array->data_array);

    const size_t num_of_bytes =
        d_array->data_array_capacity * d_array->elem_size;

    void* new_data_array = realloc (d_array->data_array, num_of_bytes);
    if (new_data_array == NULL)
        return DYNAMIC_ARRAY_ERROR;

    d_array->data_array = new_data_array;

    /* poison new bytes */
    for (size_t i = d_array->data_array_size;
                i < d_array->data_array_capacity; ++i)
    {
        memset (DynamicArrayGetElemPtrByIndex (d_array, i),
                DYNAMIC_ARRAY_POISON_BYTE, d_array->elem_size);
    }

    return DYNAMIC_ARRAY_SUCCESS;
}

dynamic_array_error_t
DynamicArrayPush (struct dynamic_array* const d_array,
                  const  void*          const push_buffer)
{
    if (DynamicArrayCheck (d_array) == DYNAMIC_ARRAY_ERROR ||
        push_buffer == NULL)
        return DYNAMIC_ARRAY_ERROR;

    if (DynamicArrayReallocCheck (d_array) == DYNAMIC_ARRAY_ERROR)
        return DYNAMIC_ARRAY_ERROR;

    void* const dest_ptr =
        DynamicArrayGetElemPtrByIndex (d_array, d_array->data_array_size);

    if (dest_ptr == NULL)
        return DYNAMIC_ARRAY_ERROR;

    memcpy (dest_ptr, push_buffer, d_array->elem_size);
    d_array->data_array_size++;

    return DYNAMIC_ARRAY_SUCCESS;
}

dynamic_array_error_t
DynamicArrayPop (struct dynamic_array* const d_array)
{
    if (DynamicArrayCheck (d_array) == DYNAMIC_ARRAY_ERROR)
        return DYNAMIC_ARRAY_ERROR;

    if (d_array->data_array_size == DYNAMIC_ARRAY_NULL_SIZE)
        return DYNAMIC_ARRAY_ERROR;

    void* const dest_ptr =
        DynamicArrayGetElemPtrByIndex (d_array, d_array->data_array_size - 1);

    if (dest_ptr == NULL)
        return DYNAMIC_ARRAY_ERROR;

    memset (dest_ptr, DYNAMIC_ARRAY_POISON_BYTE, d_array->elem_size);
    d_array->data_array_size--;

    if (DynamicArrayReallocCheck (d_array) == DYNAMIC_ARRAY_ERROR)
        return DYNAMIC_ARRAY_ERROR;

    return DYNAMIC_ARRAY_SUCCESS;
}

void*
DynamicArrayGetElemPtrByIndex (struct dynamic_array* const d_array,
                               const  size_t elem_index)
{
    if (DynamicArrayCheck (d_array) == DYNAMIC_ARRAY_ERROR ||
        d_array->data_array_capacity <= elem_index)
        return NULL;

    return (void*) ((char*) d_array->data_array +
                            d_array->elem_size * elem_index);
}

static dynamic_array_error_t
DynamicArrayCheck (struct dynamic_array* const d_array)
{
    if (d_array == NULL || d_array->data_array == NULL)
        return DYNAMIC_ARRAY_ERROR;

    return DYNAMIC_ARRAY_SUCCESS;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Stack funstions
//-----------------------------------------------------------------------------
static stack_error_t
StackCheck (const struct stack* const stk);

struct stack*
StackConstructor (const size_t initial_data_array_capaity,
                  const size_t elem_size)
{
    if (initial_data_array_capaity == STACK_NULL_SIZE ||
        elem_size                  == STACK_NULL_SIZE)
        return NULL;

    struct stack* stk =
        (struct stack*) calloc (1, sizeof (struct stack));
    if (stk == NULL)
        return NULL;

    struct dynamic_array* d_array = 
        DynamicArrayConstructor (initial_data_array_capaity, elem_size,
                                 DYNAMIC_ARRAY_REALLOC_ENABLED,
                                 DYNAMIC_ARRAY_SAVE);
    if (d_array == NULL)
    {
        free (stk);
        return NULL;
    }

    stk -> d_array = d_array;

    return stk;
}

struct stack*
StackDestructor (struct stack* const stk)
{
    if (stk == NULL)
        return NULL;

    DynamicArrayDestructor (stk -> d_array);

    stk -> d_array = NULL;
    free (stk);

    return NULL;
}

stack_error_t
StackPush (struct stack* const stk,
           const  void*  const push_buffer)
{
    if (StackCheck (stk) == STACK_ERROR || push_buffer == NULL)
        return STACK_ERROR;

    if (DynamicArrayPush (stk -> d_array, push_buffer) == DYNAMIC_ARRAY_ERROR)
        return STACK_ERROR;

    return STACK_SUCCESS;
}

stack_error_t
StackPop (struct stack* const stk)
{
    if (StackCheck (stk) == STACK_ERROR)
        return STACK_ERROR;

    if (DynamicArrayPop (stk -> d_array) == DYNAMIC_ARRAY_ERROR)
        return STACK_ERROR;

    return STACK_SUCCESS;
}

stack_error_t
StackTop (struct stack* const stk,
                 void*  const buffer)
{
    if (StackCheck (stk) == STACK_ERROR || buffer == NULL)
        return STACK_ERROR;

    void* const src_ptr = 
        DynamicArrayGetElemPtrByIndex (stk -> d_array, 
                                       stk -> d_array -> data_array_size - 1);

    if (src_ptr == NULL)
        return STACK_ERROR;
    
    memcpy (buffer, src_ptr, stk -> d_array -> elem_size);

    return STACK_SUCCESS;
}

static stack_error_t
StackCheck (const struct stack* const stk)
{
    if (stk == NULL || stk -> d_array == NULL ||
        stk -> d_array -> data_array  == NULL)
        return STACK_ERROR;

    return STACK_SUCCESS;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
const size_t STACK_INITINAL_DATA_CAPACITY = 4;

bool
CheckLastSymbol (struct stack* const stk,
                 const char open_type);

enum bracket_types
{
    FIRST_TYPE_OPEN   = '(',
    FIRST_TYPE_CLOSE  = ')',
    SECOND_TYPE_OPEN  = '[',
    SECOND_TYPE_CLOSE = ']',
    THIRD_TYPE_OPEN   = '{',
    THIRD_TYPE_CLOSE  = '}'
};


int main ()
{
    struct stack* stk =
        StackConstructor (STACK_INITINAL_DATA_CAPACITY, sizeof (char));
    assert (stk);

    char new_char = 0;
    bool continue_status = true;
    bool result = true;

    while (continue_status)
    {
        new_char = getchar ();

        switch (new_char)
        {
            case FIRST_TYPE_OPEN:
                [[fallthrough]];
            case SECOND_TYPE_OPEN:
                [[fallthrough]];
            case THIRD_TYPE_OPEN:
                StackPush (stk, &new_char);
                break;

            case FIRST_TYPE_CLOSE:
                result = result && CheckLastSymbol (stk, FIRST_TYPE_OPEN);
                break;

            case SECOND_TYPE_CLOSE:
                result = result && CheckLastSymbol (stk, SECOND_TYPE_OPEN);
                break;

            case THIRD_TYPE_CLOSE:
                result = result && CheckLastSymbol (stk, THIRD_TYPE_OPEN);
                break;

            default:
                continue_status = false;
                break;
        }
    }

    if (result == true && stk->d_array->data_array_size == 0) printf ("YES\n");
    else printf ("NO\n");

    StackDestructor (stk);
    return 0;
}

bool
CheckLastSymbol (struct stack* const stk,
                 const char open_type)
{
    assert (stk);

    if (stk->d_array->data_array_size == 0)
        return false;

    char old_symbol = 0;
    StackTop (stk, &old_symbol);

    if (old_symbol == open_type)
    {
        StackPop (stk);
        return true;
    }

    return false;
}