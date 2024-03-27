#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>



//-----------------------------------------------------------------------------
// Common const values
//-----------------------------------------------------------------------------
const char   POISON_VALUE = 0;
const size_t HASH_TABLE_RESIZE_MULTIPLIER = 2;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// List struct
//-----------------------------------------------------------------------------
struct list
{
    struct list_node* head;
    struct list_node* tail;
    size_t size;
    size_t elem_size;
};

enum list_error_status
{
    LIST_SUCCESS = 0,
    LIST_ERROR   = 1
};

typedef int list_error_t;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// List node struct
//-----------------------------------------------------------------------------
struct list_node
{
    struct list_node* next;
    struct list_node* prev;
    void*  data;
    size_t elem_size;
};

enum list_node_error_status
{
    LIST_NODE_SUCCESS = 0,
    LIST_NODE_ERROR   = 1
};

typedef int list_node_error_t;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Hash table struct and erorrs.
//-----------------------------------------------------------------------------
struct hash_table
{
    struct list** list_array;   ///< array of lists
    size_t size;                ///< number of elements in the hash table
    size_t capacity;            ///< number of keys in the hash table

    size_t (*hash_function) (const void* const key,
                             const size_t capacity);
};

struct hash_table_pair
{
    void* key;
    void* value;

    size_t key_size;    ///< size of key in bytes
    size_t value_size;  ///< size of value in bytes
};

enum hash_table_pair_cmp_status
{
    HASH_TABLE_PAIRS_EQUAL     = 0,
    HASH_TABLE_PAIRS_NOT_EQUAL = 1
};

enum hash_table_error_status
{
    HASH_TABLE_SUCCESS = 0,
    HASH_TABLE_ERROR   = 1
};

typedef int hash_table_error_t;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// List functions' prototypes
//-----------------------------------------------------------------------------
struct list*
ListConstructor (size_t elem_size);

struct list*
ListDestructor (struct list* const list);

// insert after given node
struct list_node*
ListInsertElement (struct list* const list,
                   struct list_node* const prev,
                   const  void* const data);

list_error_t
ListDeleteElement (struct list* const list,
                   struct list_node* const node);

struct list_node*
ListFindElement (struct list* const list,
                 const  void* const data);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// List node functions' prototypes
//-----------------------------------------------------------------------------
struct list_node*
ListNodeConstructor (struct list_node* const prev,
                     struct list_node* const next,
                     const  void* const data,
                     const  size_t elem_size);

struct list_node*
ListNodeDestructor (struct list_node* const node);

// O(1) because of double-linked list
list_error_t
ListNodeDelete (struct list_node* const node);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Hash table functions' prototypes
//-----------------------------------------------------------------------------
struct hash_table*
HashTableConstructor (const size_t capacity,
                      size_t (*hash_function) (const void* const key,
                                               const size_t capacity));

struct hash_table*
HashTableDestructor (struct hash_table* const table);

struct list_node*
HashTableAddElement (struct hash_table* const table,
                     const struct hash_table_pair* const data);

hash_table_error_t
HashTableDeleteElement (struct hash_table* const table,
                        struct list_node* const data);

struct list_node*
HashTableFindElement (struct hash_table* const table,
                      const struct hash_table_pair* const data);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Hash table pair functions' prototypes
//-----------------------------------------------------------------------------
struct hash_table_pair*
HashTablePairConstructor (const void* const key,
                          const void* const value,
                          const size_t key_size,
                          const size_t value_size);

struct hash_table_pair*
HashTablePairDestructor (struct hash_table_pair* const pair);

int
HashTablePairCompare (const struct hash_table_pair* const pair1,
                      const struct hash_table_pair* const pair2);

void*
HashTablePairKey (struct hash_table_pair* const pair);

void*
HashTablePairValue (struct hash_table_pair* const pair);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// List functions' implementation
//-----------------------------------------------------------------------------
struct list*
ListConstructor (size_t elem_size)
{
    if (elem_size == 0) return NULL;

    struct list* list =
        (struct list*) calloc (1, sizeof (struct list));
    if (list == NULL) return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->elem_size = elem_size;

    return list;
}

struct list*
ListDestructor (struct list* const list)
{
    if (list == NULL) return NULL;

    struct list_node* cur  = list->head;
    struct list_node* next = NULL;

    while (cur != NULL)
    {
        next = cur->next;
        ListNodeDestructor (cur);
        cur = next;
    }

    memset (list, POISON_VALUE, sizeof (struct list));
    free (list);

    return NULL;
}

