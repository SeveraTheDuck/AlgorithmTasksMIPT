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
struct key
{
    void*  key_data;
    size_t key_size;
}
key;

typedef
struct value
{
    void*  value_data;
    size_t value_size;
}
value;

typedef
struct hash_table_data
{
    key*   key;
    value* value;
    bool   is_empty;
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
FixedSetConstructor (const size_t keys_number,
                     const key** const keys_array);

second_level_hash_table**
FixedSetTablesArrayConstructor (const size_t keys_number,
                                const hash_function_coefs* const coefs);

second_level_hash_table*
SecondLevelHashTableConstructor (const size_t keys_number);

hash_function_coefs*
HashFunctionCoefsConstructor (void);

hash_table_data*
HashTableDataConstructor (const void* const key,
                          const void* const value,
                          const size_t key_size,
                          const size_t value_size);

fixed_set*
FixedSetDestructor (fixed_set* const set);

second_level_hash_table**
FixedSetTablesArrayDestructor (second_level_hash_table** const tables_array);

second_level_hash_table*
SecondLevelHashTableDestructor (second_level_hash_table* const table);

hash_function_coefs*
HashFunctionCoefsDestructor (hash_function_coefs* const coefs);

hash_table_data*
HashTableDataDestructor (hash_table_data* const data);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Hash table functions' implementation
//-----------------------------------------------------------------------------
fixed_set*
FixedSetConstructor (const size_t keys_number,
                     const key** const keys_array)
{
    if (keys_number == 0 || keys_array == NULL) return NULL;

    fixed_set* const set =
        (fixed_set*) calloc (1, sizeof (fixed_set));
    if (set == NULL) return NULL;

    set->keys_number = keys_number;

    set->coefs = HashFunctionCoefsConstructor ();
    if (set->coefs == NULL)
        return FixedSetDestructor (set);

    size_t* const collisions_num =
        (size_t*) calloc (keys_number, sizeof (size_t));
    if (collisions_num == NULL)
        return FixedSetDestructor (set);

    bool function_is_ok =
        CheckHashFunction (keys_number, keys_array,
                           collisions_num, set->coefs);

    while (function_is_ok == false)
    {
        memset (collisions_num, 0, keys_number * sizeof (size_t));
        HashFunctionSetNewCoefs (set->coefs);

        function_is_ok = CheckHashFunction (keys_number, keys_array,
                                            collisions_num, set->coefs);
    }

    set->tables_array = 
        FixedSetTablesArrayConstructor (keys_number, set->coefs);
    if (set->tables_array == NULL)
        return FixedSetDestructor (set);
    
    free (collisions_num);

    return set;
}

fixed_set*
FixedSetDestructor (fixed_set* const set)
{
    if (set == NULL) return NULL;

    if (set->coefs) 
        set->coefs = HashFunctionCoefsDestructor (set->coefs);

    if (set->tables_array)
        set->tables_array = FixedSetTablesArrayDestructor (set->tables_array);

    free (set);

    return NULL;
}

second_level_hash_table**
FixedSetTablesArrayConstructor (const size_t keys_number,
                                const hash_function_coefs* const coefs)
{
    if (keys_number == 0 || coefs == NULL) return NULL;

    second_level_hash_table** const tables_array = (second_level_hash_table**)
        calloc (keys_number, sizeof (second_level_hash_table*));
    if (tables_array == NULL) return NULL;


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
