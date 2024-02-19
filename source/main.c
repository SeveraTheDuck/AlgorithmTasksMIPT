#include "tests.h"

const size_t STRING_ARGUMENT_INDEX = 1;
const size_t TEST_REPEAT_NUMBER = 3;

int main (const int argc, const char** const argv)
{
    if (argc < 2) return 0;

    if (argv [STRING_ARGUMENT_INDEX][0] == '1')
    {
        for (size_t i = 0; i < TEST_REPEAT_NUMBER; ++i)
        {
            Test1 ();
        }
    }

    if (argv [STRING_ARGUMENT_INDEX][0] == '2')
    {
        for (size_t i = 0; i < TEST_REPEAT_NUMBER; ++i)
        {
            Test2 ();
        }
    }

    if (argv [STRING_ARGUMENT_INDEX][0] == '3')
    {
        for (size_t i = 0; i < TEST_REPEAT_NUMBER; ++i)
        {
            Test3 ();
        }
    }
}