#include "../include/avl_tree.h"



//-----------------------------------------------------------------------------
// AVL tree static functions' prototypes
//-----------------------------------------------------------------------------
//-------------------------------------
// Interface helpers
static avl_tree_key*
AVLTreeKeyCopy         (const avl_tree_key* const key);

static avl_tree_value*
AVLTreeValueCopy       (const avl_tree_value* const value);

static avl_tree_node*
AVLTreeDeleteBranch    (avl_tree*      const tree,
                        avl_tree_node* const node);

static avl_tree_node*
AVLTreeInsertImpl      (avl_tree*             const tree,
                        avl_tree_node*        const node,
                        const avl_tree_key*   const key,
                        const avl_tree_value* const value);

static avl_tree_node*
AVLTreeDeleteImpl      (avl_tree*           const tree,
                        avl_tree_node*      const node,
                        const avl_tree_key* const key);

static void
AVLTreeReplaceKeyAndValue (avl_tree_node*       const node,
                           const avl_tree_node* const replacement);

static avl_tree_node*
AVLTreeFindMaxInBranch (avl_tree_node* const node);
//-------------------------------------

//-------------------------------------
// Fixing balance
static avl_tree_node*
AVLTreeFixBalance  (avl_tree_node* const node);

static void
AVLTreeFixHeight   (avl_tree_node* const node);

static avl_tree_node*
AVLTreeRotateLeft  (avl_tree_node* const node);

static avl_tree_node*
AVLTreeRotateRight (avl_tree_node* const node);
//-------------------------------------

//-------------------------------------
// Common helpers
static int
AVLSubtreeHeight (const avl_tree_node* const node);

static int
AVLSubtreeCalculateBalance (const avl_tree_node* const node);

static int
MaxInt (const int a, const int b);
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// AVL-tree functions' implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Constructor and destructor
avl_tree*
AVLTreeConstructor (int (*key_cmp) (const avl_tree_key* const,
                                    const avl_tree_key* const))
{
    if (key_cmp == NULL) return NULL;

    avl_tree* const tree =
        (avl_tree*) malloc (sizeof (avl_tree));
    if (tree == NULL) return NULL;

    tree->key_cmp  = key_cmp;
    tree->root     = NULL;

    return tree;
}

