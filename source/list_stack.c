#include "list_stack.h"

const size_t LIST_STACK_NULL_SIZE   = 0;
const char   LIST_STACK_POISON_BYTE = 0;

static struct list_node*
ListNodeConstructor (const void*  const data,
                     const size_t elem_size);

static list_stack_error_t
ListNodeDestructor (struct list_node* const node,
                    const  size_t elem_size);

struct list_stack*
ListStackConstructor (const size_t stack_elem_size)
{
    struct list_stack* const stk =
        (struct list_stack* const) calloc (1, sizeof (struct list_stack));
    if (stk == NULL)
        return NULL;

    stk -> list_head = NULL;
    stk -> num_of_elements = LIST_STACK_NULL_SIZE;
    stk -> elem_size = stack_elem_size;

    return stk;
}

list_stack_error_t
ListStackDestructor (struct list_stack* const stk)
{
    assert (stk);

    struct list_node* cur_node  = stk -> list_head;
    struct list_node* next_node = NULL;

    while (cur_node != NULL)
    {
        next_node = cur_node -> next_node;

        ListNodeDestructor (cur_node, stk -> elem_size);

        cur_node = next_node;
    }

    stk -> elem_size = LIST_STACK_NULL_SIZE;

    return LIST_STACK_SUCCESS;
}

list_stack_error_t
ListStackPush (struct list_stack* const stk,
               const  void*       const push_value)
{
    assert (stk);
    assert (push_value);

    struct list_node* const new_node =
        ListNodeConstructor (push_value, stk -> elem_size);
    if (new_node == NULL)
        return LIST_STACK_ERROR_OCCURED;

    new_node -> next_node = stk -> list_head;
    stk -> list_head = new_node;
    stk -> num_of_elements++;

    return LIST_STACK_SUCCESS;
}

list_stack_error_t
ListStackPop (struct list_stack* const stk,
                     void*       const dest_buffer)
{
    assert (stk);
    assert (dest_buffer);

    if (ListStackTop (stk, dest_buffer) == LIST_STACK_ERROR_OCCURED)
        return LIST_STACK_ERROR_OCCURED;

    struct list_node* const new_head = stk -> list_head -> next_node;

    ListNodeDestructor (stk -> list_head, stk -> elem_size);

    stk -> list_head = new_head;
    stk -> num_of_elements--;

    return LIST_STACK_SUCCESS;
}

list_stack_error_t
ListStackTop (const struct list_stack* const stk,
                           void*       const dest_buffer)
{
    assert (stk);
    assert (dest_buffer);

    if (stk -> list_head == NULL)
        return LIST_STACK_ERROR_OCCURED;

    memcpy (dest_buffer, stk -> list_head -> data, stk -> elem_size);

    return LIST_STACK_SUCCESS;
}

struct list_node*
ListNodeConstructor (const void*  const data,
                     const size_t elem_size)
{
    assert (data);

    struct list_node* const new_node =
        (struct list_node*) calloc (1, sizeof (struct list_node));
    if (new_node == NULL)
        return NULL;

    new_node -> data = calloc (1, elem_size);
    if (new_node -> data == NULL)
    {
        free (new_node);
        return NULL;
    }

    memcpy (new_node -> data, data, elem_size);
    new_node -> next_node = NULL;

    return new_node;
}

list_stack_error_t
ListNodeDestructor (struct list_node* const node,
                    const  size_t elem_size)
{
    assert (node);

    memset (node -> data, LIST_STACK_POISON_BYTE, elem_size);
    free (node -> data);

    memset (node, LIST_STACK_POISON_BYTE, sizeof (struct list_node));
    free (node);

    return LIST_STACK_SUCCESS;
}
