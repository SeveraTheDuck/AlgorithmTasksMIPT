#ifndef TESTS_H
#define TESTS_H

#include "array_stack.h"
#include "list_stack.h"

#include <time.h>

/**
 * *begin time count*
 * 1. Push 10^6 integer values.
 * 2. Pop half of elements and push quarter.
 * 3. Repeat step 2 while there are more then 10^5 elements in steck (9 times).
 * *end time count*
 */
void Test1 (void);

/**
 * *begin time count*
 * 1. Push 10^6 integer values.
 * 2. 100 times: pop 10000 elements and push 10000 elements.
 * 3. Pop half of elements and push quarter.
 * 4. Repeat step 2 while there are more then 10^5 elements in steck (9 times).
 * 5. 100 times: pop 10000 elements and push 10000 elements.
 * *end time count*
 */
void Test2 (void);

/**
 * 1. Push 10^6 integer values.
 * *begin time count*
 * 2. 10^6 times: if random returns 1 - push, if random returns 2 - pop.
 * *end time count*
 */
void Test3 (void);

#endif