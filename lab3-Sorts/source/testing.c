#include "../include/testing.h"

const size_t TEST_FILE_NAME_MAX_LEN = 0x10;
const size_t NUMBER_MAX_LEN_IN_CHAR = 0x10;

static void
ReadTestFile (struct file_names*     const names,
              struct file_input_str* const file_input,
              char* const buffer);

static void
ReadAnswerFile (struct file_names*     const names,
                struct file_input_str* const file_input,
                char* const buffer);

static char*
ReadFileToBuffer (FILE* const file,
                  char* const buffer);

void
FillArray (int* const array,
           char* file_input,
           size_t* const elem_number);

static void
CheckArray (const int* const array,
            const size_t cur_size,
            const size_t cur_test,
            struct file_input_str* const file_input,
            size_t const elem_number);

static void
SetTestNames (struct file_names* const names,
              const size_t cur_size,
              const size_t cur_test_num);

static void
EndCurrentTest (struct file_input_str* const file_input);

void
TestSort (const char*  const test_folder,
          const char*  const output_file_name,
          const size_t from,
          const size_t   to,
          const size_t step,
          const size_t size_tests_num,
          void (*sort) (int* const, size_t))
{
    assert (test_folder);
    assert (output_file_name);
    assert (sort);

    struct file_names* names =
        FileNamesConstructor (test_folder);
    assert (names);

    struct file_input_str* file_input =
        FileInputConstructor ();
    assert (file_input);

    FILE* output = fopen (output_file_name, "wb");
    assert (output);

    int* const array =
        (int* const) malloc (to * sizeof (int));
    assert (array);

    char* const buffer =
        (char* const) malloc (to * NUMBER_MAX_LEN_IN_CHAR);
    assert (buffer);

    size_t elem_number = 0;
    clock_t sort_begin = 0;
    clock_t sort_end   = 0;

    // const size_t total_test_number = (to - from + 1) / step * size_tests_num;
    // size_t cur_test_cnt = 0;

    for (size_t cur_size = from; cur_size <= to; cur_size += step)
    {
        for (size_t cur_test = 0; cur_test < size_tests_num; ++cur_test)
        {
            SetTestNames  (names, cur_size, cur_test);
            ReadTestFile  (names, file_input, buffer);
            FillArray (array, buffer, &elem_number);

            sort_begin = clock ();
            sort (array, elem_number);
            sort_end   = clock ();

            ReadAnswerFile (names, file_input, buffer);
            CheckArray (array, cur_test, cur_size, file_input, elem_number);

            fprintf (output, "%zd %lg\n", cur_size,
                     (double) (sort_end - sort_begin) / CLOCKS_PER_SEC);
            // fprintf (stderr, "Test %zd of %zd\n", cur_test_cnt++, total_test_number);

            EndCurrentTest (file_input);
        }
    }

    names      = FileNamesDestructor    (names);
    file_input = FileInputDestructor    (file_input);
    fclose (output);
    free   (array);
    free   (buffer);
}

struct file_names*
FileNamesConstructor (const char* const test_folder)
{
    assert (test_folder);

    struct file_names* const names =
        (struct file_names* const) malloc (sizeof (struct file_names));
    assert (names);

    names->test_folder_name_size = strlen (test_folder);
    const size_t test_file_name_size =
        names->test_folder_name_size + TEST_FILE_NAME_MAX_LEN;

    names->test   = (char*) malloc (test_file_name_size);
    assert (names->test);

    names->answer = (char*) malloc (test_file_name_size);
    assert (names->answer);

    strncpy (names->test,   test_folder, names->test_folder_name_size);
    strncpy (names->answer, test_folder, names->test_folder_name_size);

    return names;
}

struct file_input_str*
FileInputConstructor (void)
{
    struct file_input_str* const file_input =
        (struct file_input_str* const) calloc (1, sizeof (struct file_input_str));
    assert (file_input);

    // calloc automaticly fills everything with zeroes

    return file_input;
}

static void
ReadTestFile (struct file_names*     const names,
              struct file_input_str* const file_input,
              char* const buffer)
{
    assert (names);
    assert (file_input);
    assert (buffer);

    FILE* test = fopen (names->test, "rb");
    assert (test);

    file_input->test = ReadFileToBuffer (test, buffer);

    fclose (test);
}

static void
ReadAnswerFile (struct file_names*     const names,
                struct file_input_str* const file_input,
                char* const buffer)
{
    assert (names);
    assert (file_input);
    assert (buffer);

    FILE* answer = fopen (names->answer, "rb");
    assert (answer);

    file_input->answer = ReadFileToBuffer (answer, buffer);

    fclose (answer);
}

static char*
ReadFileToBuffer (FILE* const file,
                  char* const buffer)
{
    assert (file);
    assert (buffer);

    size_t num_of_bytes = 0;

    fseek (file, 0, SEEK_END);
    num_of_bytes = ftell (file);
    fseek (file, 0, SEEK_SET);

    fread (buffer, sizeof (char), num_of_bytes, file);

    return buffer;
}

void
FillArray (int* const array,
           char* input,
           size_t* const elem_number)
{
    assert (array);
    assert (input);
    assert (elem_number);

    *elem_number = strtol (input, &input, 10);
    if (*elem_number == 0) return;

    for (size_t i = 0; i < *elem_number; ++i)
        array[i] = strtol (input, &input, 10);
}

static void
CheckArray (const int* const array,
            const size_t cur_size,
            const size_t cur_test,
            struct file_input_str* const file_input,
            size_t const elem_number)
{
    assert (elem_number == 0 || array);
    assert (file_input);

    int check_elem = 0;

    for (size_t i = 0; i < elem_number; ++i)
    {
        check_elem = strtol (file_input->answer, &file_input->answer, 10);

        if (array[i] != check_elem)
        {
            fprintf (stderr, "cur_test = %zd, cur_size = %zd\n",
                     cur_test, cur_size);
            fprintf (stderr, "awaited %d, recieved %d\n",
                     check_elem, array[i]);
            assert (0);
        }
    }
}

static void
SetTestNames (struct file_names* const names,
              const size_t cur_size,
              const size_t cur_test_num)
{
    assert (names);

    sprintf (names->test + names->test_folder_name_size,
             "/%zd_%zd.in", cur_size, cur_test_num);

    sprintf (names->answer + names->test_folder_name_size,
             "/%zd_%zd.out", cur_size, cur_test_num);
}

static void
EndCurrentTest (struct file_input_str* const file_input)
{
    assert (file_input);

    file_input->test   = NULL;
    file_input->answer = NULL;
}

struct file_names*
FileNamesDestructor (struct file_names* const names)
{
    assert (names);

    free (names->test);
    free (names->answer);

    names->test   = NULL;
    names->answer = NULL;
    names->output = NULL;
    names->test_folder_name_size = 0;

    free (names);

    return NULL;
}

struct file_input_str*
FileInputDestructor (struct file_input_str* const file_input)
{
    assert (file_input);

    free (file_input->test);
    free (file_input->answer);
    file_input->test   = NULL;
    file_input->answer = NULL;

    free (file_input);

    return NULL;
}
