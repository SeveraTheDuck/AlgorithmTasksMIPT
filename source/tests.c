#include "tests.h"

const size_t TEST_ELEM_NUMBER = 1000000;
const size_t ARRAY_STACK_INITIAL_CAPACITY = 4;
const double MSEC_IN_SEC = 1000.0;

const size_t TEST1_ITERATION_NUMBER = 9;
const size_t TEST1_ITERATION_MULTIPLIER = 2;

const size_t TEST2_ITERATION_NUMBER     = 100;
const size_t TEST2_FLEXIBLE_ELEM_NUMBER = 10000;

const size_t TEST3_ITERATION_NUMBER = 1000000;
const size_t TEST3_PUSH = 1;
const size_t TEST3_POP  = 2;

static double
ClocksToMs (const clock_t time_begin,
            const clock_t time_end);

static int
LittleRandom (void);

void Test1 (void)
{
    int push_value = 0;
    int pop_value  = 0;

    struct stack* array_stk =
        StackConstructor (ARRAY_STACK_INITIAL_CAPACITY, sizeof (int));

    struct list_stack* list_stk = ListStackConstructor (sizeof (int));

    //-------------------------------------------------------------------------
    // array stack part
    //-------------------------------------------------------------------------
    clock_t array_stack_begin = clock ();

    for (size_t i = 0; i < TEST_ELEM_NUMBER; ++i)
    {
        push_value = rand ();

        StackPush (array_stk, &push_value);
    }

    size_t internal_iterator = 0;

    for (size_t i = 0; i < TEST1_ITERATION_NUMBER; ++i)
    {
        internal_iterator =
            array_stk -> data_array_size / TEST1_ITERATION_MULTIPLIER;

        for (size_t j = 0; j < internal_iterator; ++j)
        {
            StackPop (array_stk, &pop_value);
        }

        internal_iterator /= TEST1_ITERATION_MULTIPLIER;
        for (size_t j = 0; j < internal_iterator; ++j)
        {
            push_value = rand ();

            StackPush (array_stk, &push_value);
        }
    }

    clock_t array_stack_end = clock ();

    //-------------------------------------------------------------------------
    // list stack part
    //-------------------------------------------------------------------------
    clock_t list_stack_begin = clock ();

    for (size_t i = 0; i < TEST_ELEM_NUMBER; ++i)
    {
        push_value = rand ();

        ListStackPush (list_stk, &push_value);
    }

    for (size_t i = 0; i < TEST1_ITERATION_NUMBER; ++i)
    {
        internal_iterator =
            list_stk -> num_of_elements / TEST1_ITERATION_MULTIPLIER;

        for (size_t j = 0; j < internal_iterator; ++j)
        {
            ListStackPop (list_stk, &pop_value);
        }

        internal_iterator /= TEST1_ITERATION_MULTIPLIER;

        for (size_t j = 0; j < internal_iterator; ++j)
        {
            push_value = rand ();

            ListStackPush (list_stk, &push_value);
        }
    }

    clock_t list_stack_end = clock ();

    printf ("%lg %lg\n", ClocksToMs (array_stack_begin, array_stack_end),
                         ClocksToMs (list_stack_begin,  list_stack_end));

    StackDestructor (array_stk);
    ListStackDestructor (list_stk);
}

