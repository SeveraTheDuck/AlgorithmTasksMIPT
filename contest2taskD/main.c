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

second_level_hash_table*
SecondLevelHashTableConstructor (const size_t keys_number);

hash_function_coefs*
HashFunctionCoefsConstructor (void);

fixed_set*
FixedSetDestructor (fixed_set* const set);

second_level_hash_table**
FixedSetTablesArrayDestructor (second_level_hash_table** const tables_array,
                               const size_t keys_number);

second_level_hash_table*
SecondLevelHashTableDestructor (second_level_hash_table* const table);

hash_function_coefs*
HashFunctionCoefsDestructor (hash_function_coefs* const coefs);
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

    set->coefs = HashFunctionCoefsConstructor ();
    if (set->coefs == NULL)
        return FixedSetDestructor (set);
//sep func begin
    size_t* const collisions_num_array =
        (size_t*) calloc (keys_number, sizeof (size_t));
    if (collisions_num_array == NULL)
        return FixedSetDestructor (set);

    bool function_is_ok =
        CheckHashFunction (keys_array, keys_number,
                           collisions_num_array, set->coefs);

    while (function_is_ok == false)
    {
        HashFunctionSetNewCoefs (set->coefs);
        function_is_ok = CheckFirstLevelHashFunction (keys_array, keys_number,
                                                      collisions_num_array,
                                                      set->coefs);
    }
// sep func end

    set->tables_array = 
        FixedSetTablesArrayConstructor (keys_array, keys_number, 
                                        collisions_num_array, set->coefs);
    if (set->tables_array == NULL)
        return FixedSetDestructor (set);
    
    free (collisions_num_array);

    return set;
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
                                const hash_function_coefs* const coefs)
{
    if (keys_array           == NULL ||
        keys_number          == 0    || 
        collisions_num_array == NULL ||
        coefs                == NULL) 
        return NULL;

    second_level_hash_table** const tables_array = (second_level_hash_table**)
        calloc (keys_number, sizeof (second_level_hash_table*));
    if (tables_array == NULL) return NULL;

    const size_t collisions_max_num = 
        ArrayMaxValue (collisions_num_array, keys_array);

    bool function_is_ok = false;

    for (size_t i = 0; i < keys_number; ++i)
    {
        if (collisions_num_array[i] == 0) continue;
        
        while (function_is_ok == false)
        {
            tables_array[i] = 
                SecondLevelHashTableConstructor (collisions_num_array[i]);
            if (tables_array == NULL)
                return FixedSetTablesArrayDestructor (tables_array, keys_number);

            function_is_ok = CheckAndFillSecondLevelTable ();

            if (function_is_ok) break;

            tables_array[i] =
                SecondLevelHashTableDestructor (tables_array[i]);
        }
        
        function_is_ok = false;
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
SecondLevelHashTableConstructor (const size_t keys_number)
{
    if (keys_number == 0) return NULL;

    second_level_hash_table* const table = (second_level_hash_table*)
        calloc (1, sizeof (second_level_hash_table));
    if (table == NULL) return NULL;

    table->elem_number = keys_number * keys_number;

    table->coefs = HashFunctionCoefsConstructor ();
    if (table->coefs == NULL)
        return SecondLevelHashTableDestructor (table);

    table->data_array = (hash_table_data*)
        calloc (table->elem_number, sizeof (hash_table_data));
    if (table->data_array == NULL)
        return SecondLevelHashTableDestructor (table);

    return table;
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
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Main functions
//-----------------------------------------------------------------------------
int main (void)
{
    return 0;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
