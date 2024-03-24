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
const size_t QUEUE_INITINAL_SIZE = 4;
const size_t INPUT_STR_MAX_SIZE  = 8;

const char const ENQUEUE_STR[] = "enqueue";
const char const DEQUEUE_STR[] = "dequeue";
const char const FRONT_STR  [] = "front";
const char const SIZE_STR   [] = "size";
const char const CLEAR_STR  [] = "clear";
const char const MIN_STR    [] = "min";
const char const OK_STR     [] = "ok";
const char const ERROR_STR  [] = "error";

enum function_flags
{
    BAD_INPUT_FLAG = 0,
    ENQUEUE_FLAG   = 1,
    DEQUEUE_FLAG   = 2,
    FRONT_FLAG     = 3,
    SIZE_FLAG      = 4,
    CLEAR_FLAG     = 5,
    MIN_FLAG       = 6
};

typedef size_t function_flag_t;

/* we'll use first as value, second as min */
struct int_pair
{
    int first;
    int second;
};

struct two_stacks_queue
{
    struct stack* stk_in;
    struct stack* stk_out;
};

struct two_stacks_queue*
QueueConstructor (const size_t initial_size,
                  const size_t elem_size);

struct two_stacks_queue*
QueueDestructor (struct two_stacks_queue* const queue);

function_flag_t
GetFunctionFlag (const char* const input_str);

void
EnqueueFunction (struct two_stacks_queue* const queue,
                 struct int_pair*         const enqueue_pair);

void
FrontFunction (struct two_stacks_queue* const queue,
               struct int_pair*         const front_pair);

void
DequeueFunction (struct two_stacks_queue* const queue,
                 struct int_pair*         const dequeue_pair);

size_t
SizeFunction (struct two_stacks_queue* const queue);

void
ClearFunction (struct two_stacks_queue* const queue);

int
MinFunction (struct two_stacks_queue* const queue,
             struct int_pair*         const min_pair);

int main()
{
    size_t M = 0;
    scanf ("%zd", &M);

    struct two_stacks_queue* queue = 
        QueueConstructor (QUEUE_INITINAL_SIZE, sizeof (struct int_pair));
    assert (queue);

    struct int_pair* operation_pair = 
        (struct int_pair*) calloc (1, sizeof (struct int_pair));
    assert (operation_pair);

    char* const input_str =
        (char*) calloc (INPUT_STR_MAX_SIZE, sizeof (char));
    assert (input_str);

    function_flag_t cur_function = 0;
    bool exit = false;

    for (size_t i = 0; i < M; ++i)
    {
        scanf ("%7s", input_str);
        cur_function = GetFunctionFlag (input_str);

        switch (cur_function)
        {
            case ENQUEUE_FLAG:
                EnqueueFunction (queue, operation_pair);
                printf ("%s\n", OK_STR);
                break;

            case DEQUEUE_FLAG:
                if (SizeFunction (queue) == 0)
                    printf ("%s\n", ERROR_STR);

                else
                {
                    DequeueFunction (queue, operation_pair);
                    printf ("%d\n", operation_pair->first);
                }
                
                break;

            case FRONT_FLAG:
                if (SizeFunction (queue) == 0)
                    printf ("%s\n", ERROR_STR);

                else
                {
                    FrontFunction (queue, operation_pair);
                    printf ("%d\n", operation_pair->first);
                }
                
                break;

            case SIZE_FLAG:
                printf ("%zd\n", SizeFunction (queue));
                break;

            case CLEAR_FLAG:
                ClearFunction (queue);
                printf ("%s\n", OK_STR);
                break;

            case MIN_FLAG:
                if (SizeFunction (queue) == 0)
                    printf ("%s\n", ERROR_STR);
                else printf ("%d\n", MinFunction (queue, operation_pair));
                break;

            default:
                assert (BAD_INPUT_FLAG);
                break;
        }
    }

    queue = QueueDestructor (queue);
    free (operation_pair);
    free (input_str);

    return 0;
}

struct two_stacks_queue*
QueueConstructor (const size_t initial_size,
                  const size_t elem_size)
{
    struct two_stacks_queue* queue =
        (struct two_stacks_queue*) calloc (1, sizeof (struct two_stacks_queue));
    assert (queue);

    struct stack* stk_in =
        StackConstructor (initial_size, elem_size);
    assert (stk_in);

    struct stack* stk_out =
        StackConstructor (initial_size, elem_size);
    assert (stk_out);

    queue->stk_in  = stk_in;
    queue->stk_out = stk_out;

    return queue;
}

