#include "../include/k_heap.h"

#include <stdio.h>

//-----------------------------------------------------------------------------
// K-heap static functions prototypes
//-----------------------------------------------------------------------------
static k_heap_error_t
KHeapPushNewItem (k_heap_t*           const heap,
                  const k_heap_key*   const key,
                  const k_heap_value* const value);

static k_heap_error_t
KHeapCopyLastElement (k_heap_t* const heap);

static k_heap_error_t
KHeapPopLastElement (k_heap_t* const heap);

static k_heap_error_t
KHeapSiftUp (k_heap_t* const heap,
             const size_t index);

static k_heap_error_t
KHeapSiftDown (k_heap_t* const heap,
               const size_t index);

static k_heap_key*
KHeapKeyCopy (const k_heap_key* const key);

static k_heap_value*
KHeapValueCopy (const k_heap_value* const value);

static size_t
KHeapSize (const k_heap_t* const heap);

static void
KHeapSwap (k_heap_t* const heap,
           const size_t index_1,
           const size_t index_2);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// K-heap functions implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Constructor and destructor
k_heap_t*
KHeapConstructor (const size_t k,
                  const k_heap_key* const infinity_key,
                  int (*key_cmp) (const k_heap_key* const,
                                  const k_heap_key* const))
{
    k_heap_t* const heap = (k_heap_t*) malloc (sizeof (k_heap_t));
    if (heap == NULL)
        return NULL;

    heap->k            = k;
    heap->infinity_key = KHeapKeyCopy (infinity_key);
    heap->array        = DynamicArrayConstructor (sizeof (k_heap_item));
    heap->key_cmp      = key_cmp;

    return heap;
}

