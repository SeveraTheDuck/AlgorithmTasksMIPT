#ifndef LIST_STACK_H
#define LIST_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * @brief list_stack_error enum signalizes whether there was an error during the process or not.
 */
enum list_stack_error
{
    LIST_STACK_ERROR_OCCURED = 0, ///< an error occured
    LIST_STACK_SUCCESS       = 1  ///< everything OK
};

/**
 * @brief list_stack_error_t type is used for return actions and uses stack_error enum values
 */
typedef int list_stack_error_t;

/**
 * @brief Struct of each node with stack data. Includes pointer to data and pointer to the next node.
 */
struct list_node
{
    void* data;                     ///< Pointer to buffer with data
    struct list_node* next_node;    ///< Pointer to the next node
};

/**
 * @brief The main struct for this part, stack itself. Uses linked list as its core.
 */
struct list_stack
{
    struct list_node* list_head;    ///< Stack last element
    size_t num_of_elements;         ///< Current number of nodes with data
    size_t elem_size;               ///< Size of each element in bytes
};

/**
 * @brief Makes a stack with initial values.
 * @param stack_elem_size is size of each element in bytes
 * @return pointer to the made stack
 */
struct list_stack*
ListStackConstructor (const size_t stack_elem_size);

/**
 * @brief Destroys the stack. Fills the memory with poison value.
 * @param stk A pointer to stack to destroy.
 * @return Error status
 */
list_stack_error_t
ListStackDestructor (struct list_stack* const stk);

/**
 * @brief Pushes the value to the stack.
 * @param stk A pointer to stack to push into.
 * @param push_value A pointer to buffer with push value
 * @return Error status
 */
list_stack_error_t
ListStackPush (struct list_stack* const stk,
               const  void*       const push_value);

/**
 * @brief Pops the value from the top of the stack.
 * @param stk A pointer to stack to push into.
 * @param dest_buffer A pointer to buffer to write the value from stack
 * @return Error status
 */
list_stack_error_t
ListStackPop (struct list_stack* const stk,
                     void*       const dest_buffer);

/**
 * @brief Similar to pop but doesn't delete the value from stack.
 * @param stk A pointer to stack to push into.
 * @param dest_buffer A pointer to buffer to write the value from stack
 * @return Error status
 */
list_stack_error_t
ListStackTop (const struct list_stack* const stk,
                           void*       const dest_buffer);

#endif /* LIST_STACK_H */
