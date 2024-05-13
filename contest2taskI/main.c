#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//-----------------------------------------------------------------------------
// Dynamic array struct
//-----------------------------------------------------------------------------
const size_t DYNAMIC_ARRAY_RESIZE_MULTIPLIER = 2;

typedef bool d_array_realloc_status_t;

/**
 * @brief Dynamic array structure
 */
typedef struct dynamic_array
{
    void*  data;            ///< Pointer to array with data
    size_t size;            ///< Current nuumber of elements in the array
    size_t capacity;        ///< Current capacity of the array
    size_t element_size;    ///< Size of each element in the array in bytes
}
d_array_t;

/**
 * @brief Dynamic array error statuses
 */
enum d_array_errors
{
    DYNAMIC_ARRAY_SUCCESS = 0,  ///< No errors occurred, success
    DYNAMIC_ARRAY_ERROR   = 1   ///< Error occurred
};

typedef size_t d_array_error_t;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Static functions prototypes
//-----------------------------------------------------------------------------
//-------------------------------------
// Realloc functions
static d_array_error_t
DynamicArrayRealloc (d_array_t* const array);

static d_array_realloc_status_t
DynamicArrayCheckRealloc (d_array_t* const array);

static d_array_error_t
DynamicArrayExecuteRealloc (d_array_t* const array);
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Dynamic array functions' prototypes
//-----------------------------------------------------------------------------
//-------------------------------------
// Constructor and destructor
/**
 * @brief Constructor for dynamic array structure
 * 
 * @param element_size size of each element in bytes
 * 
 * @return pointer to the structure if success, NULL if error
 * 
 * Allocates memory for the dynamic array structure, initializes and returns it.
 * array->data field is NULL at this point.
 */
d_array_t*
DynamicArrayConstructor       (const size_t element_size);

/**
 * @brief Destructor for dynamic array structure
 * 
 * @param array pointer to dynamic array structure
 * 
 * @return NULL
 * 
 * If array == NULL, returns NULL.
 * Otherwise, frees array->data field and array itself.
 */
d_array_t*
DynamicArrayDestructor        (d_array_t* const array);
//-------------------------------------

//-------------------------------------
// Interface
/**
 * @brief Push function with reallocation of array
 * 
 * @param array pointer to dynamic array structure
 * @param element pointer to element buffer to push
 * 
 * @return DYNAMIC_ARRAY_SUCCESS on success, DYNAMIC_ARRAY_ERROR on error
 * 
 * 1. Calls for DynamicArrayRealloc() function to check and perform realloc if needed
 * 2. Calls for DynamicArrayPushNoRealloc() function after that to perform push operation
 */
d_array_error_t
DynamicArrayPush              (d_array_t*  const array,
                               const void* const element);

/**
 * @brief Push function without reallocation of array
 * 
 * @param array pointer to dynamic array structure
 * @param element pointer to element buffer to push
 * 
 * @return DYNAMIC_ARRAY_SUCCESS on success, DYNAMIC_ARRAY_ERROR on error
 * 
 * Does nothing if array->size == array->capacity, returns DYNAMIC_ARRAY_ERROR status.
 * Otherwise, copies value of array->element_size bytes from element buffer
 * to the end of array. Increases array->size.
 */
d_array_error_t
DynamicArrayPushNoRealloc     (d_array_t* const array,
                               const void* const element);
                  
/**
 * @brief Pop function with reallocation of array
 * 
 * @param array pointer to dynamic array structure
 * 
 * @return DYNAMIC_ARRAY_SUCCESS on success, DYNAMIC_ARRAY_ERROR on error
 * 
 * 1. Calls for DynamicArrayPopNoRealloc() function to perform pop operation
 * 2. Calls for DynamicArrayRealloc() function to check and perform realloc if needed
 */
d_array_error_t
DynamicArrayPop               (d_array_t* const array);

/**
 * @brief Pop function without reallocation of array
 * 
 * @param array pointer to dynamic array structure
 *
 * @return DYNAMIC_ARRAY_SUCCESS on success, DYNAMIC_ARRAY_ERROR on error
 * 
 * Does nothing if array->size == 0, returns DYNAMIC_ARRAY_ERROR status.
 * Otherwise, decreases array->size.
 */
d_array_error_t
DynamicArrayPopNoRealloc      (d_array_t* const array);

/**
 * @brief Gets pointer to the element of the dynamic array by its index
 * 
 * @param array pointer to dynamic array structure
 * 
 * @return pointer on success, NULL on error
 * 
 * Returns NULL if index out of range.
 * Otherwise, returns pointer to the element with void* pointer type.
 */
