#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>



//-----------------------------------------------------------------------------
// Dynamic array consts
//-----------------------------------------------------------------------------
const size_t DYNAMIC_ARRAY_RESIZE_MULTIPLIER = 2;
const size_t DYNAMIC_ARRAY_NULL_SIZE   = 0;
const char   DYNAMIC_ARRAY_POISON_BYTE = 0;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// K-ary heap consts
//-----------------------------------------------------------------------------
const size_t K_HEAP_NULL_SIZE  = 0;
const size_t K_HEAP_ROOT_INDEX = 0;
const size_t K_HEAP_INITIAL_DATA_ARRAY_CAPACITY = 4;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Dynamic array structs and types
//-----------------------------------------------------------------------------
/**
 * @brief Error status for functions' return type.
 */
enum dynamic_array_error_status
{
    DYNAMIC_ARRAY_SUCCESS = 0,  ///< No errors occured, succes
    DYNAMIC_ARRAY_ERROR   = 1   ///< Some error occured
};

/**
 * @brief Adds possibility to disable or enable realloc in dynamic array.
 * It can be useful for static array storage, so struct can
 * be used both for dynamic array and static array with
 * useful information inside.
 * If realloc is disabled, beware of push/pop being out of range.
 */
enum dynmaic_array_enable_realloc
{
    DYNAMIC_ARRAY_REALLOC_DISABLED = false, ///< Realloc is disabled.
    DYNAMIC_ARRAY_REALLOC_ENABLED  = true   ///< Realloc is enabled.
};

/**
 * @brief Adds possibility to save data_array from dynamic_array struct.
 * If true, DynamicArrayDestructor() doesn't destroy void* data_array,
 * so you can use it even after the struct was destroyed.
 */
enum dynamic_array_save_array
{
    DYNAMIC_ARRAY_DESTROY = false,  ///< Array will  be destroyed and freed.
    DYNAMIC_ARRAY_SAVE    = true    ///< Array won't be destroyed and freed.
};

/**
 * @brief Return type for functions with dynamic_array_error_status values.
*/
typedef int dynamic_array_error_t;

/**
 * @brief Main struct.
 */
struct dynamic_array
{
    void* data_array;           ///< Pointer to array with all elements.
    size_t elem_size;           ///< Size of each element in bytes.

    size_t data_array_size;     ///< Number of elements.
    size_t data_array_capacity; ///< Current max number of elements in array.

