#pragma once



#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



//-----------------------------------------------------------------------------
// Constants for command line arguments
//-----------------------------------------------------------------------------
extern const size_t INPUT_FILE_NAME_INDEX;
extern const size_t INSERTS_OUTPUT_FILE_NAME_INDEX;
extern const size_t DELETES_OUTPUT_FILE_NAME_INDEX;
extern const size_t FILES_NUMBER;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Input array structure
//-----------------------------------------------------------------------------
typedef struct input_array
{
    int*   array;
    size_t elem_number;
}
input_array;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Input reading functions
//-----------------------------------------------------------------------------
input_array*
ReadInputFile (const char* const file_name);

input_array*
InputArrayConstructor (int* const array,
                       const size_t elem_number);

input_array*
InputArrayDestructor (input_array* const input_array);

int
GetRandomElement (const input_array* const input_array);

void
CheckCommandLineArguments (const int argc, const char** const argv);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