void*
DynamicArrayGetElemPtrByIndex (d_array_t* const array,
                               const size_t index);
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Dynamic array functions' prototypes
//-----------------------------------------------------------------------------
//-------------------------------------
// Constructor and destructor
d_array_t*
DynamicArrayConstructor (const size_t element_size)
{
    d_array_t* const array = (d_array_t*) calloc(1, sizeof(d_array_t));
    if (array == NULL)
        return NULL;

    array->element_size = element_size;
    array->size         = 0;
    array->capacity     = 0;

    return array;
}

d_array_t*
DynamicArrayDestructor (d_array_t* const array)
{
    if (array == NULL)
        return NULL;

    free(array->data);
    free(array);

    return NULL;
}
//-------------------------------------

//-------------------------------------
// Interface
d_array_error_t
DynamicArrayPush (d_array_t*  const array,
                  const void* const element)
{
    if (array   == NULL ||
        element == NULL)
        return DYNAMIC_ARRAY_ERROR;

    d_array_error_t error_status = DYNAMIC_ARRAY_SUCCESS;

    error_status = error_status || DynamicArrayRealloc (array);

    error_status = error_status || DynamicArrayPushNoRealloc (array, element);

    return error_status;
}

d_array_error_t
DynamicArrayPushNoRealloc (d_array_t*  const array,
                           const void* const element)
{
    if (array       == NULL ||
        array->size == array->capacity)
        return DYNAMIC_ARRAY_ERROR;

    void* const push_ptr = DynamicArrayGetElemPtrByIndex (array, array->size);
    if (push_ptr == NULL)
        return DYNAMIC_ARRAY_ERROR;

    memcpy (push_ptr, element, array->element_size);
    ++array->size;

    return DYNAMIC_ARRAY_SUCCESS;
}

d_array_error_t
DynamicArrayPop (d_array_t* const array)
{
    d_array_error_t error_status = DYNAMIC_ARRAY_SUCCESS;

    error_status = error_status || DynamicArrayPopNoRealloc (array);

    error_status = error_status || DynamicArrayRealloc (array);

    return error_status;
}

d_array_error_t
DynamicArrayPopNoRealloc (d_array_t* const array)
{
    if (array       == NULL ||
        array->size == 0)
        return DYNAMIC_ARRAY_ERROR;

    --array->size;

    return DYNAMIC_ARRAY_SUCCESS;
}