    bool enable_realloc;        ///< Enable or disable realloc during push/pop.
    bool save_array;            ///< Save or destroy data_array in DynamicArrayDestructor().
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// K-ary heap structs and types
//-----------------------------------------------------------------------------
enum k_heap_error_status
{
    K_HEAP_SUCCESS = 0,
    K_HEAP_ERROR   = 1
};

typedef int k_heap_error_t;

struct k_heap
{
    size_t k;
    const void* infinity_value;
    int (*cmp) (const void* const,
                const void* const);
    struct dynamic_array* d_array;
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Dynamic aray functions' prototypes
//-----------------------------------------------------------------------------
/**
 * @brief Makes the struct and returns a pointer to it.
 * @param initial_data_array_capacity Sets data_array_capacity field. If 0, data_array = NULL.
 * @param elem_size Size of each element. Sets elem_size field in struct If 0, data_array = NULL.
 * @param enable_realloc Sets enable_realloc field in struct.
 * @param save_array Sets save_array field in struct.
 * @return Struct with filled fields.
*/
struct dynamic_array*
DynamicArrayConstructor  (const size_t initial_data_array_capaity,
                          const size_t elem_size,
                          const bool enable_realloc,
                          const bool save_array);

/**
 * @brief Destroys the struct and clear its fields.
 * If save_array field is set to DYNAMIC_ARRAY_SAVE, doesn't destroy
 * the data_array field. Otherwise, destroyes and frees it.
 * @param d_array Pointer to dynamic array struct.
 * @return NULL
*/
struct dynamic_array*
DynamicArrayDestructor   (struct dynamic_array* const d_array);

/**
 * @brief Checks whether data_array needs realloc and calls DynamicArrayRealloc() if true.
 * Does nothing if enable_realloc == DYNAMIC_ARRAY_REALLOC_DISABLED
 * @param d_array Pointer to dynamic array struct.
 * @return Error status.
 */
dynamic_array_error_t
DynamicArrayReallocCheck (struct dynamic_array* const d_array);

/**
 * @brief Reallocs data_array field up and down.
 * @param d_array Pointer to dynamic array struct.
 * @return Error status.
 */
dynamic_array_error_t
DynamicArrayRealloc      (struct dynamic_array* const d_array);

/**
 * @brief Pushes value from push_buffer in the end of the array.
 * Calls for DynamicArrayCheckRealloc() before pushing the value.
 * @param d_array Pointer to dynamic array struct.
 * @param push_buffer A pointer to element to copy in the array.
 * @return Error status.
 */
dynamic_array_error_t
DynamicArrayPush         (struct dynamic_array* const d_array,
                          const  void*          const push_buffer);

/**
 * @brief Pops value from the end of the array.
 * Calls for DynamicArrayCheckRealloc() after poping the value.
 * @param d_array Pointer to dynamic array struct.
 * @return Error status.
 */
dynamic_array_error_t
DynamicArrayPop          (struct dynamic_array* const d_array);

/**
 * @brief Gets pointer to element with given index.
 * @param d_array Pointer to dynamic array struct.
 * @param elem_index Element index to get.
 * @return Pointer to element. NULL if index is out of range.
 */
void*
DynamicArrayGetElemPtrByIndex (struct dynamic_array* const d_array,
                               const  size_t elem_index);

dynamic_array_error_t
DynamicArrayCheck (struct dynamic_array* const d_array);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// K-ary heap functions' prototypes
//-----------------------------------------------------------------------------
struct k_heap*
KHeapConstructor (struct dynamic_array* d_array,
                  const  size_t k,
                  const  size_t elem_size,
                  const  void* const infinity_value,
                  int    (*comparator) (const void* const,
                                        const void* const));

struct k_heap*
KHeapDestructor (struct k_heap* const heap);

k_heap_error_t
KHeapSiftUp (struct k_heap* const heap,
             const size_t key_index);

k_heap_error_t
KHeapSiftDown (struct k_heap* const heap,
               const size_t key_index);

k_heap_error_t
KHeapInsert (struct k_heap* const heap,
             const  void*   const insert_buffer);

k_heap_error_t
KHeapExtractRoot (struct k_heap* const heap,
                         void*   const get_buffer);

k_heap_error_t
KHeapDeleteKey (struct k_heap* const heap,
                const size_t key_index);

