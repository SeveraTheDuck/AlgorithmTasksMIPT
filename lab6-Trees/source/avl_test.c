#include "../include/avl_tree.h"
#include "../include/tests_common.h"



int
AVLTreeKeyCmp (const avl_tree_key* const a,
               const avl_tree_key* const b)
{
    assert (a);
    assert (b);

    const int a_val = *(const int*)a->data;
    const int b_val = *(const int*)b->data;

    if (a_val < b_val) return AVL_TREE_CMP_LESS;
    if (a_val > b_val) return AVL_TREE_CMP_GREATER;
    return AVL_TREE_CMP_EQUAL;
}

void
AVLTreeKeySetData (avl_tree_key* const key,
                   const int data)
{
    assert (key);
    *(int*)key->data = data;
}

void
AVLTreeTestInserts (avl_tree* const tree,
                    avl_tree_key* const key,
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
        AVLTreeKeySetData (key, array->array[i]);
        AVLTreeInsert     (tree, key, NULL);
    }
    end = clock ();

    fprintf (output_file, "%zu %lg\n", elem_number,
            (double)(end - begin) / CLOCKS_PER_SEC);
    fclose (output_file);
}

void
AVLTreeTestDeletes (avl_tree* const tree,
                    avl_tree_key* const key,
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
        AVLTreeKeySetData (key, array->array[i]);
        AVLTreeDelete     (tree, key);
    }
    end = clock ();

    fprintf (output_file, "%zu %lg\n", elem_number,
            (double)(end - begin) / CLOCKS_PER_SEC);
    fclose (output_file);
}



int main (const int argc, const char** const argv)
{
    CheckCommandLineArguments (argc, argv);

    input_array* array = ReadInputFile (argv[INPUT_FILE_NAME_INDEX]);
    assert (array);

    avl_tree* tree = AVLTreeConstructor (AVLTreeKeyCmp);
    assert (tree);

    avl_tree_key* key = AVLTreeKeyConstructor (&array->array[0], sizeof (int));
    assert (key);

    AVLTreeTestInserts (tree, key, array, argv[INSERTS_OUTPUT_FILE_NAME_INDEX]);

    AVLTreeTestDeletes (tree, key, array, argv[DELETES_OUTPUT_FILE_NAME_INDEX]);

    array = InputArrayDestructor (array);
    key   = AVLTreeKeyDestructor (key);
    tree  = AVLTreeDestructor    (tree);
    return 0;
}