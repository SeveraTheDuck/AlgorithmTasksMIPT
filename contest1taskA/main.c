#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------
const size_t DYNAMIC_ARRAY_RESIZE_MULTIPLIER = 2;
const size_t DYNAMIC_ARRAY_NULL_SIZE   = 0;
const char   DYNAMIC_ARRAY_POISON_BYTE = 0;

const size_t STACK_NULL_SIZE = 0;
const size_t STACK_INITIAL_CAPACITY = 10;

#define ENTER_STRING_MAX_LEN 6 
const char PUSH_STR  [] = "push";
const char POP_STR   [] = "pop";
const char BACK_STR  [] = "back";
const char SIZE_STR  [] = "size";
const char CLEAR_STR [] = "clear";
const char EXIT_STR  [] = "exit";
const char ERROR_STR [] = "error";
const char OK_STR    [] = "ok";
const char BYE_STR   [] = "bye";
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// structs and types
//-----------------------------------------------------------------------------
enum function_flags
{
    BAD_INPUT_FUNCTION_FLAG = 0,
    PUSH_FUNCTION_FLAG      = 1,
    POP_FUNCTION_FLAG       = 2,
    BACK_FUNCTION_FLAG      = 3,
    SIZE_FUNCTION_FLAG      = 4,
    CLEAR_FUNCTION_FLAG     = 5,
    EXIT_FUNCTION_FLAG      = 6
};

enum task_status
{
    TASK_ERROR   = 0,
    TASK_SUCCESS = 1
};

enum dynamic_array_error_status
{
    DYNAMIC_ARRAY_SUCCESS = 0,
    DYNAMIC_ARRAY_ERROR   = 1
};

enum stack_error_status
{
    STACK_SUCCESS = 0,
    STACK_ERROR   = 1
};

typedef size_t function_flag_t;
typedef int    task_status_t;
typedef int    dynamic_array_error_t;
typedef int    stack_error_t;

struct dynamic_array 
{
    void* data_array;
    size_t elem_size;

    size_t data_array_size;
    size_t data_array_capacity;
};