struct two_stacks_queue*
QueueDestructor (struct two_stacks_queue* const queue)
{
    assert (queue);

    StackDestructor (queue->stk_in);
    StackDestructor (queue->stk_out);
    free (queue);

    return NULL;
}

function_flag_t
GetFunctionFlag (const char* const input_str)
{
    assert (input_str);

    if (strcmp (input_str, ENQUEUE_STR) == 0) return ENQUEUE_FLAG;
    if (strcmp (input_str, DEQUEUE_STR) == 0) return DEQUEUE_FLAG; 
    if (strcmp (input_str, FRONT_STR)   == 0) return FRONT_FLAG; 
    if (strcmp (input_str, SIZE_STR)    == 0) return SIZE_FLAG; 
    if (strcmp (input_str, CLEAR_STR)   == 0) return CLEAR_FLAG; 
    if (strcmp (input_str, MIN_STR)     == 0) return MIN_FLAG;

    return BAD_INPUT_FLAG;
}

void
EnqueueFunction (struct two_stacks_queue* const queue,
                 struct int_pair*         const enqueue_pair)
{
    assert (queue);
    assert (queue->stk_in);
    assert (queue->stk_out);
    assert (enqueue_pair);

    int new_elem = 0;
    scanf ("%d", &new_elem);

    int queue_min = 0;

    if (queue->stk_in->d_array->data_array_size == 0)
        queue_min = new_elem;

    else
    {
        StackTop (queue->stk_in, enqueue_pair);
        queue_min = enqueue_pair->second;
        if (new_elem < queue_min) queue_min = new_elem;
    }

    enqueue_pair->first  = new_elem;
    enqueue_pair->second = queue_min;

    StackPush (queue->stk_in, enqueue_pair);
}

void
DequeueFunction (struct two_stacks_queue* const queue,
                 struct int_pair*         const dequeue_pair)
{
    assert (queue);
    assert (queue->stk_in);
    assert (queue->stk_out);
    assert (dequeue_pair);

    FrontFunction (queue, dequeue_pair);
    StackPop (queue->stk_out);
}

void
FrontFunction (struct two_stacks_queue* const queue,
               struct int_pair*         const front_pair)
{
    assert (queue);
    assert (queue->stk_in);
    assert (queue->stk_out);
    assert (front_pair);

    if (queue->stk_out->d_array->data_array_size == 0)
    {
        int stk_out_min = INT_MAX;

        while (queue->stk_in->d_array->data_array_size > 0)
        {
            StackTop (queue->stk_in,  front_pair);
            StackPop (queue->stk_in);

            if (front_pair->first < stk_out_min)
            {
                front_pair->second = front_pair->first;
                stk_out_min = front_pair->first;
            }
            else front_pair->second = stk_out_min;

            StackPush (queue->stk_out, front_pair);
        }
    }

    StackTop (queue->stk_out, front_pair);
}

size_t
SizeFunction (struct two_stacks_queue* const queue)
{
    assert (queue);
    assert (queue->stk_in);
    assert (queue->stk_out);

    return queue->stk_in ->d_array->data_array_size +
           queue->stk_out->d_array->data_array_size;
}

void
ClearFunction (struct two_stacks_queue* const queue)
{
    assert (queue);
    assert (queue->stk_in);
    assert (queue->stk_out);

    StackDestructor (queue->stk_in);
    StackDestructor (queue->stk_out);

    queue->stk_in =
        StackConstructor (QUEUE_INITINAL_SIZE, sizeof (struct int_pair));
    assert (queue->stk_in);

    queue->stk_out =
        StackConstructor (QUEUE_INITINAL_SIZE, sizeof (struct int_pair));
    assert (queue->stk_out);
}

int
MinFunction (struct two_stacks_queue* const queue,
             struct int_pair*         const min_pair)
{
    assert (queue);
    assert (queue->stk_in);
    assert (queue->stk_out);
    assert (min_pair);

    int min = 0;

    size_t stk_in_size  = queue->stk_in ->d_array->data_array_size;
    size_t stk_out_size = queue->stk_out->d_array->data_array_size;

    if (stk_in_size == 0)
    {
        StackTop (queue->stk_out, min_pair);
        min = min_pair->second;
    }

    else
    {
        StackTop (queue->stk_in, min_pair);
        min = min_pair->second;

        if (stk_out_size != 0)
        {
            StackTop (queue->stk_out, min_pair);
            if (min_pair->second < min) min = min_pair->second;
        }
    }
    
    return min;
}