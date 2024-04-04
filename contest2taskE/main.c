#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

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
    avl_tree_node* left;
    avl_tree_node* right;
    size_t branch_height;
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

avl_tree_error_t
AVLTreeInsertKey (avl_tree*           const tree,
                  const avl_tree_key* const key);

avl_tree_error_t
AVLTreeDeleteKey (avl_tree*           const tree,
                  const avl_tree_key* const key);

avl_tree_error_t
AVLTreeDeleteBranch (avl_tree*      const tree,
                     avl_tree_node* const node);

avl_tree_node*
AVLTreeFindKey (const avl_tree*     const tree,
                const avl_tree_key* const key);

avl_tree_node*
AVLTreeFindInBranch (const avl_tree*      const tree,
                     const avl_tree_node* const node,
                     const avl_tree_key*  const key);

avl_tree_node*
AVLTreeNodeConstructor (const avl_tree_key* const key,
                        avl_tree_node* const left,
                        avl_tree_node* const right);

avl_tree_node*
AVLTreeNodeDestructor (avl_tree_node* const node);

size_t
MaxBranchHeight (const avl_tree_node* const first,
                 const avl_tree_node* const second);
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

    AVLTreeDeleteBranch (tree, tree->head);
    tree->head = NULL;

    free (tree);
    return NULL;
}

avl_tree_error_t
AVLTreeDeleteBranch (avl_tree*      const tree,
                     avl_tree_node* const node)
{
    if (tree == NULL) return AVL_TREE_ERROR;

    if (node == NULL) return AVL_TREE_SUCCESS;

    if (AVLTreeDeleteBranch (tree, node->left)  == AVL_TREE_ERROR)
        return AVL_TREE_ERROR;

    if (AVLTreeDeleteBranch (tree, node->right) == AVL_TREE_ERROR)
        return AVL_TREE_ERROR;

    AVLTreeNodeDestructor (node);
    tree->elem_num--;

    return AVL_TREE_SUCCESS;
}

avl_tree_node*
AVLTreeFindKey (const avl_tree*     const tree,
                const avl_tree_key* const key)
{
    if (tree          == NULL ||
        key           == NULL ||
        key->data     == NULL ||
        key->key_size == 0)
        return NULL;

    return AVLTreeFindInBranch (tree, tree->head, key);
}

avl_tree_node*
AVLTreeFindInBranch (const avl_tree*      const tree,
                     const avl_tree_node* const node,
                     const avl_tree_key*  const key)
{
    if (tree      == NULL ||
        node      == NULL ||
        key       == NULL ||
        key->data == NULL ||
        key->key_size == 0)
        return NULL;

    if (tree->compare (key, node->key) == AVL_TREE_CMP_EQUAL)
        return node;

    if (tree->compare (key, node->key) == AVL_TREE_CMP_LESS)
        return AVLTreeFindInBranch (tree, node->left, key);

    return AVLTreeFindInBranch (tree, node->right, key);
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
        node->key = AVLKeyConstructor (key->data, key->key_size);
        if (node->key == NULL)
            return AVLTreeNodeDestructor (node);
    }

    node->left  = left;
    node->right = right;
    node->branch_height = MaxBranchHeight (left, right);
    
    return node;
}

avl_tree_node*
AVLTreeNodeDestructor (avl_tree_node* const node)
{
    if (node == NULL) return NULL;

    if (node->key) node->key = AVLKeyDestructor (node->key);
    
    node->left  = NULL;
    node->right = NULL;

    free (node);
    return NULL;
}

size_t
MaxBranchHeight (const avl_tree_node* const first,
                 const avl_tree_node* const second)
{
    size_t answer = 0;

    if (first) answer = first->branch_height;
    
    if (second && second->branch_height > answer)
        answer = second->branch_height;

    return answer;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
