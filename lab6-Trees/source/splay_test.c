#include "../include/splay.h"
#include "../include/tests_common.h"



int
SplayTreeKeyCmp (const splay_key* const a,
                 const splay_key* const b)
{
    assert (a);
    assert (b);

    const int a_val = *(const int*)a->key;
    const int b_val = *(const int*)b->key;

    if (a_val < b_val) return SPLAY_TREE_CMP_LESS;
    if (a_val > b_val) return SPLAY_TREE_CMP_GREATER;
    return SPLAY_TREE_CMP_EQUAL;
}

void
SplayTreeKeySetData (splay_key* const key,
                     const int data)
{
    assert (key);
    *(int*)key->key = data;
}

void
SplayTreeTestInserts (splay_tree* const tree,
                      splay_key* const key,
                      const input_array* const array,
                      const char* const output_file_name)
{
    FILE* const output_file = fopen (output_file_name, "ab");
    assert (output_file);

    const size_t elem_number = array->elem_number;

    clock_t begin = 0, end = 0;

    begin = clock ();
    for (size_t i = 0; i < elem_number; ++i)
    {
        SplayTreeKeySetData (key, array->array[i]);
        SplayTreeDeleteKey  (tree, key);
    }
    end = clock ();

    fprintf (output_file, "%zu %lu\n", elem_number, end - begin);
    fclose (output_file);
}

void
SplayTreeTestDeletes (splay_tree* const tree,
                      splay_key* const key,
                      const input_array* const array,
                      const char* const output_file_name)
{
    FILE* const output_file = fopen (output_file_name, "ab");
    assert (output_file);

    const size_t elem_number = array->elem_number;
    const size_t op_number   = elem_number / 2;

    clock_t begin = 0, end = 0;

    begin = clock ();
    for (size_t i = 0; i < op_number; ++i)
    {
        SplayTreeKeySetData (key, GetRandomElement (array));
        SplayTreeInsert     (tree, key, NULL);
    }
    end = clock ();

    fprintf (output_file, "%zu %lu\n", elem_number, end - begin);
    fclose (output_file);
}



int main (const int argc, const char** const argv)
{
    CheckCommandLineArguments (argc, argv);

    input_array* array = ReadInputFile (argv[INPUT_FILE_NAME_INDEX]);
    assert (array);

    splay_tree* tree = SplayTreeConstructor (SplayTreeKeyCmp);
    assert (tree);

    splay_key* key = SplayKeyConstructor (&array->array[0], sizeof (int));
    assert (key);

    SplayTreeTestInserts (tree, key, array, argv[INSERTS_OUTPUT_FILE_NAME_INDEX]);

    SplayTreeTestDeletes (tree, key, array, argv[DELETES_OUTPUT_FILE_NAME_INDEX]);

    array = InputArrayDestructor (array);
    key   = SplayKeyDestructor   (key);
    tree  = SplayTreeDestructor  (tree);
    return 0;
}