#ifndef TESTING_H
#define TESTING_H

#include "quadratic.h"
#include "heapsort.h"
#include "mergesort.h"
#include "qsort.h"
// #include "qsort_optimization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct file_names
{
    size_t test_folder_name_size;
    const char* output;
    char* test;
    char* answer;
};

struct file_input_str
{
    char* test;
    char* answer;
    size_t   test_str_index;
    size_t answer_str_index;
};

void
TestSort (const char*  const test_folder,
          const char*  const output_file_name,
          const size_t from,
          const size_t   to,
          const size_t step,
          const size_t size_tests_num,
          void (*sort) (int* const, size_t));

struct file_names*
FileNamesConstructor (const char* const test_folder);

struct file_input_str*
FileInputConstructor (void);

struct file_names*
FileNamesDestructor (struct file_names* const names);

struct file_input_str*
FileInputDestructor (struct file_input_str* const file_input);

#endif
