#include "array_stack.h"

const size_t STACK_DATA_ARRAY_EXPANSION_MULTIPLIER = 2;

const size_t STACK_NULL_SIZE   = 0;
const char   STACK_POISON_BYTE = 0;

static stack_error_t
StackRealloc (struct stack* const stk);

static void* const
StackGetElemPtrByIndex (const struct stack* const stk,
                        const size_t elem_index);

struct stack*
StackConstructor (const size_t stack_initial_capacity,
                  const size_t stack_elem_size)
{
    struct stack* stk = (struct stack*) calloc (1, sizeof (struct stack));
    if (stk == NULL)
        return NULL;

    stk -> data_array          = NULL;
    stk -> data_array_capacity = stack_initial_capacity;
    stk -> data_array_size     = STACK_NULL_SIZE;
    stk -> elem_size           = stack_elem_size;

    if (StackRealloc (stk) == ERROR_OCCURED)
        return NULL;

    return stk;
}

stack_error_t
StackDestructor (struct stack* const stk)
{
    assert (stk);

    size_t n_bytes = stk -> data_array_capacity * stk -> elem_size;

    stk -> data_array_capacity = STACK_NULL_SIZE;
    stk -> data_array_size     = STACK_NULL_SIZE;
    stk -> elem_size           = STACK_NULL_SIZE;

    if (stk -> data_array != NULL)
        memset (stk -> data_array, STACK_POISON_BYTE, n_bytes);

    free (stk -> data_array);
    free (stk);

    return SUCCESS;
}

stack_error_t
StackPush (struct stack* const stk,
           const  void*  const push_value)
{
    assert (stk);
    assert (push_value);

    /* realloc */
    if (stk -> data_array_size == stk -> data_array_capacity)
    {
        stk -> data_array_capacity *= STACK_DATA_ARRAY_EXPANSION_MULTIPLIER;
        if (StackRealloc (stk) == ERROR_OCCURED)
            return ERROR_OCCURED;
    }

    /* push */
    void* const dest_ptr =
        StackGetElemPtrByIndex (stk, stk -> data_array_size);
    stk -> data_array_size++;

    memcpy (dest_ptr, push_value, stk -> elem_size);

    return SUCCESS;
}

stack_error_t
StackPop (struct stack* const stk,
                 void*  const dest_buffer)
{
    assert (stk);
    assert (dest_buffer);

    if (stk -> data_array_size == STACK_NULL_SIZE)
    {
        return ERROR_OCCURED;
    }

    /* pop and poisoning */
    void* const pop_ptr =
        StackGetElemPtrByIndex (stk, stk -> data_array_size - 1);

    memcpy (dest_buffer, pop_ptr, stk -> elem_size);
    memset (pop_ptr, STACK_POISON_BYTE, stk -> elem_size);

    stk -> data_array_size--;

    /* realloc */
    if (stk -> data_array_size <=
        stk -> data_array_capacity / (STACK_DATA_ARRAY_EXPANSION_MULTIPLIER *
                                      STACK_DATA_ARRAY_EXPANSION_MULTIPLIER))
    {
        stk -> data_array_capacity /= STACK_DATA_ARRAY_EXPANSION_MULTIPLIER;
        if (StackRealloc (stk) == ERROR_OCCURED)
            return ERROR_OCCURED;
    }

    return SUCCESS;
}

stack_error_t
StackTop (const struct stack* const stk,
                       void*  const dest_buffer)
{
    assert (stk);
    assert (dest_buffer);

    memcpy (dest_buffer, StackGetElemPtrByIndex (stk, stk -> data_array_size - 1),
            stk -> elem_size);

    return SUCCESS;
}

static void* const
StackGetElemPtrByIndex (const struct stack* const stk,
                        const size_t elem_index)
{
    assert (stk);

    if (elem_index > stk -> data_array_capacity)
    {
        fprintf (stderr, "In function StackGetElemByIndex "
                         "elem_index out of range\n");
        return NULL;
    }

    return (void*) ((char*) stk -> data_array +
                            elem_index * stk -> elem_size);
}

static stack_error_t
StackRealloc (struct stack* const stk)
{
    assert (stk);

    const size_t n_bytes = stk -> data_array_capacity * stk -> elem_size;

    const size_t n_poison_bytes = n_bytes - stk -> data_array_size *
                                            stk -> elem_size;

    void* new_buffer = realloc (stk -> data_array, n_bytes);

    if (new_buffer == NULL)
    {
        perror ("WARNING! StackRealloc() returned nullptr\n");
        return ERROR_OCCURED;
    }

    stk -> data_array = new_buffer;

    /* poison new bytes */
    memset (StackGetElemPtrByIndex (stk, stk -> data_array_size),
            STACK_POISON_BYTE, n_poison_bytes);

    return SUCCESS;
}
