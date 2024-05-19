#include "../include/splay.h"

#include <stdio.h>
#include <assert.h>

/******************************************************************************
 * @name Splay tree static functions prototypes
 * 
 * @{
 ******************************************************************************/
//-------------------------------------
// Key and value copy
/**
 * @brief Copies key from the given one.
 * 
 * @param[in] key splay_key structure to copy.
 * 
 * @retval Pointer to the allocated structure.
 * @retval NULL if allocation error occurred.
 * 
 * @details Calls for SplayKeyConstructor (key->key, key->value).
 */
static splay_key*
SplayKeyCopy (const splay_key* const key);

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
static splay_value*
SplayValueCopy (const splay_value* const value);
//-------------------------------------

//-------------------------------------
// Interface helpers
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
 * @details Constructs a new node via key-value parameters and inserts it into the tree root.
 */
static splay_error_t
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
 * @details Constructs a new node via key-value parameters.
 * Searches for a place to insert new node via comparator function from the tree.
 * Search implementation is similar to naive BST.
 */
static splay_node*
SplayTreeInsertImpl (splay_tree*        const tree,
                     const splay_key *  const key,
                     const splay_value* const value);

/**
 * @brief Splay tree find max key function.
 * 
 * @param[in] tree splay_tree structure to find the node.
 * 
 * @retval pointer to the node.
 * @retval NULL if tree is empty.
 * 
 * @details The function searches for the max possible key value (goes to the right subtree).
 * Calls for Splay() function from the found node to place it in the root.
*/
static splay_node*
SplayFindMaxKey (splay_tree* const tree);
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
 * @details Splay function performs several possible rotations
 * to place the given node in the root of the tree
 * The rotations depend on the node's parent and grand parent position.
 * Three rotations are possible: zig, zig-zig and zig-zag.
 */
static void
Splay (splay_tree* const tree,
       splay_node* const node);

/**
 * @brief Zig rotation of the node.
 * 
 * @param[in] node the node to rotate.
 * 
 * @details This rotation may occure only once for the Splay cycle.
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
static void
SplayZig (splay_node* const node);

/**
 * @brief Zig-zig rotation of the node.
 * 
 * @param[in] node the node to rotate.
 * 
 * @details This rotation is called when node and its parent are
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
static void
SplayZigZig (splay_node* const node);

/**
 * @brief Zig-zag rotation of the node.
 * 
 * @param[in] node the node to rotate.
 * 
 * @details This rotation is called when node and its parent are
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
static void
SplayZigZag (splay_node* const node);

/**
 * @brief Left rotation around the given node.
 * 
 * @param[in] node the node to rotate around.
 * 
 * @details Makes node's parent its left child.
 */
static void
SplayRotateLeft (splay_node* const node);

/**
 * @brief Right rotation around the given node.
 * 
 * @param[in] node the node to rotate around.
 * 
 * @details Makes node's parent its right child.
 */
static void
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
 * @details Returns node->parent.
 */
static splay_node*
SplayGetParent (const splay_node* const node);

/**
 * @brief Get node's grandparent pointer.
 * 
 * @param[in] node the node to get parent from.
 * 
 * @retval pointer to the node's grandparent.
 * @retval NULL if node == NULL or grandparent doesn't exist.
 * 
 * @details SplayGetParent (SplayGetParent (node));
 */
static splay_node*
SplayGetGrandParent (const splay_node* const node);

/**
 * @brief Checks whether the node is right child.
 * 
 * @param[in] node the node to check.
 * 
 * @retval true  if the node is right child.
 * @retval false if node == NULL, node's parent doesn't exist or it is left child.
 * 
 * @details Gets the node's parent and checks whether the node is its right child.
 * Returns boolean value.
 */
static bool
SplayIsRightChild (const splay_node* const node);

/**
 * @brief Checks whether the node is left child.
 * 
 * @param[in] node the node to check.
 * 
 * @retval true  if the node is left child.
 * @retval false if node == NULL, node's parent doesn't exist or it is right child.
 * 
 * @details Gets the node's parent and checks whether the node is its left child.
 * Returns boolean value.
 */
static bool
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
// Splay key and value constructor and destructor
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
SplayValueDestructor (splay_value* const value)
{
    if (value == NULL) return NULL;

    free (value->value);
    free (value);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Interface
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
        if (prev_right) prev_right->parent = tree->root;
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
SplayTreeFind (splay_tree*      const tree,
               const splay_key* const key)
{
    if (tree == NULL ||
        key  == NULL) return NULL;

    splay_node* node = tree->root;
    int cmp_status   = 0;

    while (node != NULL)
    {
        // printf ("%p ", node);
        cmp_status = tree->key_cmp (key, node->key);

        if (cmp_status == SPLAY_TREE_CMP_LESS)
            node = node->left;

        else if (cmp_status == SPLAY_TREE_CMP_GREATER)
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
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Splay tree static functions implementation
//-----------------------------------------------------------------------------
static splay_key*
SplayKeyCopy (const splay_key* const key)
{
    if (key == NULL) return NULL;
    return SplayKeyConstructor (key->key, key->key_len);
}

static splay_value*
SplayValueCopy (const splay_value* const value)
{
    if (value == NULL) return NULL;
    return SplayValueConstructor (value->value, value->value_len);
}
//-------------------------------------

//-------------------------------------
// Interface helpers
static splay_error_t
SplayInsertRoot (splay_tree*        const tree,
                 const splay_key*   const key,
                 const splay_value* const value)
{
    tree->root = SplayNodeConstructor (key, value);

    if (tree->root == NULL) return SPLAY_TREE_ERROR;
    return SPLAY_TREE_SUCCESS;
}

static splay_node*
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

static splay_node*
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
// Splay and rotate functions
static void
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

static void
SplayZig (splay_node* const node)
{
    splay_node* const parent = SplayGetParent (node);

    if (SplayIsLeftChild (node))
        SplayRotateRight (parent);

    else SplayRotateLeft (parent);
}

static void
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

static void
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

static void
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

static void
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
static splay_node*
SplayGetParent (const splay_node* const node)
{
    if (node == NULL) return NULL;
    return node->parent;
}

static splay_node*
SplayGetGrandParent (const splay_node* const node)
{
    if (node == NULL) return NULL;
    return SplayGetParent (SplayGetParent (node));
}

static bool
SplayIsRightChild (const splay_node* const node)
{
    splay_node* const parent = SplayGetParent (node);

    if (parent == NULL) return false;
    return parent->right == node;
}

static bool
SplayIsLeftChild (const splay_node* const node)
{
    splay_node* const parent = SplayGetParent (node);

    if (parent == NULL) return false;
    return parent->left == node;
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
