#include "../include/testing.h"

const size_t TEST_FILE_NAME_MAX_LEN = 0x10;

static void
ReadTestFiles (struct file_names*     const names,
               struct file_ptrs*      const files,
               struct file_input_str* const file_input);

static char*
ReadFileToBuffer (FILE* const file);

void
FillArray (int* const array,
           struct file_input_str* const file_input,
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
EndCurrentTest (struct file_ptrs* const files,
                struct file_input_str* const file_input);

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
        FileNamesConstructor (test_folder, output_file_name);
    assert (names);

    struct file_ptrs* files =
        FilePointersConstructor (output_file_name);
    assert (files);

    struct file_input_str* file_input =
        FileInputConstructor ();
    assert (file_input);

    int* const array = (int* const) calloc (to, sizeof (int));
    assert (array);

    size_t elem_number = 0;
    clock_t sort_begin = 0;
    clock_t sort_end   = 0;

    const size_t total_test_number = (to - from) / step;
    size_t cur_test_cnt = 0;

    for (size_t cur_size = from; cur_size <= to; cur_size += step)
    {
        for (size_t cur_test = 0; cur_test < size_tests_num; ++cur_test)
        {
            SetTestNames  (names, cur_size, cur_test);
            ReadTestFiles (names, files, file_input);
            FillArray (array, file_input, &elem_number);

            sort_begin = clock ();
            sort (array, elem_number);
            sort_end   = clock ();

            CheckArray (array, cur_test, cur_size, file_input, elem_number);

            fprintf (files->output, "%zd %zd\n", cur_size, sort_end - sort_begin);
            fprintf (stderr, "Test %zd of %zd\n", cur_test_cnt++, total_test_number);

            EndCurrentTest (files, file_input);
        }
    }

    names      = FileNamesDestructor    (names);
    files      = FilePointersDestructor (files);
    file_input = FileInputDestructor    (file_input);
    free (array);
}

struct file_names*
FileNamesConstructor (const char* const test_folder,
                      const char* const output_file_name)
{
    assert (test_folder);
    assert (output_file_name);

    struct file_names* const names =
        (struct file_names* const) calloc (1, sizeof (struct file_names));
    assert (names);

    names->test_folder_name_size = strlen (test_folder);
    const size_t test_file_name_size =
        names->test_folder_name_size + TEST_FILE_NAME_MAX_LEN;

    names->output = output_file_name;

    names->test   = (char*) calloc (test_file_name_size, sizeof (char));
    assert (names->test);

    names->answer = (char*) calloc (test_file_name_size, sizeof (char));
    assert (names->answer);

    strncpy (names->test,   test_folder, names->test_folder_name_size);
    strncpy (names->answer, test_folder, names->test_folder_name_size);

    return names;
}

struct file_ptrs*
FilePointersConstructor (const char* const output_file_name)
{
    assert (output_file_name);

    struct file_ptrs* const files =
        (struct file_ptrs* const) calloc (1, sizeof (struct file_ptrs));
    assert (files);

    files->output = fopen (output_file_name, "wb");
    assert (files->output);

    files->test   = NULL;
    files->answer = NULL;

    return files;
}

struct file_input_str*
FileInputConstructor ()
{
    struct file_input_str* const file_input =
        (struct file_input_str* const) calloc (1, sizeof (struct file_input_str));
    assert (file_input);

    // calloc automaticly fills everything with zeroes

    return file_input;
}

static void
ReadTestFiles (struct file_names*     const names,
               struct file_ptrs*      const files,
               struct file_input_str* const file_input)
{
    assert (names);
    assert (files);
    assert (file_input);

    files->test = fopen (names->test, "rb");
    assert (files->test);
    file_input->test = ReadFileToBuffer (files->test);
    file_input->test_str_index = 0;

    files->answer = fopen (names->answer, "rb");
    assert (files->answer);
    file_input->answer = ReadFileToBuffer (files->answer);
    file_input->answer_str_index = 0;
}

static char*
ReadFileToBuffer (FILE* const file)
{
    assert (file);

    size_t num_of_bytes = 0;

    fseek (file, 0, SEEK_END);
    num_of_bytes = ftell (file);
    fseek (file, 0, SEEK_SET);

    char* const buffer =
        (char* const) calloc (num_of_bytes + 1, sizeof (char));
    assert (buffer);

    fread (buffer, sizeof (char), num_of_bytes, file);

    return buffer;
}

void
FillArray (int* const array,
           struct file_input_str* const file_input,
           size_t* const elem_number)
{
    assert (array);
    assert (file_input);
    assert (elem_number);

    int scanned_symbols = 0;

    sscanf (file_input->test, "%zd %n", elem_number, &scanned_symbols);
    file_input->test_str_index += scanned_symbols;
    if (*elem_number == 0) return;

    for (size_t i = 0; i < *elem_number; ++i)
    {
        sscanf (file_input->test + file_input->test_str_index,
                "%d %n", &array[i], &scanned_symbols);

        file_input->test_str_index += scanned_symbols;
    }
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
    int scanned_symbols = 0;

    for (size_t i = 0; i < elem_number; ++i)
    {
        sscanf (file_input->answer + file_input->answer_str_index,
                "%d%n", &check_elem, &scanned_symbols);
        file_input->answer_str_index += scanned_symbols;

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
EndCurrentTest (struct file_ptrs* const files,
                struct file_input_str* const file_input)
{
    assert (files);
    assert (file_input);

    fclose (files->test);
    fclose (files->answer);

    free (file_input->test);
    free (file_input->answer);
    file_input->test   = NULL;
    file_input->answer = NULL;
    file_input->test_str_index   = 0;
    file_input->answer_str_index = 0;
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

struct file_ptrs*
FilePointersDestructor (struct file_ptrs* const files)
{
    assert (files);

    files->test   = NULL;
    files->answer = NULL;

    fclose (files->output);
    files->output = NULL;

    free (files);

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
    file_input->test_str_index   = 0;
    file_input->answer_str_index = 0;

    free (file_input);

    return NULL;
}
