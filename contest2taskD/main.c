#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>



//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------
const size_t HASH_PRIME_MODULE = 1000000007;
const size_t HASH_FUNCTION_POISON = (size_t) -1;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Structs and types
//-----------------------------------------------------------------------------
typedef
struct hash_table_data
{
    int  key;
    bool is_filled;
}
hash_table_data;

typedef
struct hash_function_coefs
{
    size_t a;
    size_t b;
}
hash_function_coefs;

typedef
struct second_level_hash_table
{
    size_t               elem_number;   ///< Squared number of keys in this table
    hash_function_coefs* coefs;         ///< a and b coefs for hash function
    hash_table_data*     data_array;    ///< Array with data
}
second_level_hash_table;

typedef
struct fixed_set
{
    size_t                    keys_number;  ///< Total number of keys, number of second level tables
    hash_function_coefs*      coefs;        ///< a and b coefs for hash function
    second_level_hash_table** tables_array; ///< Array with second level tables
}
fixed_set;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Hash table functions' prototypes
//-----------------------------------------------------------------------------
fixed_set*
FixedSetConstructor (const int* const keys_array,
                     const size_t keys_number);

second_level_hash_table**
FixedSetTablesArrayConstructor (const int* const keys_array,
                                const size_t keys_number,
                                const size_t* const collisions_num_array,
                                const hash_function_coefs* const coefs);

size_t*
SetFirstLevelHashFunction (const int* const keys_array,
                           const size_t keys_number,
                           fixed_set* const set);

bool
CheckFirstLevelHashFunction (const int* const keys_array,
                             const size_t keys_number,
                             size_t* const collisions_num_array,
                             hash_function_coefs* const coefs);

second_level_hash_table*
SecondLevelHashTableConstructor (const int* const keys_array,
                                 const size_t keys_number,
                                 const size_t collisions_number,
                                 const hash_function_coefs* const first_level_coefs,
                                 const size_t table_index);

hash_function_coefs*
SetSecondLevelHashFunction (const int* const keys_array,
                            const size_t keys_number,
                            second_level_hash_table* const table,
                            const hash_function_coefs* const first_level_coefs,
                            const size_t table_index);

hash_function_coefs*
HashFunctionCoefsConstructor (void);

void
HashFunctionSetNewCoefs (hash_function_coefs* const coefs);

fixed_set*
FixedSetDestructor (fixed_set* const set);

second_level_hash_table**
FixedSetTablesArrayDestructor (second_level_hash_table** const tables_array,
                               const size_t keys_number);

second_level_hash_table*
SecondLevelHashTableDestructor (second_level_hash_table* const table);

hash_function_coefs*
HashFunctionCoefsDestructor (hash_function_coefs* const coefs);

size_t
HashFunction (const int key,
              const size_t keys_number,
              const hash_function_coefs* const coefs);

bool IsInFixedSet (const int key,
                   const fixed_set* const set);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Hash table functions' implementation
//-----------------------------------------------------------------------------
fixed_set*
FixedSetConstructor (const int* const keys_array,
                     const size_t keys_number)
{
    if (keys_number == 0 || keys_array == NULL) return NULL;

    fixed_set* const set =
        (fixed_set*) calloc (1, sizeof (fixed_set));
    if (set == NULL) return NULL;

    set->keys_number = keys_number;

    size_t* const collisions_num_array =
        SetFirstLevelHashFunction (keys_array, keys_number, set);
    if (collisions_num_array == NULL)
        return FixedSetDestructor (set);

    set->tables_array =
        FixedSetTablesArrayConstructor (keys_array, keys_number,
                                        collisions_num_array, set->coefs);
    if (set->tables_array == NULL)
    {
        free (collisions_num_array);
        return FixedSetDestructor (set);
    }

    free (collisions_num_array);

    return set;
}

