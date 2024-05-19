#include "../include/tests_common.h"



//-----------------------------------------------------------------------------
// Constants for command line arguments
//-----------------------------------------------------------------------------
const size_t INPUT_FILE_NAME_INDEX          = 1;
const size_t INSERTS_OUTPUT_FILE_NAME_INDEX = 2;
const size_t DELETES_OUTPUT_FILE_NAME_INDEX = 3;
const size_t FILES_NUMBER = 3;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Static functions prototypes
//-----------------------------------------------------------------------------
static char*
ReadInputFileToStr (const char* const file_name);

static int*
ReadInputArrayFromStr (char* str,
                       const size_t elem_number);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Input functions implementation
//-----------------------------------------------------------------------------
input_array*
ReadInputFile (const char* const file_name)
{
    char* str_begin = ReadInputFileToStr (file_name);
    assert (str_begin);

    char* str = str_begin;

    const size_t elem_number = strtoull (str, &str, 10);
    assert (elem_number > 0);

    int* const array = ReadInputArrayFromStr (str, elem_number);
    assert (array);
    free (str_begin);

    return InputArrayConstructor (array, elem_number);
}

input_array*
InputArrayConstructor (int* const array,
                       const size_t elem_number)
{
    input_array* const new_array =
        (input_array*) malloc (sizeof (input_array));
    assert (new_array);

    new_array->array = array;
    new_array->elem_number = elem_number;

    return new_array;
}

input_array*
InputArrayDestructor (input_array* const array)
{
    assert (array);

    free (array->array);
    free (array);

    return NULL;
}

int
GetRandomElement (const input_array* const array)
{
    assert (array);
    assert (array->elem_number > 0);

    const size_t index = rand () / (RAND_MAX / array->elem_number + 1);

    return array->array[index];
}

void
CheckCommandLineArguments (const int argc, const char** const argv)
{
    assert (argc == (int) FILES_NUMBER + 1);
    assert (argv);
    assert (argv[INPUT_FILE_NAME_INDEX]);
    assert (argv[INSERTS_OUTPUT_FILE_NAME_INDEX]);
    assert (argv[DELETES_OUTPUT_FILE_NAME_INDEX]);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Static functions implementation
//-----------------------------------------------------------------------------
static char*
ReadInputFileToStr (const char* const file_name)
{
    assert (file_name);

    FILE* const input_file = fopen (file_name, "rb");
    assert (input_file);

    fseek (input_file, 0, SEEK_END);
    const size_t file_size = ftell (input_file);
    fseek (input_file, 0, SEEK_SET);

    char* const input = (char*)malloc (file_size);
    assert (input);

    fread (input, sizeof (char), file_size, input_file);

    fclose (input_file);
    return input;
}

static int*
ReadInputArrayFromStr (char* str,
                       const size_t elem_number)
{
    assert (str);

    int* const array = (int*) malloc (elem_number * sizeof (int));
    assert (array);

    for (size_t i = 0; i < elem_number; i++)
        array[i] = strtod (str, &str);

    return array;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