 void*
KHeapGetElemPtrByIndex (struct k_heap* const heap,
                        const size_t key_index);

void
swap (void* elem1, void* elem2, const size_t elem_size);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------





//-----------------------------------------------------------------------------
// Dynamic array functions' implementation
//-----------------------------------------------------------------------------
/**
 * @brief Checks whether the struct pointers are OK.
 * @param d_array Pointer to struct.
 * @return Error status.
 */


struct dynamic_array*
DynamicArrayConstructor (const size_t initial_data_array_capaity,
                         const size_t elem_size,
                         const bool enable_realloc,
                         const bool save_array)
{
    struct dynamic_array* new_dynamic_array =
        (struct dynamic_array*) calloc (1, sizeof (struct dynamic_array));
    if (new_dynamic_array == NULL)
        return NULL;

    void* new_data_array = NULL;

    if (initial_data_array_capaity != DYNAMIC_ARRAY_NULL_SIZE &&
        elem_size                  != DYNAMIC_ARRAY_NULL_SIZE)
    {
        new_data_array = calloc (initial_data_array_capaity, elem_size);
        if (new_data_array == NULL)
        {
            free (new_dynamic_array);
            return NULL;
        }
    }

    new_dynamic_array->data_array          = new_data_array;
    new_dynamic_array->data_array_capacity = initial_data_array_capaity;
    new_dynamic_array->data_array_size     = DYNAMIC_ARRAY_NULL_SIZE;
    new_dynamic_array->elem_size           = elem_size;
    new_dynamic_array->enable_realloc      = enable_realloc;
    new_dynamic_array->save_array          = save_array;

    return new_dynamic_array;
}

struct dynamic_array*
DynamicArrayDestructor (struct dynamic_array* const d_array)
{
    if (d_array == NULL) return NULL;

    /* destroy data_array */
    if (d_array->data_array != NULL &&
        d_array->save_array == DYNAMIC_ARRAY_DESTROY)
    {
        for (size_t i = 0; i < d_array->data_array_capacity; ++i)
        {
            memset (DynamicArrayGetElemPtrByIndex (d_array, i),
                    DYNAMIC_ARRAY_POISON_BYTE, d_array->elem_size);
        }

        free (d_array->data_array);
    }

    memset (d_array, DYNAMIC_ARRAY_POISON_BYTE, sizeof (struct dynamic_array));

    free (d_array);

    return NULL;
}

dynamic_array_error_t
DynamicArrayReallocCheck (struct dynamic_array* const d_array)
{
    assert (d_array);
    assert (d_array->data_array);

    if (d_array->enable_realloc == DYNAMIC_ARRAY_REALLOC_DISABLED)
        return DYNAMIC_ARRAY_SUCCESS;

    /* realloc up */
    if (d_array->data_array_size == d_array->data_array_capacity)
    {
        d_array->data_array_capacity *= DYNAMIC_ARRAY_RESIZE_MULTIPLIER;

        if (DynamicArrayRealloc (d_array) == DYNAMIC_ARRAY_ERROR)
            return DYNAMIC_ARRAY_ERROR;
    }

    /* realloc down */
    else if (d_array->data_array_size <=
             d_array->data_array_capacity / DYNAMIC_ARRAY_RESIZE_MULTIPLIER /
                                            DYNAMIC_ARRAY_RESIZE_MULTIPLIER)
    {
        d_array->data_array_capacity /= DYNAMIC_ARRAY_RESIZE_MULTIPLIER;

        if (DynamicArrayRealloc (d_array) == DYNAMIC_ARRAY_ERROR)
            return DYNAMIC_ARRAY_ERROR;
    }

    return DYNAMIC_ARRAY_SUCCESS;
}

dynamic_array_error_t
DynamicArrayRealloc (struct dynamic_array* const d_array)
{
    assert (d_array);
    assert (d_array->data_array);

    const size_t num_of_bytes =
        d_array->data_array_capacity * d_array->elem_size;

    void* new_data_array = realloc (d_array->data_array, num_of_bytes);
    if (new_data_array == NULL)
        return DYNAMIC_ARRAY_ERROR;

    d_array->data_array = new_data_array;

    /* poison new bytes */
    for (size_t i = d_array->data_array_size;
                i < d_array->data_array_capacity; ++i)
    {
        memset (DynamicArrayGetElemPtrByIndex (d_array, i),
                DYNAMIC_ARRAY_POISON_BYTE, d_array->elem_size);
    }

    return DYNAMIC_ARRAY_SUCCESS;
}

dynamic_array_error_t
DynamicArrayPush (struct dynamic_array* const d_array,
                  const  void*          const push_buffer)
{
    if (DynamicArrayCheck (d_array) == DYNAMIC_ARRAY_ERROR ||
        push_buffer == NULL)
        return DYNAMIC_ARRAY_ERROR;

    if (DynamicArrayReallocCheck (d_array) == DYNAMIC_ARRAY_ERROR)
        return DYNAMIC_ARRAY_ERROR;

    void* const dest_ptr =
        DynamicArrayGetElemPtrByIndex (d_array, d_array->data_array_size);

    if (dest_ptr == NULL)
        return DYNAMIC_ARRAY_ERROR;

    memcpy (dest_ptr, push_buffer, d_array->elem_size);
    d_array->data_array_size++;

    return DYNAMIC_ARRAY_SUCCESS;
}

dynamic_array_error_t
DynamicArrayPop (struct dynamic_array* const d_array)
{
    if (DynamicArrayCheck (d_array) == DYNAMIC_ARRAY_ERROR)
        return DYNAMIC_ARRAY_ERROR;

    if (d_array->data_array_size == DYNAMIC_ARRAY_NULL_SIZE)
        return DYNAMIC_ARRAY_ERROR;

    void* const dest_ptr =
        DynamicArrayGetElemPtrByIndex (d_array, d_array->data_array_size - 1);

    if (dest_ptr == NULL)
        return DYNAMIC_ARRAY_ERROR;

    memset (dest_ptr, DYNAMIC_ARRAY_POISON_BYTE, d_array->elem_size);
    d_array->data_array_size--;

    if (DynamicArrayReallocCheck (d_array) == DYNAMIC_ARRAY_ERROR)
        return DYNAMIC_ARRAY_ERROR;

    return DYNAMIC_ARRAY_SUCCESS;
}

void*
DynamicArrayGetElemPtrByIndex (struct dynamic_array* const d_array,
                               const  size_t elem_index)
{
    if (DynamicArrayCheck (d_array) == DYNAMIC_ARRAY_ERROR ||
        d_array->data_array_capacity <= elem_index)
        return NULL;

    return (void*) ((char*) d_array->data_array +
                            d_array->elem_size * elem_index);
}