size_t*
SetFirstLevelHashFunction (const int* const keys_array,
                           const size_t keys_number,
                           fixed_set* const set)
{
    if (keys_array  == NULL ||
        keys_number == 0    ||
        set         == NULL)
        return NULL;

    set->coefs = HashFunctionCoefsConstructor ();
    if (set->coefs == NULL)
        return NULL;

    size_t* const collisions_num_array =
        (size_t*) calloc (keys_number, sizeof (size_t));
    if (collisions_num_array == NULL)
        return NULL;

    bool function_is_ok = CheckFirstLevelHashFunction (keys_array, keys_number,
                                                       collisions_num_array,
                                                       set->coefs);

    while (function_is_ok == false)
    {
        HashFunctionSetNewCoefs (set->coefs);
        memset (collisions_num_array, 0, keys_number * sizeof (size_t));
        function_is_ok = CheckFirstLevelHashFunction (keys_array, keys_number,
                                                      collisions_num_array,
                                                      set->coefs);
    }

    return collisions_num_array;
}

bool
CheckFirstLevelHashFunction (const int* const keys_array,
                             const size_t keys_number,
                             size_t* const collisions_num_array,
                             hash_function_coefs* const coefs)
{
    if (keys_array           == NULL ||
        keys_number          == 0    ||
        collisions_num_array == NULL ||
        coefs                == NULL)
        return false;

    size_t hash_index = 0;

    for (size_t i = 0; i < keys_number; ++i)
    {
        hash_index = HashFunction (keys_array[i], keys_number, coefs);
        collisions_num_array[hash_index]++;
    }

    size_t total_col_squares = 0;
    for (size_t i = 0; i < keys_number; ++i)
        total_col_squares += collisions_num_array[i] *
                             collisions_num_array[i];

    if (total_col_squares > 4 * keys_number)
        return false;

    return true;
}

fixed_set*
FixedSetDestructor (fixed_set* const set)
{
    if (set == NULL) return NULL;

    if (set->coefs)
        set->coefs = HashFunctionCoefsDestructor (set->coefs);

    if (set->tables_array)
        set->tables_array = FixedSetTablesArrayDestructor (set->tables_array,
                                                           set->keys_number);

    free (set);

    return NULL;
}

second_level_hash_table**
FixedSetTablesArrayConstructor (const int* const keys_array,
                                const size_t keys_number,
                                const size_t* const collisions_num_array,
                                const hash_function_coefs* const first_level_coefs)
{
    if (keys_array           == NULL ||
        keys_number          == 0    ||
        collisions_num_array == NULL ||
        first_level_coefs    == NULL)
        return NULL;

    second_level_hash_table** const tables_array = (second_level_hash_table**)
        calloc (keys_number, sizeof (second_level_hash_table*));
    if (tables_array == NULL) return NULL;

    for (size_t i = 0; i < keys_number; ++i)
    {
        if (collisions_num_array[i] == 0) continue;

        tables_array[i] =
            SecondLevelHashTableConstructor (keys_array, keys_number,
                                             collisions_num_array[i],
                                             first_level_coefs, i);
        if (tables_array[i] == NULL)
            return FixedSetTablesArrayDestructor (tables_array, keys_number);
    }

    return tables_array;
}

second_level_hash_table**
FixedSetTablesArrayDestructor (second_level_hash_table** const tables_array,
                               const size_t keys_number)
{
    if (tables_array == NULL) return NULL;

    for (size_t i = 0; i < keys_number; ++i)
        tables_array[i] = SecondLevelHashTableDestructor (tables_array[i]);

    free (tables_array);
    return NULL;
}

second_level_hash_table*
SecondLevelHashTableConstructor (const int* const keys_array,
                                 const size_t keys_number,
                                 const size_t collisions_number,
                                 const hash_function_coefs* const first_level_coefs,
                                 const size_t table_index)
{
    if (keys_array        == NULL ||
        keys_number       == 0    ||
        collisions_number == 0    ||
        first_level_coefs == NULL)
        return NULL;

    second_level_hash_table* const table = (second_level_hash_table*)
        calloc (1, sizeof (second_level_hash_table));
    if (table == NULL) return NULL;

    table->elem_number = collisions_number * collisions_number;

    table->data_array = (hash_table_data*)
        calloc (table->elem_number, sizeof (hash_table_data));
    if (table->data_array == NULL)
        return SecondLevelHashTableDestructor (table);

    table->coefs =
        SetSecondLevelHashFunction (keys_array, keys_number, table,
                                    first_level_coefs, table_index);
    if (table->coefs == NULL)
        return SecondLevelHashTableDestructor (table);

    return table;
}