struct stack
{
    struct dynamic_array* d_array;
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Dynamic array functions' prototypes 
//-----------------------------------------------------------------------------
struct dynamic_array*
DynamicArrayConstructor  (const size_t initial_data_array_capaity,
                          const size_t elem_size);

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

dynamic_array_error_t
DynamicArrayCheck  (const struct dynamic_array* const d_array);
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

stack_error_t
StackCheck (const struct stack* const stk);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Dynamic array functions
//-----------------------------------------------------------------------------
struct dynamic_array*
DynamicArrayConstructor (const size_t initial_data_array_capaity,
                         const size_t elem_size)
{
    if (initial_data_array_capaity == DYNAMIC_ARRAY_NULL_SIZE ||
        elem_size                  == DYNAMIC_ARRAY_NULL_SIZE)
        return NULL;

    struct dynamic_array* new_dynamic_array =
        (struct dynamic_array*) calloc (1, sizeof (struct dynamic_array));
    if (new_dynamic_array == NULL)
        return NULL;

    void* new_data_array = calloc (initial_data_array_capaity, elem_size);
    if (new_data_array == NULL)
    {
        free (new_dynamic_array);
        return NULL;
    }

    new_dynamic_array -> data_array          = new_data_array;
    new_dynamic_array -> data_array_capacity = initial_data_array_capaity;
    new_dynamic_array -> data_array_size     = DYNAMIC_ARRAY_NULL_SIZE;
    new_dynamic_array -> elem_size           = elem_size;

    return new_dynamic_array;
}

struct dynamic_array*
DynamicArrayDestructor (struct dynamic_array* const d_array)
{
    if (d_array == NULL) return NULL;

    /* poison data_array */
    if (d_array -> data_array != NULL)
    {
        for (size_t i = 0; i < d_array -> data_array_capacity; ++i)
        {
            memset (DynamicArrayGetElemPtrByIndex (d_array, i),
                    DYNAMIC_ARRAY_POISON_BYTE, d_array -> elem_size);
        }
    }

    memset (d_array, DYNAMIC_ARRAY_POISON_BYTE, sizeof (struct dynamic_array));

    free (d_array -> data_array);
    free (d_array);

    return NULL;
}

dynamic_array_error_t
DynamicArrayReallocCheck (struct dynamic_array* const d_array)
{
    assert (d_array);
    assert (d_array -> data_array);

    if (d_array -> data_array_size == d_array -> data_array_capacity)
    {
        d_array -> data_array_capacity *= DYNAMIC_ARRAY_RESIZE_MULTIPLIER;

        if (DynamicArrayRealloc (d_array) == DYNAMIC_ARRAY_ERROR)
            return DYNAMIC_ARRAY_ERROR;        
    }

    else if (d_array -> data_array_size <
             d_array -> data_array_capacity / DYNAMIC_ARRAY_RESIZE_MULTIPLIER /
                                              DYNAMIC_ARRAY_RESIZE_MULTIPLIER)
    {
        d_array -> data_array_capacity /= DYNAMIC_ARRAY_RESIZE_MULTIPLIER;

        if (DynamicArrayRealloc (d_array) == DYNAMIC_ARRAY_ERROR)
            return DYNAMIC_ARRAY_ERROR;
    }

    return DYNAMIC_ARRAY_SUCCESS;
}

dynamic_array_error_t
DynamicArrayRealloc (struct dynamic_array* const d_array)
{
    assert (d_array);
    assert (d_array -> data_array);

    const size_t num_of_bytes = 
        d_array -> data_array_capacity * d_array -> elem_size;

    void* new_data_array = realloc (d_array -> data_array, num_of_bytes);
    if (new_data_array == NULL)
        return DYNAMIC_ARRAY_ERROR;

    d_array -> data_array = new_data_array;

    /* poison new bytes */
    for (size_t i = d_array -> data_array_size;
                i < d_array -> data_array_capacity; ++i)
    {
        memset (DynamicArrayGetElemPtrByIndex (d_array, i),
                DYNAMIC_ARRAY_POISON_BYTE, d_array -> elem_size);
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
        DynamicArrayGetElemPtrByIndex (d_array, d_array -> data_array_size);

    if (dest_ptr == NULL)
        return DYNAMIC_ARRAY_ERROR;

    memcpy (dest_ptr, push_buffer, d_array -> elem_size);
    d_array -> data_array_size++;

    return DYNAMIC_ARRAY_SUCCESS;
}

dynamic_array_error_t
DynamicArrayPop (struct dynamic_array* const d_array)
{
    if (DynamicArrayCheck (d_array) == DYNAMIC_ARRAY_ERROR)
        return DYNAMIC_ARRAY_ERROR;

    if (d_array -> data_array_size == DYNAMIC_ARRAY_NULL_SIZE)
        return DYNAMIC_ARRAY_ERROR;

    void* const dest_ptr =
        DynamicArrayGetElemPtrByIndex (d_array, d_array -> data_array_size - 1);

    if (dest_ptr == NULL)
        return DYNAMIC_ARRAY_ERROR;

    memset (dest_ptr, DYNAMIC_ARRAY_POISON_BYTE, d_array -> elem_size);
    d_array -> data_array_size--;

    if (DynamicArrayReallocCheck (d_array) == DYNAMIC_ARRAY_ERROR)
        return DYNAMIC_ARRAY_ERROR;

    return DYNAMIC_ARRAY_SUCCESS;
}

void*
DynamicArrayGetElemPtrByIndex (struct dynamic_array* const d_array,
                               const  size_t elem_index)
{
    if (DynamicArrayCheck (d_array) == DYNAMIC_ARRAY_ERROR ||
        d_array -> data_array_capacity <= elem_index)
        return NULL;

    return (void*) ((char*) d_array -> data_array +
                            d_array -> elem_size * elem_index);
}

dynamic_array_error_t
DynamicArrayCheck (const struct dynamic_array* const d_array)
{
    if (d_array == NULL || d_array -> data_array == NULL)
        return DYNAMIC_ARRAY_ERROR;

    return DYNAMIC_ARRAY_SUCCESS;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Stack functions
//-----------------------------------------------------------------------------
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
        DynamicArrayConstructor (initial_data_array_capaity, elem_size);
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

stack_error_t
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
// Main functions
//-----------------------------------------------------------------------------
function_flag_t
GetFunctionFlag (const char* const input_str);

void
PushFunction (struct stack* const stk);

task_status_t
PopFunction (struct stack* const stk,
             long long*    const pop_value);

task_status_t
BackFunction (struct stack* const stk,
              long long*    const pop_value);

void
ClearFunction (struct stack* const stk);

int main ()
{
    char* const input_str = 
        (char* const) calloc (ENTER_STRING_MAX_LEN, sizeof (char));
    assert (input_str);

    struct stack* stk = 
        StackConstructor (STACK_INITIAL_CAPACITY, sizeof (long long));
    assert (stk);

    function_flag_t function_flag = 0;
    task_status_t   task_status   = 0;

    long long pop_ret_value = 0;

    while (scanf ("%5s", input_str) == 1)
    {
        function_flag = GetFunctionFlag (input_str);

        switch (function_flag)
        {
            case PUSH_FUNCTION_FLAG:
                PushFunction (stk);
                printf ("%s\n", OK_STR);
                break;
            
            case POP_FUNCTION_FLAG:
                task_status = PopFunction (stk, &pop_ret_value);
                if (task_status == TASK_SUCCESS)
                    printf ("%lld\n", pop_ret_value);
                else printf ("%s\n", ERROR_STR);
                break;

            case BACK_FUNCTION_FLAG:
                task_status = BackFunction (stk, &pop_ret_value);
                if (task_status == TASK_SUCCESS)
                    printf ("%lld\n", pop_ret_value);
                else printf ("%s\n", ERROR_STR);
                break;

            case SIZE_FUNCTION_FLAG:
                printf ("%zd\n", stk -> d_array -> data_array_size);
                break;

            case CLEAR_FUNCTION_FLAG:
                ClearFunction (stk);
                printf ("%s\n", OK_STR);
                break;

            case EXIT_FUNCTION_FLAG:
                printf ("%s\n", BYE_STR);
                free (input_str);
                stk = StackDestructor (stk);
                return 0;

            default:
                assert (BAD_INPUT_FUNCTION_FLAG);
                break;
        }
    }

    return 0;
}

function_flag_t
GetFunctionFlag (const char* const input_str)
{
    assert (input_str);

    if (strcmp (PUSH_STR,  input_str) == 0) return PUSH_FUNCTION_FLAG;

    if (strcmp (POP_STR,   input_str) == 0) return POP_FUNCTION_FLAG;

    if (strcmp (BACK_STR,  input_str) == 0) return BACK_FUNCTION_FLAG;

    if (strcmp (SIZE_STR,  input_str) == 0) return SIZE_FUNCTION_FLAG;

    if (strcmp (CLEAR_STR, input_str) == 0) return CLEAR_FUNCTION_FLAG;

    if (strcmp (EXIT_STR,  input_str) == 0) return EXIT_FUNCTION_FLAG;

    return BAD_INPUT_FUNCTION_FLAG;
}

void
PushFunction (struct stack* const stk)
{
    assert (stk);

    long long push_value = 0;
    assert (scanf ("%lld", &push_value) == 1); // assert if scanf didn't read a number

    StackPush (stk, &push_value);

    return;
}

task_status_t
PopFunction (struct stack* const stk,
             long long*    const pop_value)
{
    assert (stk);
    assert (pop_value);

    if (stk -> d_array -> data_array_size == DYNAMIC_ARRAY_NULL_SIZE)
        return TASK_ERROR;

    StackTop (stk, pop_value);
    StackPop (stk);

    return TASK_SUCCESS;
}

task_status_t
BackFunction (struct stack* const stk,
              long long*    const pop_value)
{
    assert (stk);
    assert (pop_value);

    if (stk -> d_array -> data_array_size == DYNAMIC_ARRAY_NULL_SIZE)
        return TASK_ERROR;

    StackTop (stk, pop_value);

    return TASK_SUCCESS;
}

void
ClearFunction (struct stack* const stk)
{
    assert (stk);

    const size_t stk_size = stk -> d_array -> data_array_size;

    for (size_t i = 0; i < stk_size; ++i) StackPop (stk);

    return;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