 dynamic_array_error_t
DynamicArrayCheck (struct dynamic_array* const d_array)
{
    if (d_array == NULL || d_array->data_array == NULL)
        return DYNAMIC_ARRAY_ERROR;

    return DYNAMIC_ARRAY_SUCCESS;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// K-ary heap functions' implementation
//-----------------------------------------------------------------------------
struct k_heap*
KHeapConstructor (struct dynamic_array* d_array,
                  const  size_t k,
                  const  size_t elem_size,
                  const  void* const infinity_value,
                  int    (*comparator) (const void* const,
                                        const void* const))
{
    struct k_heap* heap =
        (struct k_heap*) calloc (1, sizeof (struct k_heap));
    if (heap == NULL) return NULL;

    heap->k = k;
    heap->infinity_value = infinity_value;
    heap->cmp = comparator;

    if (d_array == NULL)
    {
        heap->d_array =
            DynamicArrayConstructor (K_HEAP_INITIAL_DATA_ARRAY_CAPACITY, elem_size,
                                     DYNAMIC_ARRAY_REALLOC_ENABLED,
                                     DYNAMIC_ARRAY_DESTROY);
        if (heap->d_array == NULL)
        {
            free (heap);
            return NULL;
        }
    }

    else
    {
        heap->d_array = d_array;

        for (size_t i = d_array->data_array_size / heap->k; i > 0; --i)
            KHeapSiftDown (heap, i);

        KHeapSiftDown (heap, K_HEAP_ROOT_INDEX);
    }

    return heap;
}

struct k_heap*
KHeapDestructor (struct k_heap* const heap)
{
    if (heap == NULL) return NULL;

    heap->d_array = DynamicArrayDestructor (heap->d_array);
    heap->infinity_value = NULL;
    heap->k = K_HEAP_NULL_SIZE;
    heap->cmp = NULL;

    return NULL;
}

k_heap_error_t
KHeapSiftUp (struct k_heap* const heap,
             const size_t key_index)
{
    if (heap == NULL || key_index >= heap->d_array->data_array_size)
        return K_HEAP_ERROR;

    size_t  cur_index  = key_index;
    size_t  parent     = 0;
    size_t* cur_ptr    = NULL;
    size_t* parent_ptr = NULL;

    while (cur_index > 0)
    {
        parent = (cur_index - 1) / heap->k;

        cur_ptr    = KHeapGetElemPtrByIndex (heap, cur_index);
        parent_ptr = KHeapGetElemPtrByIndex (heap, parent);

        if (cur_ptr == NULL || parent_ptr == NULL)
            return K_HEAP_ERROR;

        if (heap->cmp (parent_ptr, cur_ptr) < 0)
        {
            swap (parent_ptr, cur_ptr, heap->d_array->elem_size);
            cur_index = parent;
        }

        else return K_HEAP_SUCCESS;
    }

    return K_HEAP_SUCCESS;
}

k_heap_error_t
KHeapSiftDown (struct k_heap* const heap,
               const size_t key_index)
{
    if (heap == NULL || key_index >= heap->d_array->data_array_size)
        return K_HEAP_ERROR;

    size_t cur_index = key_index;
    size_t cur_child = key_index * heap->k + 1;

    size_t* cur_ptr = KHeapGetElemPtrByIndex (heap, cur_index);
    size_t* cur_child_ptr = NULL;

    for (size_t i = 0; i < heap->k; ++i)
    {
        cur_child_ptr = KHeapGetElemPtrByIndex (heap, cur_child);
        if (cur_child_ptr == NULL || cur_child >= heap->d_array->data_array_size)
            break;

        if (heap->cmp (cur_ptr, cur_child_ptr) < 0)
        {
            cur_index     = cur_child;
            cur_ptr       = cur_child_ptr;
        }

        cur_child++;
    }

    if (cur_index != key_index)
    {
        swap (KHeapGetElemPtrByIndex (heap, key_index),
              cur_ptr, heap->d_array->elem_size);
        KHeapSiftDown (heap, cur_index);
    }

    return K_HEAP_SUCCESS;
}

k_heap_error_t
KHeapInsert (struct k_heap* const heap,
             const  void*   const insert_buffer)
{
    if (heap == NULL || insert_buffer == NULL)
        return K_HEAP_ERROR;

    if (DynamicArrayPush (heap->d_array, insert_buffer) == DYNAMIC_ARRAY_ERROR)
        return K_HEAP_ERROR;

    if (KHeapSiftUp (heap, heap->d_array->data_array_size - 1) == K_HEAP_ERROR)
        return K_HEAP_ERROR;

    return K_HEAP_SUCCESS;
}

k_heap_error_t
KHeapExtractRoot (struct k_heap* const heap,
                         void*   const get_buffer)
{
    if (heap == NULL)
        return K_HEAP_ERROR;

    void* const root_ptr = KHeapGetElemPtrByIndex
        (heap, K_HEAP_ROOT_INDEX);

    void* const last_ptr = KHeapGetElemPtrByIndex
        (heap, heap->d_array->data_array_size - 1);

    // fprintf (stderr, "LAST_PTR %d\n", *(int*)last_ptr);

    if (root_ptr == NULL || last_ptr == NULL) return K_HEAP_ERROR;

    if (get_buffer != NULL)
        memcpy (get_buffer, root_ptr, heap->d_array->elem_size);

    memcpy (root_ptr, last_ptr, heap->d_array->elem_size);

    if (DynamicArrayPop (heap->d_array) == DYNAMIC_ARRAY_ERROR)
        return K_HEAP_ERROR;

    if (KHeapSiftDown (heap, K_HEAP_ROOT_INDEX) == K_HEAP_ERROR)
        return K_HEAP_ERROR;

    return K_HEAP_SUCCESS;
}

k_heap_error_t
KHeapDeleteKey (struct k_heap* const heap,
                const size_t key_index)
{
    if (heap == NULL || key_index >= heap->d_array->data_array_size)
        return K_HEAP_ERROR;

    // a[key_index] = inf;
    memcpy (KHeapGetElemPtrByIndex (heap, key_index),
            heap->infinity_value, heap->d_array->elem_size);

    if (KHeapSiftUp (heap, key_index) == K_HEAP_ERROR)
        return K_HEAP_ERROR;

    if (KHeapExtractRoot (heap, NULL) == K_HEAP_ERROR)
        return K_HEAP_ERROR;

    return K_HEAP_SUCCESS;
}

 void*
KHeapGetElemPtrByIndex (struct k_heap* const heap,
                        const size_t key_index)
{
    if (heap == NULL) return NULL;

    return DynamicArrayGetElemPtrByIndex (heap->d_array, key_index);
}

void
swap (void* elem1, void* elem2, const size_t elem_size)
{
    assert (elem1);
    assert (elem2);

    void* tmp_elem = calloc (1, elem_size);
    assert (tmp_elem);

    memcpy (tmp_elem, elem2,    elem_size);
    memcpy (elem2,    elem1,    elem_size);
    memcpy (elem1,    tmp_elem, elem_size);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Main functions
//-----------------------------------------------------------------------------
const unsigned MODULE_VALUE = 0x40000000;

int
UnsignedCmp (const void* const elem1,
             const void* const elem2);

void
FillHeap (struct k_heap* const heap,
          const size_t N,
          const size_t K,
          const unsigned a_0,
          const unsigned x,
          const unsigned y);

void
SortHeap (struct k_heap* const heap,
          const size_t elem_number);

unsigned
NextSequenceElement (const unsigned prev,
                     const unsigned x,
                     const unsigned y);

int main ()
{
    size_t N = 0;
    scanf ("%zd", &N);

    size_t K = 0;
    scanf ("%zd", &K);

    const unsigned infinity_value = UINT_MAX;

    struct k_heap* const heap = 
        KHeapConstructor (NULL, 2, sizeof (unsigned), &infinity_value, UnsignedCmp);
    assert (heap);

    unsigned a_0 = 0;
    unsigned x   = 0;
    unsigned y   = 0;

    scanf ("%u", &a_0);
    scanf ("%u", &x);
    scanf ("%u", &y);

    if (K > N) K = N;

    FillHeap (heap, N, K, a_0, x, y);

    SortHeap (heap, K);

    const unsigned* elem_ptr = KHeapGetElemPtrByIndex (heap, K_HEAP_ROOT_INDEX);
    assert (elem_ptr);

    for (size_t i = 0; i < K; ++i)
        printf ("%u ", *elem_ptr++);

    KHeapDestructor (heap);
    return 0;
}

void
FillHeap (struct k_heap* const heap,
          const size_t N,
          const size_t K,
          const unsigned a_0,
          const unsigned x,
          const unsigned y)
{
    assert (heap);

    unsigned prev_value = a_0;
    unsigned next_value = 0;

    // first K values
    for (size_t i = 0; i < K; ++i)
    {
        next_value = NextSequenceElement (prev_value, x, y);
        KHeapInsert (heap, &next_value);
        prev_value = next_value;
    }

    if (K == N) return;

    unsigned heap_max =
        *(unsigned*) KHeapGetElemPtrByIndex (heap, K_HEAP_ROOT_INDEX);

    for (size_t i = K; i < N; ++i)
    {
        next_value = NextSequenceElement (prev_value, x, y);

        if (next_value < heap_max)
        {
            KHeapExtractRoot (heap, &heap_max);
            KHeapInsert      (heap, &next_value);
            heap_max =
                *(unsigned*) KHeapGetElemPtrByIndex (heap, K_HEAP_ROOT_INDEX);
        }

        prev_value = next_value;
    }
}

void
SortHeap (struct k_heap* const heap,
          const size_t elem_number)
{
    assert (heap);

    qsort (heap->d_array->data_array, elem_number,
           sizeof (unsigned), heap->cmp);
}

unsigned
NextSequenceElement (const unsigned prev,
                     const unsigned x,
                     const unsigned y)
{
    unsigned long long new_value = prev;
    new_value = (new_value * x) % MODULE_VALUE;
    new_value = (new_value + y) % MODULE_VALUE;

    // explicit cast of unsigned values discards high-order bits
    return (unsigned) new_value;
}

int
UnsignedCmp (const void* const elem1,
             const void* const elem2)
{
    assert (elem1);
    assert (elem2);

    if (*(unsigned*)elem1 >= *(unsigned*)elem2) return 1;
    return -1;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
