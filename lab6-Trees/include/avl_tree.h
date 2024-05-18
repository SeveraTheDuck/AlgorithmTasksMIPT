#include <stdlib.h>
#include <string.h>



//-----------------------------------------------------------------------------
// Constants for rotations
//-----------------------------------------------------------------------------
enum avl_tree_rotates
{
    AVL_TREE_SMALL_LEFT_ROTATE  = -2,
    AVL_TREE_BIG_RIGHT_ROTATE   = -1,
    AVL_TREE_BIG_LEFT_ROTATE    =  1,
    AVL_TREE_SMALL_RIGHT_ROTATE =  2
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// AVL-tree structs and types
//-----------------------------------------------------------------------------
typedef struct avl_tree_key
{
    void*  data;
    size_t key_size;
}
avl_tree_key;

typedef struct avl_tree_value
{
    void*  data;
    size_t value_size;
}
avl_tree_value;

typedef struct avl_tree_node
{
    avl_tree_key*         key;
    avl_tree_value*       value;

    struct avl_tree_node* left;
    struct avl_tree_node* right;

    int branch_height;
}
avl_tree_node;

typedef struct avl_tree
{
    avl_tree_node* root;
    int (*key_cmp) (const avl_tree_key* const,
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
//-------------------------------------
// Constructor and destructor
avl_tree*
AVLTreeConstructor (int (*key_cmp) (const avl_tree_key* const,
                                    const avl_tree_key* const));

avl_tree*
AVLTreeDestructor  (avl_tree* const tree);
//-------------------------------------

//-------------------------------------
// Interface
avl_tree_error_t
AVLTreeInsert  (avl_tree*             const tree,
                const avl_tree_key*   const key,
                const avl_tree_value* const value);

avl_tree_error_t
AVLTreeDelete  (avl_tree*           const tree,
                const avl_tree_key* const key);

avl_tree_node*
AVLTreeFindKey (avl_tree*           const tree,
                const avl_tree_key* const key);
//-------------------------------------

//-------------------------------------
// Node, key and value constructors and destructors
avl_tree_node*
AVLTreeNodeConstructor  (const avl_tree_key*   const key,
                         const avl_tree_value* const value);

avl_tree_node*
AVLTreeNodeDestructor   (avl_tree_node* const node);

avl_tree_key*
AVLTreeKeyConstructor   (const void* const data,
                         const size_t key_size);

avl_tree_key*
AVLTreeKeyDestructor    (avl_tree_key* const key);

avl_tree_value*
AVLTreeValueConstructor (const void* const data,
                         const size_t value_size);

avl_tree_value*
AVLTreeValueDestructor  (avl_tree_value* const value);
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