struct list_node*
ListInsertElement (struct list* const list,
                   struct list_node* const prev,
                   const void* const data)
{
    if (list == NULL || data == NULL) return NULL;

    struct list_node* new_node = NULL;

    if (prev == NULL)
        new_node = ListNodeConstructor (NULL, list->head, data, list->elem_size);
    else
        new_node = ListNodeConstructor (prev, prev->next, data, list->elem_size);

    if (new_node == NULL) return NULL;

    if (new_node->prev == NULL) list->head = new_node;
    if (new_node->next == NULL) list->tail = new_node;

    list->size++;

    return new_node;
}

list_error_t
ListDeleteElement (struct list* const list,
                   struct list_node* const node)
{
    if (list == NULL || node == NULL) return LIST_ERROR;

    if (list->head == node) list->head = node->next;
    if (list->tail == node) list->tail = node->prev;

    if (ListNodeDelete (node) == LIST_NODE_ERROR) return LIST_ERROR;

    list->size--;

    return LIST_SUCCESS;
}

// Add cmp function param?
struct list_node*
ListFindElement (struct list* const list,
                 const  void* const data)
{
    if (list == NULL || data == NULL) return NULL;

    struct list_node* cur = list->head;

    while (cur != NULL)
    {
        if (HashTablePairCompare (cur->data, data) == HASH_TABLE_PAIRS_EQUAL) // here use cmp
            return cur;
        cur = cur->next;
    }

    return NULL;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// List node functions' implementation
//-----------------------------------------------------------------------------
struct list_node*
ListNodeConstructor (struct list_node* const prev,
                     struct list_node* const next,
                     const  void* const data,
                     const  size_t elem_size)
{
    struct list_node* const node =
        (struct list_node* const) calloc (1, sizeof (struct list_node));
    if (node == NULL) return NULL;

    node->data = calloc (1, elem_size);
    if (node->data == NULL)
    {
        free (node);
        return NULL;
    }

    if (data != NULL)
        memcpy (node->data, data, elem_size);

    if (prev != NULL)
    {
        prev->next = node;
        node->prev = prev;
    }

    if (next != NULL)
    {
        node->next = next;
        next->prev = node;
    }

    node->elem_size = elem_size;

    return node;
}

struct list_node*
ListNodeDestructor (struct list_node* const node)
{
    if (node == NULL) return NULL;

    if (node->data != NULL)
        memset (node->data, POISON_VALUE, node->elem_size);

    free (node->data);

    memset (node, POISON_VALUE, sizeof (struct list_node));
    free (node);

    return NULL;
}

list_node_error_t
ListNodeDelete (struct list_node* const node)
{
    if (node == NULL) return LIST_NODE_ERROR;

    struct list_node* prev = node->prev;
    struct list_node* next = node->next;

    if (prev != NULL) prev->next = next;
    if (next != NULL) next->prev = prev;

    ListNodeDestructor (node);

    return LIST_NODE_SUCCESS;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Hash table functions' implementation
//-----------------------------------------------------------------------------
struct hash_table*
HashTableConstructor (const size_t capacity,
                      size_t (*hash_function) (const void* const key,
                                               const size_t capacity))
{
    struct hash_table* const table =
        (struct hash_table* const) calloc (1, sizeof (struct hash_table));
    if (table == NULL) return NULL;

    table->size          = 0;
    table->capacity      = capacity;
    table->hash_function = hash_function;

    table->list_array =
        (struct list**) calloc (capacity, sizeof (struct list*));
    if (table->list_array == NULL)
    {
        free (table);
        return NULL;
    }

    return table;
}

struct hash_table*
HashTableDestructor (struct hash_table* const table)
{
    if (table == NULL) return NULL;
    if (table->list_array == NULL)
    {
        memset (table, POISON_VALUE, sizeof (struct hash_table));
        free (table);
        return NULL;
    }

    for (size_t i = 0; i < table->capacity; i++)
        table->list_array[i] = ListDestructor (table->list_array[i]);

    free (table->list_array);
    memset (table, POISON_VALUE, sizeof (struct hash_table));

    free (table);
    return NULL;
}

struct list_node*
HashTableAddElement (struct hash_table* const table,
                     const struct hash_table_pair* const data)
{
    if (table                == NULL ||
        table->list_array    == NULL ||
        table->hash_function == NULL ||
        data                 == NULL ||
        data->key            == NULL ||
        data->value          == NULL)
        return NULL;

    if (HashTableFindElement (table, data) != NULL)
        return NULL;

    const size_t list_index = table->hash_function (data->key, table->capacity);

    if (list_index >= table->capacity) return NULL;

    if (table->list_array[list_index] == NULL)
    {
        table->list_array[list_index] =
            ListConstructor (sizeof (struct hash_table_pair));
        if (table->list_array[list_index] == NULL) return NULL;
    }

    struct hash_table_pair* data_copy =
        HashTablePairConstructor (data->key, data->value, data->key_size, data->value_size);
    if (data_copy == NULL) return NULL;

    struct list_node* const new_node =
        ListInsertElement (table->list_array[list_index],
                           table->list_array[list_index]->tail,
                           data_copy);

    if (new_node == NULL) return NULL;

    table->size++;

    return new_node;
}

hash_table_error_t
HashTableDeleteElement (struct hash_table* const table,
                        struct list_node*  const node)
{
    if (table                == NULL ||
        table->list_array    == NULL ||
        table->hash_function == NULL ||
        node                 == NULL)
        return HASH_TABLE_ERROR;

    const size_t list_index =
        table->hash_function (HashTablePairKey (node->data), table->capacity);
    if (list_index >= table->capacity) return HASH_TABLE_ERROR;

    struct list* list = table->list_array[list_index];
    if (list == NULL) return HASH_TABLE_ERROR;

    node->data = HashTablePairDestructor (node->data);

    if (ListDeleteElement (list, node) == LIST_ERROR)
        return HASH_TABLE_ERROR;

    table->size--;
    return HASH_TABLE_SUCCESS;
}

struct list_node*
HashTableFindElement (struct hash_table* const table,
                      const struct hash_table_pair* const data)
{
    if (table             == NULL ||
        table->list_array == NULL ||
        data              == NULL ||
        data->key         == NULL ||
        data->value       == NULL)
        return NULL;

    const size_t list_index = table->hash_function (data->key, table->capacity);

    if (list_index >= table->capacity) return NULL;

    if (table->list_array[list_index] == NULL) return NULL;

    struct list_node* node =
        ListFindElement (table->list_array[list_index], data);

    return node;
}

hash_table_error_t
HashTableRehash (struct hash_table* const table)
{
    if (table             == NULL ||
        table->list_array == NULL)
        return HASH_TABLE_ERROR;

    const size_t old_capacity = table->capacity;
    table->capacity *= 2;

    struct list** old_list_array = table->list_array;
    struct list** new_list_array =
        (struct list**) calloc (table->capacity, sizeof (struct list*));
    if (new_list_array == NULL) return HASH_TABLE_ERROR;

    table->list_array = new_list_array;

    struct list*      cur_list = NULL;
    struct list_node* cur_node = NULL;

    for (size_t i = 0; i < old_capacity; i++)
    {
        cur_list = old_list_array[i];
        if (cur_list == NULL) continue;

        cur_node = cur_list->head;

        while (cur_node != NULL)
        {
            if (HashTableAddElement (table, cur_node->data) == NULL)
                return HASH_TABLE_ERROR;
            cur_node = cur_node->next;
        }

        ListDestructor (cur_list);
    }

    free (old_list_array);
    return HASH_TABLE_SUCCESS;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Hash table pair functions' implementation
//-----------------------------------------------------------------------------
struct hash_table_pair*
HashTablePairConstructor (const void* const key,
                          const void* const value,
                          const size_t key_size,
                          const size_t value_size)
{
    struct hash_table_pair* const pair =
        (struct hash_table_pair* const) calloc (1, sizeof (struct hash_table_pair));
    if (pair == NULL) return NULL;

    pair->key = calloc (1, key_size);
    if (pair->key == NULL)
    {
        free (pair);
        return NULL;
    }

    pair->value = calloc (1, value_size);
    if (pair->value == NULL)
    {
        free (pair->key);
        free (pair);
        return NULL;
    }

    if (key != NULL)
        memcpy (pair->key, key, key_size);

    pair->key_size = key_size;

    if (value != NULL)
        memcpy (pair->value, value, value_size);

    pair->value_size = value_size;

    return pair;
}

struct hash_table_pair*
HashTablePairDestructor (struct hash_table_pair* const pair)
{
    if (pair == NULL) return NULL;

    free (pair->key);
    free (pair->value);

    memset (pair, POISON_VALUE, sizeof (struct hash_table_pair));

    free (pair);
    return NULL;
}

int
HashTablePairCompare (const struct hash_table_pair* const pair1,
                      const struct hash_table_pair* const pair2)
{
    if (pair1        == NULL || pair2        == NULL ||
        pair1->key   == NULL || pair2->key   == NULL ||
        pair1->value == NULL || pair2->value == NULL)
        return HASH_TABLE_PAIRS_NOT_EQUAL;

    if (memcmp (pair1->key,   pair2->key,   pair1->key_size)   == 0 &&
        memcmp (pair1->value, pair2->value, pair1->value_size) == 0)
        return HASH_TABLE_PAIRS_EQUAL;

    return HASH_TABLE_PAIRS_NOT_EQUAL;
}

void*
HashTablePairKey (struct hash_table_pair* const pair)
{
    if (pair == NULL) return NULL;
    return pair->key;
}

void*
HashTablePairValue (struct hash_table_pair* const pair)
{
    if (pair == NULL) return NULL;
    return pair->value;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Main task functions etc
//-----------------------------------------------------------------------------
const size_t HASH_FUNCTION_DEFAULT_CAPACITY = 100;
const size_t HASH_FUNCTION_DEFAULT_COEF     = (size_t) -1;
const size_t HASH_PRIME                     = 1000000009;

const char YES_STR[] = "YES";
const char NO_STR [] = "NO";

void
AddFunction (struct hash_table* const table,
             struct hash_table_pair* const input_pair);

void
DeleteFunction (struct hash_table* const table,
                struct hash_table_pair* const input_pair);

int
FindFunction (struct hash_table* const table,
              struct hash_table_pair* const input_pair);

enum operations
{
    ADD    = '+',
    DELETE = '-',
    FIND   = '?'
};

enum find_function_status
{
    FOUND     = 0,
    NOT_FOUND = 1
};

typedef size_t operation_status_t;

size_t
HashFunction (const void*  const key,
              const size_t capacity)
{
    assert (key);

    static size_t a = HASH_FUNCTION_DEFAULT_COEF;
    static size_t b = HASH_FUNCTION_DEFAULT_COEF;

    if (a == HASH_FUNCTION_DEFAULT_COEF &&
        b == HASH_FUNCTION_DEFAULT_COEF)
    {
        srand (time (NULL));
        a = rand () % HASH_PRIME;
        b = rand () % HASH_PRIME;
    }

    size_t key_index = *(size_t*)key;

    return ((a * key_index + b) % HASH_PRIME) % capacity;
}

int main (void)
{
    size_t N = 0;
    scanf ("%zd", &N);

    struct hash_table* const table =
        HashTableConstructor (N, HashFunction);
    assert (table);

    struct hash_table_pair* input_pair =
        HashTablePairConstructor (NULL, NULL, sizeof (size_t), sizeof (size_t));
    assert (input_pair);

    int operation = 0;

    for (size_t i = 0; i < N; i++)
    {
        do { operation = getchar (); }
        while (operation == '\n');

        switch (operation)
        {
            case ADD:
                AddFunction (table, input_pair);
                break;

            case DELETE:
                DeleteFunction (table, input_pair);
                break;

            case FIND:
                if (FindFunction (table, input_pair) == FOUND)
                     printf ("%s\n", YES_STR);
                else printf ("%s\n", NO_STR);
                break;

            default:
                assert (0);
        }
    }

    HashTablePairDestructor (input_pair);
    HashTableDestructor (table);
    return 0;
}

void
AddFunction (struct hash_table* const table,
             struct hash_table_pair* const input_pair)
{
    assert (table);
    assert (input_pair);

    int key = 0;
    scanf ("%d", &key);

    *(int*)input_pair->key   = key;
    *(int*)input_pair->value = key;

    HashTableAddElement (table, input_pair);
}

void
DeleteFunction (struct hash_table* const table,
                struct hash_table_pair* const input_pair)
{
    assert (table);
    assert (input_pair);

    int key = 0;
    scanf ("%d", &key);

    *(int*)input_pair->key   = key;
    *(int*)input_pair->value = key;

    struct list_node* del_node =
        HashTableFindElement (table, input_pair);

    HashTableDeleteElement (table, del_node);
}

int
FindFunction (struct hash_table* const table,
              struct hash_table_pair* const input_pair)
{
    assert (table);
    assert (input_pair);

    int key = 0;
    scanf ("%d", &key);

    *(int*)input_pair->key   = key;
    *(int*)input_pair->value = key;

    if (HashTableFindElement (table, input_pair) != NULL)
        return FOUND;

    return NOT_FOUND;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