k_heap_t*
KHeapDestructor  (k_heap_t* const heap)
{
    if (heap == NULL)
        return NULL;

    heap->infinity_key = KHeapKeyDestructor (heap->infinity_key);

    k_heap_item* cur_item = NULL;

    for (size_t i = 0; i < KHeapSize (heap); ++i)
    {
        cur_item = DynamicArrayGetElemPtrByIndex (heap->array, i);
        
        if (cur_item != NULL)
        {
            cur_item->key   = KHeapKeyDestructor (cur_item->key);
            cur_item->value = KHeapValueDestructor (cur_item->value);
        }
    }

    heap->array = DynamicArrayDestructor (heap->array);

    free (heap);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Interface
k_heap_error_t
KHeapInsert (k_heap_t*           const heap,
             const k_heap_key*   const key,
             const k_heap_value* const value)
{
    bool error_status = K_HEAP_SUCCESS;

    error_status = error_status || KHeapPushNewItem (heap, key, value);

    error_status = error_status || KHeapSiftUp (heap, KHeapSize (heap) - 1);

    return error_status;
}

k_heap_value*
KHeapGetRoot (k_heap_t* const heap)
{
    if (heap == NULL) return NULL;

    k_heap_item* const item = DynamicArrayGetElemPtrByIndex (heap->array, 0);
    if (item == NULL) return NULL;

    return item->value;
}

k_heap_error_t
KHeapExtractRoot (k_heap_t* const heap)
{
    bool error_status = K_HEAP_SUCCESS;

    error_status = error_status || KHeapCopyLastElement (heap);

    error_status = error_status || KHeapPopLastElement  (heap);

    error_status = error_status || KHeapSiftDown (heap, 0);

    return error_status;
}
//-------------------------------------

//-------------------------------------
// Static functions
static k_heap_error_t
KHeapPushNewItem (k_heap_t*           const heap,
                  const k_heap_key*   const key,
                  const k_heap_value* const value)
{
    if (heap  == NULL ||
        key   == NULL ||
        value == NULL)
        return K_HEAP_ERROR;

    k_heap_key* new_key = KHeapKeyCopy (key);
    if (new_key == NULL) return K_HEAP_ERROR;

    k_heap_value* new_value = KHeapValueCopy (value);
    if (new_value == NULL) return K_HEAP_ERROR;

    k_heap_item new_item = {.key = new_key, .value = new_value};

    if (DynamicArrayPush (heap->array, &new_item) == DYNAMIC_ARRAY_ERROR)
    {
        new_key   = KHeapKeyDestructor   (new_key);
        new_value = KHeapValueDestructor (new_value);
        return K_HEAP_ERROR;
    }

    return K_HEAP_SUCCESS;
}

static k_heap_error_t
KHeapCopyLastElement (k_heap_t* const heap)
{
    if (heap        == NULL ||
        heap->array == NULL)
        return K_HEAP_ERROR;

    k_heap_item* const last_item =
        DynamicArrayGetElemPtrByIndex (heap->array, KHeapSize (heap) - 1);
    if (last_item == NULL) return K_HEAP_ERROR;

    k_heap_item* const root_item =
        DynamicArrayGetElemPtrByIndex (heap->array, 0);
    if (root_item == NULL) return K_HEAP_ERROR;

    if (root_item == last_item) return K_HEAP_SUCCESS;

    root_item->key   = KHeapKeyDestructor   (root_item->key);
    root_item->value = KHeapValueDestructor (root_item->value);

    root_item->key   = KHeapKeyCopy         (last_item->key);
    root_item->value = KHeapValueCopy       (last_item->value);

    return K_HEAP_SUCCESS;
}

static k_heap_error_t
KHeapPopLastElement (k_heap_t* const heap)
{
    if (heap == NULL) return K_HEAP_ERROR;
    
    k_heap_item* const last_item =
        DynamicArrayGetElemPtrByIndex (heap->array, KHeapSize (heap) - 1);

    last_item->key   = KHeapKeyDestructor   (last_item->key);
    last_item->value = KHeapValueDestructor (last_item->value);

    d_array_error_t error_status = DynamicArrayPop (heap->array);

    if (error_status == DYNAMIC_ARRAY_ERROR) return K_HEAP_ERROR;
    return K_HEAP_SUCCESS;
}

static k_heap_error_t
KHeapSiftUp (k_heap_t* const heap,
             const size_t index)
{
    if (heap          == NULL ||
        heap->array   == NULL ||
        heap->key_cmp == NULL)
        return K_HEAP_ERROR;

    size_t cur_index    = index;
    size_t parent_index = 0;

    k_heap_item* cur_item    = NULL;
    k_heap_item* parent_item = NULL;

    while (cur_index != 0)
    {
        parent_index = (cur_index - 1) / 2;

        cur_item    = DynamicArrayGetElemPtrByIndex (heap->array, cur_index);
        parent_item = DynamicArrayGetElemPtrByIndex (heap->array, parent_index);

        if (heap->key_cmp (parent_item->key, cur_item->key) == K_HEAP_CMP_LESS)
        {
            KHeapSwap (heap, parent_index, cur_index);
            cur_index = parent_index;
        }

        else break;
    }

    return K_HEAP_SUCCESS;
}

static k_heap_error_t
KHeapSiftDown (k_heap_t* const heap,
               const size_t index)
{
    if (heap          == NULL ||
        heap->key_cmp == NULL)
        return K_HEAP_ERROR;

    const size_t heap_size = KHeapSize (heap);

    if (index >= heap_size) return K_HEAP_SUCCESS;

    size_t min_index         = index;
    size_t left_child_index  = index * 2 + 1;
    size_t right_child_index = index * 2 + 2;

    k_heap_item* min_item   =
        DynamicArrayGetElemPtrByIndex (heap->array, min_index);

    k_heap_item* left_item  = 
        DynamicArrayGetElemPtrByIndex (heap->array, left_child_index);

    k_heap_item* right_item = 
        DynamicArrayGetElemPtrByIndex (heap->array, right_child_index);

    if (left_child_index < heap_size && left_item != NULL &&
        heap->key_cmp (min_item->key, left_item->key) == K_HEAP_CMP_LESS)
        min_index = left_child_index;

    min_item = DynamicArrayGetElemPtrByIndex (heap->array, min_index);

    if (right_child_index < heap_size && right_item != NULL &&
        heap->key_cmp (min_item->key, right_item->key) == K_HEAP_CMP_LESS)
        min_index = right_child_index;

    if (min_index != index)
    {
        KHeapSwap (heap, min_index, index);
        return KHeapSiftDown (heap, min_index);
    }

    return K_HEAP_SUCCESS;
}

static size_t
KHeapSize (const k_heap_t* const heap)
{
    if (heap        == NULL ||
        heap->array == NULL)
        return 0;

    return heap->array->size;
}

static void
KHeapSwap (k_heap_t* const heap,
           const size_t index_1,
           const size_t index_2)
{
    if (heap        == NULL ||
        heap->array == NULL)
        return;

    k_heap_item* const item_1 =
        DynamicArrayGetElemPtrByIndex (heap->array, index_1);
    if (item_1 == NULL) return;

    k_heap_item* const item_2 =
        DynamicArrayGetElemPtrByIndex (heap->array, index_2);
    if (item_2 == NULL) return;

    k_heap_key*   const tmp_key   = item_1->key;
    k_heap_value* const tmp_value = item_1->value;

    item_1->key   = item_2->key;
    item_1->value = item_2->value;
    item_2->key   = tmp_key;
    item_2->value = tmp_value;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// K-heap key and value functions implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Constructor and destructor
k_heap_key*
KHeapKeyConstructor (const void* const key,
                     const size_t key_len)
{
    k_heap_key* const new_key =
        (k_heap_key*) calloc (1, sizeof (k_heap_key));
    if (new_key == NULL)
        return NULL;

    if (key != NULL && key_len != 0)
    {
        new_key->key = malloc (key_len);
        if (new_key->key == NULL)
            return KHeapKeyDestructor (new_key);

        memcpy (new_key->key, key, key_len);
        new_key->key_len = key_len;
    }

    return new_key;
}

k_heap_key*
KHeapKeyDestructor (k_heap_key* const key)
{
    if (key == NULL) return NULL;

    free (key->key);
    free (key);
    return NULL;
}

k_heap_value*
KHeapValueConstructor (const void* const value,
                       const size_t value_len)
{
    k_heap_value* const new_value =
        (k_heap_value*) calloc (1, sizeof (k_heap_value));
    if (new_value == NULL)
        return NULL;

    if (value != NULL && value_len != 0)
    {
        new_value->value = malloc (value_len);
        if (new_value->value == NULL)
            return KHeapValueDestructor (new_value);

        memcpy (new_value->value, value, value_len);
        new_value->value_len = value_len;
    }

    return new_value;
}

k_heap_value*
KHeapValueDestructor (k_heap_value* const value)
{
    if (value == NULL) return NULL;

    free (value->value);
    free (value);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Static functions
static k_heap_key*
KHeapKeyCopy (const k_heap_key* const key)
{
    if (key == NULL) return NULL;
    return KHeapKeyConstructor (key->key, key->key_len);
}

static k_heap_value*
KHeapValueCopy (const k_heap_value* const value)
{
    if (value == NULL) return NULL;
    return KHeapValueConstructor (value->value, value->value_len);
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------