void*
DynamicArrayGetElemPtrByIndex (d_array_t* const array,
                               const size_t index)
{
    if (array == NULL ||
        index >= array->capacity)
        return NULL;

    return (char*)array->data + (index * array->element_size);
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Static functions implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Realloc functions
static d_array_error_t
DynamicArrayRealloc (d_array_t* const array)
{
    if (DynamicArrayCheckRealloc (array) == true)
        return DynamicArrayExecuteRealloc (array);

    return DYNAMIC_ARRAY_SUCCESS;
}

static d_array_realloc_status_t
DynamicArrayCheckRealloc (d_array_t* const array)
{
    const size_t size     = array->size;
    const size_t capacity = array->capacity;

    if (capacity == 0)
    {
        array->capacity = 1;
        return true;
    }

    else if (size == capacity)
    {
        array->capacity *= DYNAMIC_ARRAY_RESIZE_MULTIPLIER;
        return true;
    }

    else if (size < capacity / DYNAMIC_ARRAY_RESIZE_MULTIPLIER / 
                               DYNAMIC_ARRAY_RESIZE_MULTIPLIER)
    {
        array->capacity /= DYNAMIC_ARRAY_RESIZE_MULTIPLIER;
        return true;
    }

    return false;
}

static d_array_error_t
DynamicArrayExecuteRealloc (d_array_t* const array)
{
    if (array == NULL) return DYNAMIC_ARRAY_ERROR;

    const size_t num_of_bytes = array->capacity * array->element_size;

    void* const new_ptr = realloc (array->data, num_of_bytes);
    if (new_ptr == NULL)
        return DYNAMIC_ARRAY_ERROR;

    array->data = new_ptr;
    return DYNAMIC_ARRAY_SUCCESS;
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



/******************************************************************************
 * @name Splay tree structs and types.
 * 
 * @{
 ******************************************************************************/
/**
 * @brief Splay key structure.
 * 
 * A container for key of the node.
 */
typedef struct splay_key
{
    void*  key;                                 ///< buffer for the key
    size_t key_len;                             ///< size of the key buffer in bytes
}
splay_key;

/**
 * @brief Splay value structure.
 * 
 * A container for value of the node.
 */
typedef struct splay_value
{
    void*  value;                               ///< buffer for the value
    size_t value_len;                           ///< size of the value buffer in bytes
}
splay_value;

/**
 * @brief Splay node structure.
 * 
 * A container for node and its information.
 * Includes key and value fields and pointers to child nodes and parent node.
 */
typedef struct splay_node
{
    splay_key*         key;                     ///< key structure
    splay_value*       value;                   ///< value structure
    struct splay_node* left;                    ///< pointer to the left child node
    struct splay_node* right;                   ///< pointer to the right child node
    struct splay_node* parent;                  ///< pointer to the parent node
}
splay_node;

/**
 * @brief Splay tree structure.
 * 
 * Includes pointer to the root node and pointer to key comparator function.
 */
typedef struct splay_tree
{
    splay_node* root;                           ///< pointer to the root node
    int (*key_cmp) (const splay_key* const,     ///< key comparator function
                    const splay_key* const);
}
splay_tree;

/**
 * @brief Splay tree errors enumeration.
 * 
 * List of possible error codes, including no-error status.
 */
enum splay_errors
{
    SPLAY_TREE_SUCCESS = 0,                     ///< no error occured during function
    SPLAY_TREE_ERROR   = 1                      ///< error occured during function
};

/**
 * @brief Splay tree error type.
 * 
 * Return type for functions error codes.
 */
typedef size_t splay_error_t;

/******************************************************************************
 * @}
 ******************************************************************************/



/******************************************************************************
 * @name Splay tree functions prototypes
 * 
 * @{
 ******************************************************************************/
//-------------------------------------
// Splay tree constructor and destructor.
//-------------------------------------
/**
 * @brief Splay tree constructor.
 * 
 * @param[in] key_cmp key comparator function.
 * 
 * @retval Pointer to the allocated structure.
 * @retval NULL if allocation error occurred.
 * 
 * Allocates memory for the splay_tree structure and fills its
 * key_cmp field with given compare function.
 */
splay_tree*
SplayTreeConstructor (int (*key_cmp) (const splay_key* const,
                                      const splay_key* const));

/**
 * @brief Splay tree destructor.
 * 
 * @param[in] tree Pointer to the tree to destroy.
 * 
 * @retval NULL in all cases.
 * 
 * Destroys all nodes using SplayTreeDeleteKey() and frees allocated memory.
 */
splay_tree*
SplayTreeDestructor (splay_tree* const tree);
//-------------------------------------

//-------------------------------------
// Splay node constructor and destructor.
//-------------------------------------
/**
 * @brief Splay node constructor.
 * 
 * @param[in] key   structure of the key   to copy. No copying if NULL is given.
 * @param[in] value structure of the value to copy. No copying if NULL is given.
 * 
 * @retval Pointer to the allocated structure.
 * @retval NULL if allocation error occurred.
 * 
 * Allocates memory for the splay_node structure and fills its
 * key and value fields with given structures.
 * Key and value are being copied using SplayKeyCopy() and SplayValueCopy() functions.
 */
splay_node*
SplayNodeConstructor (const splay_key*   const key,
                      const splay_value* const value);

/**
 * @brief Splay node destructor.
 * 
 * @param[in] node a node to destroy.
 * @retval NULL in all cases.
 * 
 * Calls for key and value destructors and frees allocated memory.
 */
splay_node*
SplayNodeDestructor  (splay_node* const node);
//-------------------------------------

//-------------------------------------
// Splay key and value constructor, destructor and copy.
//-------------------------------------
/**
 * @brief Splay key constructor
 * 
 * @param[in] key buffer of the key.
 * @param[in] key_len buffer length.
 * 
 * @retval Pointer to the allocated structure.
 * @retval NULL if allocation error occurred.
 * 
 * Allocates memory for the splay_key structure.
 * Allocates key_len bytes for a new buffer and
 * copies key_len bytes from the given buffer.
 * If buffer is NULL or key_len is 0, only allocates splay_key structure.
 */
splay_key*
SplayKeyConstructor (const void* const key,
                     const size_t key_len);

/**
 * @brief Copies key from the given one.
 * 
 * @param[in] key splay_key structure to copy.
 * 
 * @retval Pointer to the allocated structure.
 * @retval NULL if allocation error occurred.
 * 
 * Calls for SplayKeyConstructor (key->key, key->value).
 */
splay_key*
SplayKeyCopy (const splay_key* const key);

/**
 * @brief Splay key destructor.
 * 
 * @param[in] key splay_key structure to destroy.
 * 
 * @retval NULL in all cases.
 * 
 * Frees buffer inside the structure.
 * Frees given splay_key structure.
 */
splay_key*
SplayKeyDestructor (splay_key* const key);

/**
 * @brief Splay value constructor.
 * 
 * @param[in] value buffer of the value.
 * @param[in] value_len buffer length.
 * 
 * @retval Pointer to the allocated structure.
 * @retval NULL if allocation error occurred.
 * 
 * @see SplayKeyConstructor(). Same thing but for splay_value.
 */
splay_value*
SplayValueConstructor (const void* const value,
                       const size_t value_len);

/**
 * @brief Copies value from the given one.
 * 
 * @param[in] value splay_value structure to copy.
 * 
 * @retval Pointer to the allocated structure.
 * @retval NULL if allocation error occurred.
 * 
 * @see SplayKeyCopy(). Same thing but for splay_value.
 */
splay_value*
SplayValueCopy (const splay_value* const value);

/**
 * @brief Splay value destructor.
 * 
 * @param[in] value splay_value structure to destroy.
 * 
 * @retval NULL in all cases.
 * 
 * @see SplayKeyDestructor(). Same thing but for splay_value.
 */
splay_value*
SplayValueDestructor (splay_value* const value);
//-------------------------------------

//-------------------------------------
// Insert functions
//-------------------------------------
/**
 * @brief Splay tree insert function.
 * 
 * @param[in] tree  splay_tree structure to insert into.
 * @param[in] key   splay_key   to insert.
 * @param[in] value splay_value to insert.
 * 
 * @retval SPLAY_TREE_ERROR   if error occured.
 * @retval SPLAY_TREE_SUCCESS otherwise.
 * 
 * If tree has no root, calls for SplayInsertRoot() function, returns its status.
 * Calls for SplayTreeInsertImpl() otherwise.
 * Calls for Splay function in the end to keep new node in the root.
 */
splay_error_t
SplayTreeInsert (splay_tree*        const tree,
                 const splay_key*   const key,
                 const splay_value* const value);

/**
 * @brief Splay tree insert root function.
 * 
 * @param[in] tree  splay_tree structure to insert into.
 * @param[in] key   splay_key to insert.
 * @param[in] value splay_value to insert.
 * 
 * @retval SPLAY_TREE_ERROR   if error occured.
 * @retval SPLAY_TREE_SUCCESS otherwise.
 * 
 * Constructs a new node via key-value parameters and inserts it into the tree root.
 */
splay_error_t
SplayInsertRoot (splay_tree*        const tree,
                 const splay_key*   const key,
                 const splay_value* const value);

/**
 * @brief Splay tree insert implementation function.
 * 
 * @param[in] tree  splay_tree structure to insert into.
 * @param[in] key   splay_key to insert.
 * @param[in] value splay_value to insert.
 * 
 * @retval pointer to the new node.
 * @retval NULL if error occured.
 * 
 * Constructs a new node via key-value parameters.
 * Searches for a place to insert new node via comparator function from the tree.
 * Search implementation is similar to naive BST.
 */
splay_node*
SplayTreeInsertImpl (splay_tree*        const tree,
                     const splay_key *  const key,
                     const splay_value* const value);
//-------------------------------------

//-------------------------------------
// Delete functions
//-------------------------------------
/**
 * @brief Splay tree delete by key function.
 * 
 * @param[in] tree splay_tree structure to delete from.
 * @param[in] key  splay_key to find the node to delete.
 * 
 * @retval SPLAY_TREE_ERROR   if error occured.
 * @retval SPLAY_TREE_SUCCESS otherwise.
 * 
 * Searches for the node to delete using SplayTreeFind() function.
 * If no node found, returns SPLAY_TREE_ERROR.
 * Otherwise, after SplayTreeFind() function the node is located in the root.
 * Replaces it with the node from SplayFindMaxKey()
 * and deletes the node using SplayNodeDestructor() function.
 */
splay_error_t
SplayTreeDeleteKey (splay_tree*      const tree,
                    const splay_key* const key);

/**
 * @brief Splay tree find max key function.
 * 
 * @param[in] tree splay_tree structure to find the node.
 * 
 * @retval pointer to the node.
 * @retval NULL if tree is empty.
 * 
 * The function searches for the max possible key value (goes to the right subtree).
 * Calls for Splay() function from the found node to place it in the root.
*/
splay_node*
SplayFindMaxKey (splay_tree* const tree);
//-------------------------------------


//-------------------------------------
// Find functions.
//-------------------------------------
/**
 * @brief Splay tree find by key function.
 * 
 * @param[in] tree splay_tree structure to find the node.
 * @param[in] key  splay_key  structure to compare to.
 * 
 * @retval pointer to the node, NULL if not found.
 * 
 * The function searches for the node with equal key
 * using key_cmp function from the tree structure.
 * Calls for Splay() function from the found node to place it in the root.
*/
splay_node*
SplayTreeFind (splay_tree*      const tree,
               const splay_key* const key);
//-------------------------------------

//-------------------------------------
// Splay and rotate functions
//-------------------------------------
/**
 * @brief Splay function
 * 
 * @param[in] tree splay_tree structure where the node is located.
 * @param[in] node splay_node structure to place in the root.
 * 
 * Splay function performs several possible rotations
 * to place the given node in the root of the tree
 * The rotations depend on the node's parent and grand parent position.
 * Three rotations are possible: zig, zig-zig and zig-zag.
 */
void
Splay (splay_tree* const tree,
       splay_node* const node);

/**
 * @brief Zig rotation of the node.
 * 
 * @param[in] node the node to rotate.
 * 
 * This rotation may occure only once for the Splay cycle.
 * It is called if the node's parent is the tree's root.
 * It calls for SplayRotateLeft() or SplayRotateRight() functions.
 * Right rotation example to place node n in the root:
 * 
 *     p              n
 *    / \            / \
 *   n   c   =>     a   p
 *  / \                / \
 * a   b              b   c
 * 
 */
void
SplayZig (splay_node* const node);

/**
 * @brief Zig-zig rotation of the node.
 * 
 * @param[in] node the node to rotate.
 * 
 * This rotation is called when node and its parent are
 * both right children or both left children.
 * Firstly, calls for rotate function around the parent node,
 * then around the given node.
 * Left children case:
 * 
 *       g               p              n
 *      / \             / \            / \
 *     p   d           /   g          a   p
 *    / \             /   / \            / \
 *   n   c     =>    n   c   d   =>     b   g
 *  / \             / \                    / \
 * a   b           a   b                  c   d
 * 
 */
void
SplayZigZig (splay_node* const node);

/**
 * @brief Zig-zag rotation of the node.
 * 
 * @param[in] node the node to rotate.
 * 
 * This rotation is called when node and its parent are
 * different children, one is right and the other is left.
 * Calls for two rotations around the given node.
 * Example:
 * 
 *       g              g                    n
 *      / \            / \                  / \
 *     p   d          n   d                /   g
 *    / \            / \                  /   / \
 *   a   n     =>   p   c      =>        p   c   d
 *      / \        / \                  / \
 *     b   c      a   b                a   b
 * 
 */
void
SplayZigZag (splay_node* const node);

/**
 * @brief Left rotation around the given node.
 * 
 * @param[in] node the node to rotate around.
 * 
 * Makes node's parent its left child.
 */
void
SplayRotateLeft (splay_node* const node);

/**
 * @brief Right rotation around the given node.
 * 
 * @param[in] node the node to rotate around.
 * 
 * Makes node's parent its right child.
 */
void
SplayRotateRight (splay_node* const node);
//-------------------------------------

//-------------------------------------
// Little helper functions.
//-------------------------------------
/**
 * @brief Get node's parent pointer.
 * 
 * @param[in] node the node to get parent from.
 * 
 * @retval pointer to the node's parent.
 * @retval NULL if node == NULL or parent doesn't exist.
 * 
 * Returns node->parent.
 */
splay_node*
SplayGetParent (const splay_node* const node);

/**
 * @brief Get node's grandparent pointer.
 * 
 * @param[in] node the node to get parent from.
 * 
 * @retval pointer to the node's grandparent.
 * @retval NULL if node == NULL or grandparent doesn't exist.
 * 
 * SplayGetParent (SplayGetParent (node));
 */
splay_node*
SplayGetGrandParent (const splay_node* const node);

/**
 * @brief Checks whether the node is right child.
 * 
 * @param[in] node the node to check.
 * 
 * @retval true  if the node is right child.
 * @retval false if node == NULL, node's parent doesn't exist or it is left child.
 * 
 * Gets the node's parent and checks whether the node is its right child.
 * Returns boolean value.
 */
bool
SplayIsRightChild (const splay_node* const node);

/**
 * @brief Checks whether the node is left child.
 * 
 * @param[in] node the node to check.
 * 
 * @retval true  if the node is left child.
 * @retval false if node == NULL, node's parent doesn't exist or it is right child.
 * 
 * Gets the node's parent and checks whether the node is its left child.
 * Returns boolean value.
 */
bool
SplayIsLeftChild (const splay_node* const node);
//-------------------------------------
/******************************************************************************
 * @}
 ******************************************************************************/



//-----------------------------------------------------------------------------
// Splay tree functions' implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Constructor and destructor functions
splay_tree*
SplayTreeConstructor (int (*key_cmp) (const splay_key* const,
                                      const splay_key* const))
{
    splay_tree* const tree =
        (splay_tree*) calloc (1, sizeof (splay_tree));

    tree->key_cmp = key_cmp;

    return tree;
}

splay_tree*
SplayTreeDestructor (splay_tree* const tree)
{
    if (tree == NULL) return NULL;

    while (tree->root != NULL)
        SplayTreeDeleteKey (tree, tree->root->key);

    free (tree);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Splay node constructor and destructor
splay_node*
SplayNodeConstructor (const splay_key*   const key,
                      const splay_value* const value)
{
    splay_node* const node =
        (splay_node*) calloc (1, sizeof (splay_node));
    if (node == NULL) return NULL;

    node->key   = SplayKeyCopy   (key);
    node->value = SplayValueCopy (value);

    return node;
}

splay_node*
SplayNodeDestructor (splay_node* const node)
{
    if (node == NULL) return NULL;

    node->key    = SplayKeyDestructor   (node->key);
    node->value  = SplayValueDestructor (node->value);
    node->left   = NULL;
    node->right  = NULL;
    node->parent = NULL;

    free (node);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Splay key and value constructor, destructor and copy
splay_key*
SplayKeyConstructor (const void* const key,
                     const size_t key_len)
{
    splay_key* const new_key =
        (splay_key*) malloc (sizeof (splay_key));
    if (new_key == NULL) return NULL;

    if (key != NULL && key_len != 0)
    {
        new_key->key = malloc (key_len);
        if (new_key->key == NULL)
            return SplayKeyDestructor (new_key);

        memcpy (new_key->key, key, key_len);
        new_key->key_len = key_len;
    }

    return new_key;
}

splay_key*
SplayKeyCopy (const splay_key* const key)
{
    if (key == NULL) return NULL;
    return SplayKeyConstructor (key->key, key->key_len);
}

splay_key*
SplayKeyDestructor (splay_key* const key)
{
    if (key == NULL) return NULL;

    free (key->key);
    free (key);
    return NULL;
}

splay_value*
SplayValueConstructor (const void* const value,
                       const size_t value_len)
{
    splay_value* const new_value =
        (splay_value*) malloc (sizeof (splay_value));
    if (new_value == NULL) return NULL;

    if (value != NULL && value_len != 0)
    {
        new_value->value = malloc (value_len);
        if (new_value->value == NULL)
            return SplayValueDestructor (new_value);

        memcpy (new_value->value, value, value_len);
        new_value->value_len = value_len;
    }

    return new_value;
}

splay_value*
SplayValueCopy (const splay_value* const value)
{
    if (value == NULL) return NULL;
    return SplayValueConstructor (value->value, value->value_len);
}

splay_value*
SplayValueDestructor (splay_value* const value)
{
    if (value == NULL) return NULL;

    free (value->value);
    free (value);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Insert functions implementation
splay_error_t
SplayTreeInsert (splay_tree*        const tree,
                 const splay_key*   const key,
                 const splay_value* const value)
{
    if (tree == NULL)
        return SPLAY_TREE_ERROR;

    if (tree->root == NULL)
        return SplayInsertRoot (tree, key, value);

    splay_node* const new_node = SplayTreeInsertImpl (tree, key, value);
    if (new_node == NULL) return SPLAY_TREE_ERROR;

    Splay (tree, new_node);
    return SPLAY_TREE_SUCCESS;
}

splay_error_t
SplayInsertRoot (splay_tree*        const tree,
                 const splay_key*   const key,
                 const splay_value* const value)
{
    tree->root = SplayNodeConstructor (key, value);

    if (tree->root == NULL) return SPLAY_TREE_ERROR;
    return SPLAY_TREE_SUCCESS;
}

splay_node*
SplayTreeInsertImpl (splay_tree*        const tree,
                     const splay_key *  const key,
                     const splay_value* const value)
{
    splay_node* node = tree->root;
    splay_node* prev = NULL;
    splay_node* next = NULL;
    bool is_left_child = false;

    while (node != NULL)
    {
        if (tree->key_cmp (key, node->key) <= 0)
        {
            next = node->left;
            is_left_child = true;
        }
        else
        {
            next = node->right;
            is_left_child = false;
        }

        prev = node;
        node = next;
    }

    splay_node* new_node = SplayNodeConstructor (key, value);
    if (new_node == NULL) return NULL;

    if (is_left_child)
        prev->left  = new_node;
    else
        prev->right = new_node;

    new_node->parent = prev;

    return new_node;
}
//-------------------------------------

//-------------------------------------
// Delete functions
splay_error_t
SplayTreeDeleteKey (splay_tree*      const tree,
                    const splay_key* const key)
{
    if (tree == NULL) return SPLAY_TREE_ERROR;

    splay_node* const del_node = SplayTreeFind (tree, key);
    if (del_node == NULL) return SPLAY_TREE_ERROR;

    splay_node* const prev_right = tree->root->right;
    splay_node* const prev_left  = tree->root->left;

    if (prev_left != NULL)
    {
        tree->root = prev_left;
        prev_left->parent = NULL;

        Splay (tree, SplayFindMaxKey (tree));
        tree->root->right = prev_right;
    }

    else
    {
        tree->root = prev_right;
        if (prev_right != NULL)
            prev_right->parent = NULL;
    }

    SplayNodeDestructor (del_node);

    return SPLAY_TREE_SUCCESS;
}

splay_node*
SplayFindMaxKey (splay_tree* const tree)
{
    if (tree       == NULL ||
        tree->root == NULL)
        return NULL;

    splay_node* node = tree->root;

    while (node->right != NULL)
        node = node->right;

    Splay (tree, node);

    return node;
}
//-------------------------------------

//-------------------------------------
// Find functions implementation
splay_node*
SplayTreeFind (splay_tree*      const tree,
               const splay_key* const key)
{
    if (tree == NULL ||
        key  == NULL) return NULL;

    splay_node* node = tree->root;
    int direction    = 0;

    while (node != NULL)
    {
        direction = strcmp (key->key, node->key->key);

        if (direction < 0)
            node = node->left;

        else if (direction > 0)
            node = node->right;

        else
        {
            Splay (tree, node);
            return node;
        }
    }

    return NULL;
}
//-------------------------------------

//-------------------------------------
// Splay and rotate functions
void
Splay (splay_tree* const tree,
       splay_node* const node)
{
    if (tree == NULL ||
        node == NULL)
        return;

    splay_node* parent = NULL;

    while (SplayGetParent (node) != NULL)
    {
        parent = SplayGetParent (node);

        if (SplayGetGrandParent (node) == NULL)
        {
            SplayZig (node);
        }

        else if ((SplayIsLeftChild  (node) && SplayIsLeftChild  (parent)) ||
                 (SplayIsRightChild (node) && SplayIsRightChild (parent)))
        {
            SplayZigZig (node);
        }

        else if ((SplayIsRightChild (node) && SplayIsLeftChild  (parent)) ||
                 (SplayIsLeftChild  (node) && SplayIsRightChild (parent)))
        {
            SplayZigZag(node);
        }
    }

    tree->root = node;
}

void
SplayZig (splay_node* const node)
{
    splay_node* const parent = SplayGetParent (node);

    if (SplayIsLeftChild (node))
        SplayRotateRight (parent);

    else SplayRotateLeft (parent);
}

void
SplayZigZig (splay_node* const node)
{
    if (SplayIsLeftChild (node))
    {
        SplayRotateRight (SplayGetGrandParent (node));
        SplayRotateRight (SplayGetParent      (node));
    }

    else
    {
        SplayRotateLeft (SplayGetGrandParent (node));
        SplayRotateLeft (SplayGetParent      (node));
    }
}

void
SplayZigZag (splay_node* const node)
{
    if (SplayIsLeftChild (node))
    {
        SplayRotateRight (SplayGetParent (node));
        SplayRotateLeft  (SplayGetParent (node));
    }

    else
    {
        SplayRotateLeft  (SplayGetParent (node));
        SplayRotateRight (SplayGetParent (node));
    }
}

void
SplayRotateLeft (splay_node* const q)
{
    splay_node* p     = q->right;
    splay_node* q_par = q->parent;

    bool isset   = q_par != NULL;
    bool is_left = isset && q_par->left == q;

    q->right = p->left;

    if (p->left)
        p->left->parent = q;

    p->left   = q;
    q->parent = p;
    p->parent = q_par;

    if (isset && is_left)
        q_par->left = p;

    else if (isset)
        q_par->right = p;
}

void
SplayRotateRight (splay_node* const p)
{
    splay_node* q     = p->left;
    splay_node* p_par = p->parent;

    bool isset = p_par != NULL;
    bool is_left = isset && p_par->left == p;

    p->left = q->right;

    if (q->right)
        q->right->parent = p;

    q->right  = p;
    p->parent = q;
    q->parent = p_par;

    if (isset && is_left)
        p_par->left = q;

    else if (isset)
        p_par->right = q;
}
//-------------------------------------

//-------------------------------------
// Little helper functions
splay_node*
SplayGetParent (const splay_node* const node)
{
    if (node == NULL) return NULL;
    return node->parent;
}

splay_node*
SplayGetGrandParent (const splay_node* const node)
{
    if (node == NULL) return NULL;
    return SplayGetParent (SplayGetParent (node));
}

bool
SplayIsRightChild (const splay_node* const node)
{
    splay_node* const parent = SplayGetParent (node);

    if (parent == NULL) return false;
    return parent->right == node;
}

bool
SplayIsLeftChild (const splay_node* const node)
{
    splay_node* const parent = SplayGetParent (node);

    if (parent == NULL) return false;
    return parent->left == node;
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Main functions
//-----------------------------------------------------------------------------
int key_cmp (const splay_key* const key1, const splay_key* const key2)
{
    return strcmp (key1->key, key2->key);
}

void
ReadString (d_array_t* const input_array)
{
    assert (input_array);

    input_array->size = 0;
    char c = 0;

    while (true)
    {
        c = getchar ();

        if (isspace (c))
        {
            c = '\0';
            DynamicArrayPush (input_array, &c);
            break;
        }

        else
            DynamicArrayPush (input_array, &c);
    }
}

void
ReadTree (splay_tree* const tree,
          splay_tree* const rev_tree,
          const size_t N)
{
    d_array_t* key_str = DynamicArrayConstructor (sizeof (char));
    assert (key_str);
    
    d_array_t* value_str = DynamicArrayConstructor (sizeof (char));
    assert (value_str);

    splay_key* key = SplayKeyConstructor (NULL, 0);
    assert (key);

    splay_value* value = SplayValueConstructor (NULL, 0);
    assert (value);

    for (size_t i = 0; i < N; ++i)
    {
        ReadString (key_str);
        ReadString (value_str);

        key->key     = key_str->data;
        value->value = value_str->data;

        key->key_len     = key_str->size;
        value->value_len = value_str->size;

        SplayTreeInsert (tree, key, value);
        SplayTreeInsert (rev_tree, (splay_key*)value, (splay_value*)key);
    }

    key_str   = DynamicArrayDestructor (key_str);
    value_str = DynamicArrayDestructor (value_str);

    key->key     = NULL;
    value->value = NULL;

    key       = SplayKeyDestructor     (key);
    value     = SplayValueDestructor   (value);
}

void
ExecuteRequests (splay_tree* const tree,
                 splay_tree* const rev_tree,
                 const size_t Q)
{
    d_array_t*  key_str = DynamicArrayConstructor (sizeof (char));
    splay_key*  key     = SplayKeyConstructor (NULL, 0);
    splay_node* node    = NULL;

    for (size_t i = 0; i < Q; ++i)
    {
        ReadString (key_str);
        key->key = key_str->data;
        key->key_len = key_str->size;

        node = SplayTreeFind (tree, key);

        if (node == NULL)
            node = SplayTreeFind (rev_tree, key);

        printf ("%s\n", (char*)node->value->value);
    }

    key_str = DynamicArrayDestructor (key_str);

    key->key = NULL;
    key = SplayKeyDestructor (key);
}

int main (void)
{
    splay_tree* tree = SplayTreeConstructor (key_cmp);
    assert (tree);

    splay_tree* rev_tree = SplayTreeConstructor (key_cmp);
    assert (rev_tree);

    size_t N = 0;
    assert (scanf ("%zd ", &N) == 1);
    ReadTree (tree, rev_tree, N);

    size_t Q = 0;
    assert (scanf ("%zd ", &Q) == 1);
    ExecuteRequests (tree, rev_tree, Q);

    tree     = SplayTreeDestructor (tree);
    rev_tree = SplayTreeDestructor (rev_tree);
    return 0;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