hash_function_coefs*
SetSecondLevelHashFunction (const int* const keys_array,
                            const size_t keys_number,
                            second_level_hash_table* const table,
                            const hash_function_coefs* const first_level_coefs,
                            const size_t table_index)
{
    if (keys_array        == NULL ||
        keys_number       == 0    ||
        table             == NULL ||
        first_level_coefs == NULL)
        return NULL;

    bool function_is_ok = false;
    size_t new_elem_index = 0;

    hash_function_coefs* const coefs = HashFunctionCoefsConstructor ();
    if (coefs == NULL) return NULL;

    while (function_is_ok == false)
    {
        function_is_ok = true;

        for (size_t i = 0; i < keys_number; ++i)
        {
            if (HashFunction (keys_array[i], keys_number,
                              first_level_coefs) == table_index)
            {
                new_elem_index =
                    HashFunction (keys_array[i], table->elem_number, coefs);

                if (table->data_array[new_elem_index].is_filled == false)
                {
                    table->data_array[new_elem_index].key = keys_array[i];
                    table->data_array[new_elem_index].is_filled = true;
                }

                else
                {
                    function_is_ok = false;
                    memset (table->data_array, 0,
                            table->elem_number * sizeof (hash_table_data));

                    break;
                }
            }
        }

        if (function_is_ok == false)
            HashFunctionSetNewCoefs (coefs);
    }

    return coefs;
}

second_level_hash_table*
SecondLevelHashTableDestructor (second_level_hash_table* const table)
{
    if (table == NULL) return NULL;

    if (table->coefs)
        table->coefs = HashFunctionCoefsDestructor (table->coefs);

    if (table->data_array)
        free (table->data_array);

    free (table);
    return NULL;
}

hash_function_coefs*
HashFunctionCoefsConstructor (void)
{
    hash_function_coefs* const coefs = (hash_function_coefs*)
        calloc (1, sizeof (hash_function_coefs));
    if (coefs == NULL) return NULL;

    HashFunctionSetNewCoefs (coefs);

    return coefs;
}

void
HashFunctionSetNewCoefs (hash_function_coefs* const coefs)
{
    if (coefs == NULL) return;

    coefs->a = 1 + rand () % HASH_PRIME_MODULE;
    coefs->b =     rand () % HASH_PRIME_MODULE;
}

hash_function_coefs*
HashFunctionCoefsDestructor (hash_function_coefs* const coefs)
{
    if (coefs == NULL) return NULL;
    free (coefs);
    return NULL;
}

size_t
HashFunction (const int key,
              const size_t keys_number,
              const hash_function_coefs* const coefs)
{
    if (coefs == NULL)
        return HASH_FUNCTION_POISON;

    return (coefs->a * key + coefs->b) % HASH_PRIME_MODULE % keys_number;
}

bool IsInFixedSet (const int key,
                   const fixed_set* const set)
{
    if (set == NULL) return false;

    const size_t first_lvl_hash =
        HashFunction (key, set->keys_number, set->coefs);

    const second_level_hash_table* table = set->tables_array[first_lvl_hash];
    if (table == NULL) return false;

    const size_t second_lvl_hash =
        HashFunction (key, table->elem_number, table->coefs);

    if (table->data_array[second_lvl_hash].is_filled &&
        table->data_array[second_lvl_hash].key == key)
        return true;

    return false;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Main functions
//-----------------------------------------------------------------------------
const char END_OF_INPUT = '.';
const char* const YES_STR = "YES";
const char* const NO_STR  = "NO";

int*
ReadArray (const size_t N);

int main (void)
{
    size_t N = 0;
    assert (scanf ("%zd", &N) == 1);

    int* const a = ReadArray (N);
    assert (a);

    srand (time (NULL));
    fixed_set* set = FixedSetConstructor (a, N);

    int input = 0;

    while (scanf ("%d", &input) == 1)
    {
        if (IsInFixedSet (input, set))
            printf ("%s\n", YES_STR);

        else printf ("%s\n", NO_STR);
    }

    if (getchar() != END_OF_INPUT)
        assert (0);

    set = FixedSetDestructor (set);
    free (a);
    return 0;
}

int*
ReadArray (const size_t N)
{
    assert (N > 0);

    int* const a = (int*) calloc (N, sizeof (int));
    assert (a);

    for (size_t i = 0; i < N; ++i)
        assert (scanf ("%d", &a[i]) == 1);

    return a;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
