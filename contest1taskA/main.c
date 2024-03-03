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

// ask on code-review about const and sizes in const arrays
#define ENTER_STRING_MAX_LEN 6 
const char PUSH_STR  [] = "push\0";
const char POP_STR   [] = "pop\0";
const char BACK_STR  [] = "back\0";
const char SIZE_STR  [] = "size\0";
const char CLEAR_STR [] = "clear\0";
const char EXIT_STR  [] = "exit\0";
const char ERROR_STR [] = "error\0";
const char OK_STR    [] = "ok\0";
const char BYE_STR   [] = "bye\0";
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// structs and types
//-----------------------------------------------------------------------------
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

typedef int dynamic_array_error_t;
typedef int stack_error_t;

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
void
PushFunction (struct stack* const stk);

void
PopFunction (struct stack* const stk);

void
BackFunction (struct stack* const stk);

void
BackFunction (struct stack* const stk);

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

    while (scanf ("%6s", input_str) == 1)
    {
        if      (strcmp (PUSH_STR,  input_str) == 0) PushFunction  (stk);

        else if (strcmp (POP_STR,   input_str) == 0) PopFunction   (stk);

        else if (strcmp (BACK_STR,  input_str) == 0) BackFunction  (stk);

        else if (strcmp (SIZE_STR,  input_str) == 0)
            printf ("%zd\n", stk -> d_array -> data_array_size);

        else if (strcmp (CLEAR_STR, input_str) == 0) ClearFunction (stk);

        else if (strcmp (EXIT_STR,  input_str) == 0)
        {
            printf ("%s\n", BYE_STR);
            break;
        }

        else assert (0);
    }

    free (input_str);
    stk = StackDestructor (stk);

    return 0;
}

void
PushFunction (struct stack* const stk)
{
    assert (stk);

    long long push_value = 0;
    assert (scanf ("%lld", &push_value) == 1); // assert if scanf didn't read a number

    StackPush (stk, &push_value);

    printf ("%s\n", OK_STR);
}

void
PopFunction (struct stack* const stk)
{
    assert (stk);

    if (stk -> d_array -> data_array_size == DYNAMIC_ARRAY_NULL_SIZE)
    {
        printf ("%s\n", ERROR_STR);
        return;
    }

    long long pop_value = 0;
    StackTop (stk, &pop_value);
    StackPop (stk);

    printf ("%lld\n", pop_value);

}

void
BackFunction (struct stack* const stk)
{
    assert (stk);

    if (stk -> d_array -> data_array_size == DYNAMIC_ARRAY_NULL_SIZE)
    {
        printf ("%s\n", ERROR_STR);
        return;
    }

    long long back_value = 0;
    StackTop (stk, &back_value);

    printf ("%lld\n", back_value);
}

void
ClearFunction (struct stack* const stk)
{
    assert (stk);

    const size_t stk_size = stk -> d_array -> data_array_size;

    for (size_t i = 0; i < stk_size; ++i) StackPop (stk);

    printf ("%s\n", OK_STR);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
