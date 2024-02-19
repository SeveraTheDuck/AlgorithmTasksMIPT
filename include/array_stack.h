#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * @brief stack_error enum signalizes whether there was an error during the process or not
 */
enum stack_error
{
    ERROR_OCCURED = 0, ///< an error occured
    SUCCESS       = 1  ///< everything OK
};

/**
 * @brief stack_error_t type is used for return actions and uses stack_error enum values
 */
typedef int stack_error_t;

/**
 * @brief The main struct for this part, stack itself. Uses dynamic array as its core.
 */
struct stack
{
    void*  data_array;          ///< Dynamic array with data
    size_t data_array_capacity; ///< Current capacity of the array
    size_t data_array_size;     ///< Current number of elements in data array

    size_t elem_size;           ///< Size of each element in bytes
};

/**
 * @brief Makes a stack with initial values.
 * @param stack_initial_capacity is stack data array initial capacity
 * @param stack_elem_size is size of each element of data array in bytes
 * @return pointer to the made stack 
 */
struct stack*
StackConstructor (const size_t stack_initial_capacity,
                  const size_t stack_elem_size);

/**
 * @brief Destroys the stack. Fills the memory with poison value.
 * @param stk A pointer to stack to destroy.
 * @return Error status
 */
stack_error_t
StackDestructor (struct stack* const stk);

/**
 * @brief Pushes the value to the stack.
 * @param stk A pointer to stack to push into.
 * @param push_value A pointer to buffer with push value
 * @return Error status
 */
stack_error_t
StackPush (struct stack* const stk, 
           const  void*  const push_value);

/**
 * @brief Pops the value from the top of the stack.
 * @param stk A pointer to stack to push into.
 * @param dest_buffer A pointer to buffer to write the value from stack
 * @return Error status
 */
stack_error_t
StackPop (struct stack* const stk,
                 void*  const dest_buffer);

/**
 * @brief Similar to pop but doesn't delete the value from stack.
 * @param stk A pointer to stack to push into.
 * @param dest_buffer A pointer to buffer to write the value from stack
 * @return Error status
 */
stack_error_t
StackTop (const struct stack* const stk,
                       void*  const dest_buffer);

#endif /* STACK_H */