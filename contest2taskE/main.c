#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>



//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------
const int AVL_TREE_SMALL_LEFT_ROTATE  = -2;
const int AVL_TREE_SMALL_RIGHT_ROTATE =  2;
const int AVL_TREE_BIG_LEFT_ROTATE    =  1;
const int AVL_TREE_BIG_RIGHT_ROTATE   = -1;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// AVL-tree structs
//-----------------------------------------------------------------------------
typedef
struct avl_tree_key
{
    void*  data;
    size_t key_size;
}
avl_tree_key;

typedef
struct avl_tree_node
{
    avl_tree_key*  key;
    struct avl_tree_node* left;
    struct avl_tree_node* right;
    int branch_height;
}
avl_tree_node;

typedef
struct avl_tree
{
    avl_tree_node* head;
    size_t elem_num;
    int (*compare) (const avl_tree_key* const,
                    const avl_tree_key* const);
}
avl_tree;

enum avl_tree_errors
{
    AVL_TREE_SUCCESS = 0,
    AVL_TREE_ERROR   = 1
};

typedef size_t avl_tree_error_t;

enum avl_tree_cmp_status
{
    AVL_TREE_CMP_LESS    = -1,
    AVL_TREE_CMP_EQUAL   =  0,
    AVL_TREE_CMP_GREATER =  1
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// AVL-tree functions' prototypes
//-----------------------------------------------------------------------------
avl_tree*
AVLTreeConstructor (int (*compare) (const avl_tree_key* const,
                                    const avl_tree_key* const));

avl_tree*
AVLTreeDestructor (avl_tree* const tree);

avl_tree_node*
AVLTreeDeleteBranch (avl_tree*      const tree,
                     avl_tree_node* const node);

avl_tree_error_t
AVLTreeInsertKey (avl_tree*           const tree,
                  const avl_tree_key* const key);

avl_tree_node*
AVLTreeInsertImplementration (avl_tree*           const tree,
                              avl_tree_node*      const node,
                              const avl_tree_key* const key);

avl_tree_error_t
AVLTreeDeleteKey (avl_tree*           const tree,
                  const avl_tree_key* const key);

avl_tree_node*
AVLTreeDeleteImplementation (avl_tree*      const tree,
                             avl_tree_node* const node);

avl_tree_node*
AVLTreeUnlinkMax (avl_tree_node* const node);

avl_tree_node*
AVLTreeFindKey (avl_tree*           const tree,
                const avl_tree_key* const key);

avl_tree_node*
AVLTreeFindMinInBranch (avl_tree_node* const node);

avl_tree_node*
AVLTreeFindMaxInBranch (avl_tree_node* const node);

avl_tree_node*
AVLTreeNodeConstructor (const avl_tree_key* const key,
                        avl_tree_node* const left,
                        avl_tree_node* const right);

avl_tree_node*
AVLTreeNodeDestructor (avl_tree_node* const node);

avl_tree_key*
AVLTreeKeyConstructor (const void* const data,
                       const size_t key_size);

avl_tree_key*
AVLTreeKeyDestructor (avl_tree_key* const key);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// AVL tree balance functions' prototypes
//-----------------------------------------------------------------------------
avl_tree_node*
AVLTreeFixBalance (avl_tree_node* const node);

void
AVLTreeFixHeight (avl_tree_node* const node);

int
AVLSubtreeHeight (const avl_tree_node* const node);

int
AVLSubtreeCalculateBalance (const avl_tree_node* const node);

avl_tree_node*
AVLTreeRotateLeft (avl_tree_node* const node);

avl_tree_node*
AVLTreeRotateRight (avl_tree_node* const node);

int
MaxInt (const int a, const int b);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// AVL-tree functions' implementation
//-----------------------------------------------------------------------------
avl_tree*
AVLTreeConstructor (int (*compare) (const avl_tree_key* const,
                                    const avl_tree_key* const))
{
    if (compare == NULL) return NULL;

    avl_tree* const tree =
        (avl_tree*) malloc (sizeof (avl_tree));
    if (tree == NULL) return NULL;

    tree->compare  = compare;
    tree->elem_num = 0;
    tree->head     = NULL;

    return tree;
}

avl_tree*
AVLTreeDestructor (avl_tree* const tree)
{
    if (tree == NULL) return NULL;

    tree->compare = NULL;

    tree->head = AVLTreeDeleteBranch (tree, tree->head);

    free (tree);
    return NULL;
}

avl_tree_node*
AVLTreeDeleteBranch (avl_tree*      const tree,
                     avl_tree_node* const node)
{
    if (tree == NULL ||
        node == NULL)
        return NULL;

    node->left  = AVLTreeDeleteBranch (tree, node->left);
    node->right = AVLTreeDeleteBranch (tree, node->right);

    tree->elem_num--;
    return AVLTreeNodeDestructor (node);
}

avl_tree_error_t
AVLTreeInsertKey (avl_tree*           const tree,
                  const avl_tree_key* const key)
{
    if (tree          == NULL ||
        key           == NULL ||
        key->data     == NULL ||
        key->key_size == 0)
        return AVL_TREE_ERROR;

    tree->head = AVLTreeInsertImplementration (tree, tree->head, key);
    if (tree->head == NULL) return AVL_TREE_ERROR;

    tree->elem_num++;

    return AVL_TREE_SUCCESS;
}

avl_tree_node*
AVLTreeInsertImplementration (avl_tree*           const tree,
                              avl_tree_node*      const node,
                              const avl_tree_key* const key)
{
    if (node == NULL)
        return AVLTreeNodeConstructor (key, NULL, NULL);

    if (tree->compare (key, node->key) == AVL_TREE_CMP_GREATER)
         node->right = AVLTreeInsertImplementration (tree, node->right, key);

    else node->left  = AVLTreeInsertImplementration (tree, node->left,  key);

    return AVLTreeFixBalance (node);
}

avl_tree_error_t
AVLTreeDeleteKey (avl_tree*           const tree,
                  const avl_tree_key* const key)
{
    avl_tree_node* const del_node = AVLTreeFindKey (tree, key);
    if (del_node == NULL) return AVL_TREE_ERROR;

    tree->head = AVLTreeDeleteImplementation (tree, tree->head);

    tree->elem_num--;

    return AVL_TREE_SUCCESS;
}

avl_tree_node*
AVLTreeDeleteImplementation (avl_tree*      const tree,
                             avl_tree_node* const node)
{
    if (tree == NULL ||
        node == NULL)
        return NULL;

    avl_tree_node* replace_node = NULL;

    if (node->left == NULL || node->right == NULL)
    {
        if (node->left == NULL) replace_node = node->right;
        else replace_node = node->left;
        
    }

    else
    {
        replace_node = AVLTreeFindMaxInBranch (node->left);
        node->left   = AVLTreeUnlinkMax       (node->left);

        replace_node->left  = node->left;
        replace_node->right = node->right;
    }
    
    AVLTreeNodeDestructor (node);
    return AVLTreeFixBalance (replace_node);
}

avl_tree_node*
AVLTreeUnlinkMax (avl_tree_node* const node)
{
    if (node->right == NULL) return node;
    node->right = AVLTreeUnlinkMax (node->right);

    return AVLTreeFixBalance (node);
}

avl_tree_node*
AVLTreeFindKey (avl_tree*           const tree,
                const avl_tree_key* const key)
{
    if (tree          == NULL ||
        key           == NULL ||
        key->data     == NULL ||
        key->key_size == 0)
        return NULL;

    int cmp_status = AVL_TREE_CMP_EQUAL;
    avl_tree_node* cur_node = tree->head;

    while (cur_node != NULL)
    {
        cmp_status = tree->compare (key, cur_node->key);

        if (cmp_status == AVL_TREE_CMP_LESS)
            cur_node = cur_node->left;

        else if (cmp_status == AVL_TREE_CMP_GREATER)
            cur_node = cur_node->right;

        else return cur_node;
    }

    return cur_node;
}

avl_tree_node*
AVLTreeFindMinInBranch (avl_tree_node* const node)
{
    if (node == NULL)
        return NULL;

    avl_tree_node* ret_node = node;

    while (ret_node->left != NULL)
        ret_node = ret_node->left;

    return ret_node;
}

avl_tree_node*
AVLTreeFindMaxInBranch (avl_tree_node* const node)
{
    if (node == NULL)
        return NULL;

    avl_tree_node* ret_node = node;

    while (ret_node->right != NULL)
        ret_node = ret_node->right;

    return ret_node;
}

avl_tree_node*
AVLTreeNodeConstructor (const avl_tree_key* const key,
                        avl_tree_node* const left,
                        avl_tree_node* const right)
{
    avl_tree_node* const node =
        (avl_tree_node*) malloc (sizeof (avl_tree_node));

    if (key == NULL) node->key = NULL;

    else
    {
        node->key = AVLTreeKeyConstructor (key->data, key->key_size);
        if (node->key == NULL)
            return AVLTreeNodeDestructor (node);
    }

    node->left  = left;
    node->right = right;

    node->branch_height = 0;

    return node;
}

avl_tree_node*
AVLTreeNodeDestructor (avl_tree_node* const node)
{
    if (node == NULL) return NULL;

    if (node->key) node->key = AVLTreeKeyDestructor (node->key);

    node->left  = NULL;
    node->right = NULL;

    free (node);
    return NULL;
}

avl_tree_key*
AVLTreeKeyConstructor (const void* const data,
                       const size_t key_size)
{
    if (data     == NULL ||
        key_size == 0)
        return NULL;

    avl_tree_key* const key = malloc (sizeof (avl_tree_key));
    if (key == NULL) return NULL;

    key->data = malloc (key_size);
    if (key->data == NULL)
        return AVLTreeKeyDestructor (key);

    memcpy (key->data, data, key_size);

    key->key_size = key_size;

    return key;
}

avl_tree_key*
AVLTreeKeyDestructor (avl_tree_key* const key)
{
    if (key == NULL) return NULL;

    if (key->data) free (key->data);

    free (key);
    return NULL;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// AVL tree balance functions' implementation
//-----------------------------------------------------------------------------
avl_tree_node*
AVLTreeFixBalance (avl_tree_node* const node)
{
    if (node == NULL) return NULL;
    AVLTreeFixHeight (node);

    if (AVLSubtreeCalculateBalance (node) == AVL_TREE_SMALL_RIGHT_ROTATE)
    {
        if (AVLSubtreeCalculateBalance (node->left) == AVL_TREE_BIG_RIGHT_ROTATE)
        {
            node->left = AVLTreeRotateLeft (node->left);
        }

        return AVLTreeRotateRight (node);
    }

    if (AVLSubtreeCalculateBalance (node) == AVL_TREE_SMALL_LEFT_ROTATE)
    {
        if (AVLSubtreeCalculateBalance (node->right) == AVL_TREE_BIG_LEFT_ROTATE)
        {
            node->right = AVLTreeRotateRight (node->right);
        }

        return AVLTreeRotateLeft (node);
    }

    return node;
}

void
AVLTreeFixHeight (avl_tree_node* const node)
{
    if (node == NULL) return;
    node->branch_height = MaxInt(AVLSubtreeHeight (node->left), 
                                 AVLSubtreeHeight (node->right)) + 1;
}

int
AVLSubtreeHeight (const avl_tree_node* const node)
{
    if (node == NULL) return 0;
    return node->branch_height;
}

int
AVLSubtreeCalculateBalance (const avl_tree_node* const node)
{
    if (node == NULL) return 0;
    return AVLSubtreeHeight (node->left) - AVLSubtreeHeight (node->right);
}

avl_tree_node*
AVLTreeRotateLeft (avl_tree_node* const node)
{
    if (node == NULL) return NULL;

    avl_tree_node* const replace = node->right;
    node->right = replace->left;
    replace->left = node;

    AVLTreeFixHeight (node);
    AVLTreeFixHeight (replace);

    return replace;
}

avl_tree_node*
AVLTreeRotateRight (avl_tree_node* const node)
{
    if (node == NULL) return NULL;

    avl_tree_node* const replace = node->left;
    node->left = replace->right;
    replace->right = node;

    AVLTreeFixHeight (node);
    AVLTreeFixHeight (replace);

    return replace;
}

int
MaxInt (const int a, const int b)
{
    return a > b ? a : b;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Main functions
//-----------------------------------------------------------------------------
const int MAX_INPUT_VALUE = 1000000000;
const int NOT_FOUND = -1;

enum operation
{
    OP_ADD  = '+',
    OP_NEXT = '?'
};

int int_cmp (const avl_tree_key* const a,
                  const avl_tree_key* const b)
{
    assert (a);
    assert (b);

    const int a_val = *(const int*) a->data;
    const int b_val = *(const int*) b->data;

    if (a_val < b_val) return AVL_TREE_CMP_LESS;
    if (a_val > b_val) return AVL_TREE_CMP_GREATER;

    return AVL_TREE_CMP_EQUAL;
}

const avl_tree_node*
AVLTreeFindNext (const avl_tree*     const tree,
                 const avl_tree_key* const key)
{
    assert (tree);

    const avl_tree_node* cur_node = tree->head;
    const avl_tree_node* parent   = NULL;

    while (cur_node != NULL)
    {
        parent = cur_node;

        if (tree->compare (key, cur_node->key) == AVL_TREE_CMP_LESS)
            cur_node = cur_node->left;

        else if (tree->compare (key, cur_node->key) == AVL_TREE_CMP_GREATER)
            cur_node = cur_node->right;

        else break;
    }

    if (parent == NULL || tree->compare (key, parent->key) == AVL_TREE_CMP_GREATER)
        return NULL;

    return parent;
}

int main (void)
{
    avl_tree* tree = AVLTreeConstructor (int_cmp);
    assert (tree);

    int input = 0;
    avl_tree_key* key = AVLTreeKeyConstructor (&input, sizeof (int));
    assert (key);

    size_t n = 0;
    assert (scanf ("%zd", &n) == 1);

    unsigned char operation = 0;
    int yoda_magic = 0;
    const avl_tree_node* find_node = NULL;

    for (size_t i = 0; i < n; ++i)
    {
        assert (scanf (" %c %d", &operation, &input) == 2);
        *(int*)key->data = input;

        if (operation == OP_ADD)
        {
            *(int*)key->data += yoda_magic;
            *(int*)key->data %= MAX_INPUT_VALUE;
            AVLTreeInsertKey (tree, key);
            yoda_magic = 0;
        }

        else if (operation == OP_NEXT)
        {
            find_node = AVLTreeFindNext (tree, key);
            if (find_node == NULL)
            {
                printf ("%d\n", NOT_FOUND);
                yoda_magic = NOT_FOUND;
            }

            else
            {
                yoda_magic = *(unsigned*)find_node->key->data;
                printf ("%u\n", yoda_magic);
            }
        }

        else assert (0);
    }

    tree = AVLTreeDestructor (tree);
    key  = AVLTreeKeyDestructor (key);
    return 0;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