avl_tree*
AVLTreeDestructor (avl_tree* const tree)
{
    if (tree == NULL) return NULL;

    tree->key_cmp = NULL;

    tree->root = AVLTreeDeleteBranch (tree, tree->root);

    free (tree);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Interface
avl_tree_error_t
AVLTreeInsert (avl_tree*             const tree,
               const avl_tree_key*   const key,
               const avl_tree_value* const value)
{
    if (tree          == NULL ||
        tree->key_cmp == NULL ||
        key           == NULL)
        return AVL_TREE_ERROR;

    avl_tree_node* const new_root =
        AVLTreeInsertImpl (tree, tree->root, key, value);

    if (new_root == NULL) return AVL_TREE_ERROR;

    tree->root = new_root;

    return AVL_TREE_SUCCESS;
}

avl_tree_error_t
AVLTreeDelete (avl_tree*           const tree,
               const avl_tree_key* const key)
{
    avl_tree_node* const del_node = AVLTreeFindKey (tree, key);
    if (del_node == NULL) return AVL_TREE_ERROR;

    tree->root = AVLTreeDeleteImpl (tree, tree->root, key);

    return AVL_TREE_SUCCESS;
}

avl_tree_node*
AVLTreeFindKey (avl_tree*           const tree,
                const avl_tree_key* const key)
{
    if (tree          == NULL ||
        tree->key_cmp == NULL ||
        key           == NULL)
        return NULL;

    int cmp_status = AVL_TREE_CMP_EQUAL;
    avl_tree_node* cur_node = tree->root;

    while (cur_node != NULL)
    {
        cmp_status = tree->key_cmp (key, cur_node->key);

        if (cmp_status == AVL_TREE_CMP_LESS)
            cur_node = cur_node->left;

        else if (cmp_status == AVL_TREE_CMP_GREATER)
            cur_node = cur_node->right;

        else return cur_node;
    }

    return NULL;
}
//-------------------------------------

//-------------------------------------
// AVL tree node, key and value constructors and destructors
avl_tree_node*
AVLTreeNodeConstructor (const avl_tree_key*   const key,
                        const avl_tree_value* const value)
{
    avl_tree_node* const node =
        (avl_tree_node*) calloc (1, sizeof (avl_tree_node));
    if (node == NULL) return NULL;

    node->key   = AVLTreeKeyCopy   (key);
    node->value = AVLTreeValueCopy (value);

    node->branch_height = 1;

    return node;
}

avl_tree_node*
AVLTreeNodeDestructor (avl_tree_node* const node)
{
    if (node == NULL) return NULL;

    node->key   = AVLTreeKeyDestructor   (node->key);
    node->value = AVLTreeValueDestructor (node->value);

    node->left  = NULL;
    node->right = NULL;
    node->branch_height = 0;

    free (node);
    return NULL;
}

avl_tree_key*
AVLTreeKeyConstructor (const void* const data,
                       const size_t key_size)
{
    avl_tree_key* const key =
        (avl_tree_key* const) malloc (sizeof (avl_tree_key));
    if (key == NULL) return NULL;

    if (key != NULL && key_size != 0)
    {
        key->data = malloc (key_size);
        if (key->data == NULL)
            return AVLTreeKeyDestructor (key);

        memcpy (key->data, data, key_size);
        key->key_size = key_size;
    }

    return key;
}

avl_tree_key*
AVLTreeKeyDestructor (avl_tree_key* const key)
{
    if (key == NULL) return NULL;

    free (key->data);
    free (key);
    return NULL;
}

avl_tree_value*
AVLTreeValueConstructor (const void* const data,
                         const size_t value_size)
{
    avl_tree_value* const value =
        (avl_tree_value* const) malloc (sizeof (avl_tree_value));
    if (value == NULL) return NULL;

    if (value != NULL && value_size != 0)
    {
        value->data = malloc (value_size);
        if (value->data == NULL)
            return AVLTreeValueDestructor (value);

        memcpy (value->data, data, value_size);
        value->value_size = value_size;
    }

    return value;
}

avl_tree_value*
AVLTreeValueDestructor (avl_tree_value* const value)
{
    if (value == NULL) return NULL;

    free (value->data);
    free (value);
    return NULL;
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// AVL tree static functions implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Fixing balance
static avl_tree_node*
AVLTreeFixBalance (avl_tree_node* const node)
{
    if (node == NULL) return NULL;
    AVLTreeFixHeight (node);

    const int balance_node  = AVLSubtreeCalculateBalance (node);
    const int balance_left  = AVLSubtreeCalculateBalance (node->left);
    const int balance_right = AVLSubtreeCalculateBalance (node->right);

    if (balance_node == AVL_TREE_SMALL_RIGHT_ROTATE)
    {
        if (balance_left == AVL_TREE_BIG_RIGHT_ROTATE)
            node->left = AVLTreeRotateLeft (node->left);

        return AVLTreeRotateRight (node);
    }

    if (balance_node == AVL_TREE_SMALL_LEFT_ROTATE)
    {
        if (balance_right == AVL_TREE_BIG_LEFT_ROTATE)
            node->right = AVLTreeRotateRight (node->right);

        return AVLTreeRotateLeft (node);
    }

    return node;
}

static void
AVLTreeFixHeight (avl_tree_node* const node)
{
    if (node == NULL) return;
    node->branch_height = MaxInt(AVLSubtreeHeight (node->left), 
                                 AVLSubtreeHeight (node->right)) + 1;
}

static avl_tree_node*
AVLTreeRotateLeft (avl_tree_node* const node)
{
    if (node == NULL) return NULL;

    avl_tree_node* const replacement = node->right;
    node->right = replacement->left;
    replacement->left = node;

    AVLTreeFixHeight (node);
    AVLTreeFixHeight (replacement);

    return replacement;
}

static avl_tree_node*
AVLTreeRotateRight (avl_tree_node* const node)
{
    if (node == NULL) return NULL;

    avl_tree_node* const replacement = node->left;
    node->left = replacement->right;
    replacement->right = node;

    AVLTreeFixHeight (node);
    AVLTreeFixHeight (replacement);

    return replacement;
}
//-------------------------------------

//-------------------------------------
// Interface helpers
static avl_tree_node*
AVLTreeDeleteBranch (avl_tree*      const tree,
                     avl_tree_node* const node)
{
    if (node == NULL) return NULL;

    node->left  = AVLTreeDeleteBranch (tree, node->left);
    node->right = AVLTreeDeleteBranch (tree, node->right);

    return AVLTreeNodeDestructor (node);
}

static avl_tree_node*
AVLTreeInsertImpl (avl_tree*             const tree,
                   avl_tree_node*        const node,
                   const avl_tree_key*   const key,
                   const avl_tree_value* const value)
{
    if (node == NULL)
        return AVLTreeNodeConstructor (key, value);

    if (tree->key_cmp (key, node->key) == AVL_TREE_CMP_GREATER)
         node->right = AVLTreeInsertImpl (tree, node->right, key, value);

    else node->left  = AVLTreeInsertImpl (tree, node->left,  key, value);

    return AVLTreeFixBalance (node);
}

static avl_tree_node*
AVLTreeFindMaxInBranch (avl_tree_node* const node)
{
    if (node == NULL)
        return NULL;

    avl_tree_node* ret_node = node;

    while (ret_node->right != NULL)
        ret_node = ret_node->right;

    return ret_node;
}

static avl_tree_node*
AVLTreeDeleteImpl (avl_tree*           const tree,
                   avl_tree_node*      const node,
                   const avl_tree_key* const key)
{
    if (node == NULL) return NULL;
    
    avl_tree_node* replacement = NULL;

    if (tree->key_cmp (key, node->key) == AVL_TREE_CMP_LESS)
        node->left = AVLTreeDeleteImpl (tree, node->left, key);

    else if (tree->key_cmp (key, node->key) == AVL_TREE_CMP_GREATER)
        node->right = AVLTreeDeleteImpl (tree, node->right, key);

    else
    {
        if (node->left == NULL || node->right == NULL)
        {
            replacement = node->left ? node->left : node->right;

            AVLTreeNodeDestructor (node);
            return AVLTreeFixBalance (replacement);
        }

        replacement = AVLTreeFindMaxInBranch (node->left);
        AVLTreeReplaceKeyAndValue (node, replacement);
        
        node->left = AVLTreeDeleteImpl (tree, node->left, replacement->key);
    }

    return AVLTreeFixBalance (node);
}

static void
AVLTreeReplaceKeyAndValue (avl_tree_node*       const node,
                           const avl_tree_node* const replacement)
{
    if (node == NULL || replacement == NULL) return;
    
    node->key = AVLTreeKeyDestructor  (node->key);
    node->key = AVLTreeKeyCopy (replacement->key);

    node->value = AVLTreeValueDestructor  (node->value);
    node->value = AVLTreeValueCopy (replacement->value);
}

static avl_tree_key*
AVLTreeKeyCopy (const avl_tree_key* const key)
{
    if (key == NULL) return NULL;
    return AVLTreeKeyConstructor (key->data, key->key_size);
}

static avl_tree_value*
AVLTreeValueCopy (const avl_tree_value* const value)
{
    if (value == NULL) return NULL;
    return AVLTreeValueConstructor (value->data, value->value_size);
}
//-------------------------------------

//-------------------------------------
// Common helpers
static int
AVLSubtreeHeight (const avl_tree_node* const node)
{
    if (node == NULL) return 0;
    return node->branch_height;
}

static int
AVLSubtreeCalculateBalance (const avl_tree_node* const node)
{
    if (node == NULL) return 0;
    return AVLSubtreeHeight (node->left) - AVLSubtreeHeight (node->right);
}

static int
MaxInt (const int a, const int b)
{
    return a > b ? a : b;
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
