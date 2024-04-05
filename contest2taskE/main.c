#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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
    struct avl_tree_node* left;
    struct avl_tree_node* right;
    struct avl_tree_node* parent;
    int height_diff;
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
AVLTreeInsertKeySetParent (avl_tree*      const tree,
                           avl_tree_node* const node);

avl_tree_error_t
AVLTreeDeleteKey (avl_tree*           const tree,
                  const avl_tree_key* const key);

avl_tree_error_t
AVLTreeReplaceDeletedNode (avl_tree*            const tree,
                           const avl_tree_node* const del_node);

avl_tree_error_t
AVLTreeDeleteBranch (avl_tree*      const tree,
                     avl_tree_node* const node);

avl_tree_node*
AVLTreeFindKey (const avl_tree*     const tree,
                const avl_tree_key* const key);

avl_tree_node*
AVLTreeFindInBranch (const avl_tree* const tree,
                     avl_tree_node*  const node,
                     const avl_tree_key* const key);

avl_tree_node*
AVLTreeFindMinInBranch (avl_tree_node* const node);

avl_tree_node*
AVLTreeFindMaxInBranch (avl_tree_node* const node);

avl_tree_node*
AVLTreeNodeConstructor (const avl_tree_key* const key,
                        avl_tree_node* const left,
                        avl_tree_node* const right,
                        avl_tree_node* const parent);

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
AVLTreeInsertKey (avl_tree*           const tree,
                  const avl_tree_key* const key)
{
    if (tree          == NULL ||
        key           == NULL ||
        key->data     == NULL ||
        key->key_size == 0)
        return AVL_TREE_ERROR;

    avl_tree_node* const new_node =
        AVLTreeNodeConstructor (key, NULL, NULL, NULL);
    if (new_node == NULL) return AVL_TREE_ERROR;

    if (AVLTreeInsertKeySetParent (tree, new_node) == AVL_TREE_ERROR)
    {
        AVLTreeNodeDestructor (new_node);
        return AVL_TREE_ERROR;
    }

    tree->elem_num++;

    if (AVLTreeInsertKeyFixHeights (new_node) == AVL_TREE_ERROR)
        return AVL_TREE_ERROR;

    return AVL_TREE_SUCCESS;
}

avl_tree_error_t
AVLTreeInsertKeySetParent (avl_tree*      const tree,
                           avl_tree_node* const node)
{
    if (tree == NULL ||
        node == NULL)
        return AVL_TREE_ERROR;

    avl_tree_node* parent       = tree->head;
    avl_tree_node* insert_place = tree->head;

    while (insert_place != NULL)
    {
        parent = insert_place;

        if (tree->compare (node->key, insert_place->key) == AVL_TREE_CMP_GREATER)
             insert_place = insert_place->right;
        else insert_place = insert_place->left;
    }

    node->parent = parent;

    if (parent == NULL) tree->head = node;

    else if (tree->compare (node->key, parent->key) == AVL_TREE_CMP_GREATER)
         parent->right = node;

    else parent->left = node;

    return AVL_TREE_SUCCESS;
}

avl_tree_error_t
AVLTreeInsertKeyFixHeights (avl_tree_node* const node)
{
    if (node == NULL) return AVL_TREE_ERROR;

    avl_tree_node* parent = node->parent;

    while (parent != NULL)
    {
        if (parent->left == node) parent->height_diff++;
        else parent->height_diff--;

        if (parent->height_diff == 0) break;

        if (parent->height_diff ==  1 ||
            parent->height_diff == -1)
            parent = parent->parent;

        else parent = AVLTreeRotateSubtree (parent);
    }

    return AVL_TREE_SUCCESS;
}

avl_tree_error_t
AVLTreeDeleteKey (avl_tree*           const tree,
                  const avl_tree_key* const key)
{
    if (tree          == NULL ||
        key           == NULL ||
        key->data     == NULL ||
        key->key_size == 0)
        return AVL_TREE_ERROR;

    avl_tree_node* const del_node = AVLTreeFindKey (tree, key);
    if (del_node == NULL) return AVL_TREE_ERROR;

    if (AVLTreeReplaceDeletedNode (tree, del_node) == AVL_TREE_ERROR)
        return AVL_TREE_ERROR;

    AVLTreeNodeDestructor (del_node);
    tree->elem_num--;

    // fix heights

    return AVL_TREE_SUCCESS;
}

avl_tree_error_t
AVLTreeReplaceDeletedNode (avl_tree*            const tree,
                           const avl_tree_node* const del_node)
{
    if (tree     == NULL ||
        del_node == NULL)
        return AVL_TREE_ERROR;

    avl_tree_node* replace_node =
        AVLTreeFindMaxInBranch (del_node->left);
    if (replace_node) replace_node->parent->left = NULL;

    if (replace_node == NULL)
    {
        replace_node = AVLTreeFindMinInBranch (del_node->right);
        if (replace_node) replace_node->parent->right = NULL;
    }

    if (del_node->parent == NULL)
        tree->head = replace_node;

    else if (del_node->parent->left == del_node)
        del_node->parent->left = replace_node;

    else del_node->parent->right = replace_node;

    if (replace_node)
    {
        replace_node->parent      = del_node->parent;
        replace_node->left        = del_node->left;
        replace_node->right       = del_node->right;
        replace_node->height_diff = del_node->height_diff; //???
    }

    return AVL_TREE_SUCCESS;
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

    // fix heights

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
AVLTreeFindInBranch (const avl_tree* const tree,
                     avl_tree_node*  const begin_node,
                     const avl_tree_key* const key)
{
    if (tree          == NULL ||
        key           == NULL ||
        key->data     == NULL ||
        key->key_size == 0)
        return NULL;

    int cmp_status = AVL_TREE_CMP_EQUAL;

    avl_tree_node* cur_node = begin_node;

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
                        avl_tree_node* const right,
                        avl_tree_node* const parent)
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

    node->left   = left;
    node->right  = right;
    node->parent = parent;
    node->height_diff = 0;

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

    key->data = calloc (key_size, sizeof (char));
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

avl_tree_node*
AVLTreeRotateSubtree (avl_tree_node* const node)
{
    if (node == NULL) return NULL;

    if (node)
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int size_t_cmp (const avl_tree_key* const a,
                const avl_tree_key* const b)
{
    const size_t a_val = *(const size_t*) a->data;
    const size_t b_val = *(const size_t*) b->data;

    if (a_val < b_val) return -1;
    if (a_val > b_val) return 1;

    return 0;
}

int main (void)
{
    avl_tree* tree = AVLTreeConstructor (size_t_cmp);

    size_t temp_in = 0;

    avl_tree_key* key = AVLTreeKeyConstructor (&temp_in, sizeof (size_t));

    for (size_t i = 0; i < 10; i++)
    {
        *(size_t*)key->data = i;
        AVLTreeInsertKey (tree, key);
    }

    avl_tree_node* node = tree->head;

    *(size_t*)key->data = 2;
    AVLTreeInsertKey (tree, key);

    for (size_t i = 0; i < 10; i++)
    {
        fprintf (stderr, "node %zu: %zu, left %p right %p\n", i, *(size_t*)node->key->data, node->left, node->right);
        if (node->left) node = node->left;
        else node = node->right;
    }

    tree = AVLTreeDestructor (tree);
    return 0;
}