void Test2 (void)
{
    int push_value = 0;
    int pop_value  = 0;

    struct stack* array_stk =
        StackConstructor (ARRAY_STACK_INITIAL_CAPACITY, sizeof (int));

    struct list_stack* list_stk = ListStackConstructor (sizeof (int));

    //-------------------------------------------------------------------------
    // array stack part
    //-------------------------------------------------------------------------
    clock_t array_stack_begin = clock ();

    for (size_t i = 0; i < TEST_ELEM_NUMBER; ++i)
    {
        push_value = rand ();

        StackPush (array_stk, &push_value);
    }

    for (size_t i = 0; i < TEST2_ITERATION_NUMBER; ++i)
    {
        for (size_t j = 0; j < TEST2_FLEXIBLE_ELEM_NUMBER; ++j)
        {
            StackPop (array_stk, &pop_value);
        }

        for (size_t j = 0; j < TEST2_FLEXIBLE_ELEM_NUMBER; ++j)
        {
            push_value = rand ();
            StackPush (array_stk, &push_value);
        }
    }

    size_t internal_iterator = 0;

    for (size_t i = 0; i < TEST1_ITERATION_NUMBER; ++i)
    {
        internal_iterator =
            array_stk -> data_array_size / TEST1_ITERATION_MULTIPLIER;

        for (size_t j = 0; j < internal_iterator; ++j)
        {
            StackPop (array_stk, &pop_value);
        }

        internal_iterator /= TEST1_ITERATION_MULTIPLIER;
        for (size_t j = 0; j < internal_iterator; ++j)
        {
            push_value = rand ();

            StackPush (array_stk, &push_value);
        }
    }

    clock_t array_stack_end = clock ();

    //-------------------------------------------------------------------------
    // list stack part
    //-------------------------------------------------------------------------
    clock_t list_stack_begin = clock ();

    for (size_t i = 0; i < TEST_ELEM_NUMBER; ++i)
    {
        push_value = rand ();

        ListStackPush (list_stk, &push_value);
    }

    for (size_t i = 0; i < TEST2_ITERATION_NUMBER; ++i)
    {
        for (size_t j = 0; j < TEST2_FLEXIBLE_ELEM_NUMBER; ++j)
        {
            ListStackPop (list_stk, &pop_value);
        }

        for (size_t j = 0; j < TEST2_FLEXIBLE_ELEM_NUMBER; ++j)
        {
            push_value = rand ();
            ListStackPush (list_stk, &push_value);
        }
    }

    for (size_t i = 0; i < TEST1_ITERATION_NUMBER; ++i)
    {
        internal_iterator =
            list_stk -> num_of_elements / TEST1_ITERATION_MULTIPLIER;

        for (size_t j = 0; j < internal_iterator; ++j)
        {
            ListStackPop (list_stk, &pop_value);
        }

        internal_iterator /= TEST1_ITERATION_MULTIPLIER;

        for (size_t j = 0; j < internal_iterator; ++j)
        {
            push_value = rand ();

            ListStackPush (list_stk, &push_value);
        }
    }

    clock_t list_stack_end = clock ();

    printf ("%lg %lg\n", ClocksToMs (array_stack_begin, array_stack_end),
                         ClocksToMs (list_stack_begin,  list_stack_end));

    StackDestructor (array_stk);
    ListStackDestructor (list_stk);
}

void Test3 (void)
{
    int push_value = 0;
    int pop_value  = 0;

    struct stack* array_stk =
        StackConstructor (ARRAY_STACK_INITIAL_CAPACITY, sizeof (int));

    struct list_stack* list_stk = ListStackConstructor (sizeof (int));

    //-------------------------------------------------------------------------
    // array stack part
    //-------------------------------------------------------------------------

    // push 10^6 values
    for (size_t i = 0; i < TEST_ELEM_NUMBER; ++i)
    {
        push_value = rand ();
        StackPush (array_stk, &push_value);
    }

    clock_t array_stack_begin = clock ();

    // push / pop 10^6 times
    for (size_t i = 0; i < TEST3_ITERATION_NUMBER; ++i)
    {
        if (LittleRandom () == TEST3_PUSH)
        {
            push_value = rand ();
            StackPush (array_stk, &push_value);
        }

        else
        {
            StackPop (array_stk, &pop_value);
        }
    }

    clock_t array_stack_end = clock ();

     //-------------------------------------------------------------------------
    // list stack part
    //-------------------------------------------------------------------------

    // push 10^6 values
    for (size_t i = 0; i < TEST_ELEM_NUMBER; ++i)
    {
        push_value = rand ();
        ListStackPush (list_stk, &push_value);
    }

    clock_t list_stack_begin = clock ();

    // push / pop 10^6 times
    for (size_t i = 0; i < TEST3_ITERATION_NUMBER; ++i)
    {
        if (LittleRandom () == TEST3_PUSH)
        {
            push_value = rand ();
            ListStackPush (list_stk, &push_value);
        }

        else
        {
            ListStackPop (list_stk, &pop_value);
        }
    }

    clock_t list_stack_end = clock ();

    printf ("%lg %lg\n", ClocksToMs (array_stack_begin, array_stack_end),
                         ClocksToMs (list_stack_begin,  list_stack_end));

    StackDestructor (array_stk);
    ListStackDestructor (list_stk);
}

static double
ClocksToMs (const clock_t time_begin,
            const clock_t time_end)
{
    return MSEC_IN_SEC * (time_end - time_begin) / CLOCKS_PER_SEC;
}

static int
LittleRandom (void)
{
    return rand () / (RAND_MAX / 2) + 1;
}
