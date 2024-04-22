#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


//------------------------------------------------//
//                  IMPORTANT                     //
// This is not regular treap, but implicit treap. //
//------------------------------------------------//



//-----------------------------------------------------------------------------
// Structs and types
//-----------------------------------------------------------------------------
typedef struct treap_value
{
    void*  value;
    size_t size;  ///< size in bytes
}
treap_value;

typedef struct treap_node
{
    int                priority;
    size_t             subtree_size;
    treap_value*       value;
    struct treap_node* parent;
    struct treap_node* left;
    struct treap_node* right;
}
treap_node;

typedef treap_node treap; ///< as a whole tree, used as a pointer to root

typedef struct treap_pair
{
    treap* tree1;
    treap* tree2;
}
treap_pair;

enum treap_errors
{
    TREAP_SUCCESS = 0,
    TREAP_ERROR   = 1
};

typedef size_t treap_error_t;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Treap functions' prototypes
//-----------------------------------------------------------------------------
//-------------------------------------
// Treap destructor
treap*
TreapDestructor     (treap* const tree);

treap_node*
TreapDestructorImpl (treap_node* const node);
//-------------------------------------

//-------------------------------------
// Treap node constructor and destructor
treap_node*
TreapNodeConstructor (const treap_value* const value);

treap_node*
TreapNodeDestructor  (treap_node* const node);
//-------------------------------------

//-------------------------------------
// Treap value constructor, destructor and copy
treap_value*
TreapValueConstructor (const void* const value,
                       const size_t size);

treap_value*
TreapValueDestructor  (treap_value* const value);

treap_value*
TreapValueCopy        (const treap_value* const value);
//-------------------------------------

//-------------------------------------
// Treap split and merge
// cut_number is number of nodes to leave in pair->tree1
treap_pair*
TreapSplit             (treap* const tree,
                        const size_t cut_number);

treap*
TreapMerge             (treap* const tree1,
                        treap* const tree2);

treap_pair*
TreapPairConstructor   (treap* const tree1,
                        treap* const tree2);

treap_pair*
TreapPairDestructor    (treap_pair* const pair);

void
TreapUpdateSubtreeSize (treap* const tree);

size_t
TreapSize              (const treap* const tree);
//-------------------------------------

//-------------------------------------
// Treap find, insert and delete
treap_node*
TreapFind   (const treap*       const tree,
             const treap_value* const value);

treap_error_t
TreapInsert (treap*             const tree,
             const treap_value* const value);

treap_error_t
TreapDelete (treap*             const tree,
             const treap_value* const value);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Treap functions' implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Treap destructor
treap*
TreapDestructor (treap* const tree)
{
    return TreapDestructorImpl (tree);
}

treap_node*
TreapDestructorImpl (treap_node* const node)
{
    if (node == NULL) return NULL;

    TreapDestructorImpl (node->left);
    TreapDestructorImpl (node->right);

    return TreapNodeDestructor (node);
}
//-------------------------------------

//-------------------------------------
// Treap node constructor and destructor
treap_node*
TreapNodeConstructor (const treap_value* const value)
{
    treap_node* const node =
        (treap_node*) calloc (1, sizeof (treap_node));
    if (node == NULL) return NULL;

    node->priority = rand ();
    node->value    = TreapValueCopy (value);

    return node;
}

treap_node*
TreapNodeDestructor (treap_node* const node)
{
    if (node == NULL) return NULL;

    node->value = TreapValueDestructor (node->value);

    free (node);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Treap value constructor, destructor and copy
treap_value*
TreapValueConstructor (const void* const value,
                       const size_t size)
{
    treap_value* const new_value =
        (treap_value*) calloc (1, sizeof (treap_value));
    if (new_value == NULL) return NULL;

    if (value != NULL && size != 0)
    {
        new_value->value = malloc (size);
        if (new_value->value == NULL)
            return TreapValueDestructor (new_value);

        memcpy (new_value->value, value, size);
        new_value->size = size;
    }

    return new_value;
}

treap_value*
TreapValueDestructor (treap_value* const value)
{
    if (value == NULL) return NULL;

    free (value->value);
    free (value);
    return NULL;
}

treap_value*
TreapValueCopy (const treap_value* const value)
{
    if (value == NULL) return NULL;
    return TreapValueConstructor (value->value, value->size);
}
//-------------------------------------

//-------------------------------------
// Treap split and merge
treap_pair*
TreapSplit (treap* const tree,
            const size_t cut_number)
{
    if (tree == NULL)
        return TreapPairConstructor (NULL, NULL);

    const size_t left_subtree_size = TreapSize (tree->left);
    treap_pair*  pair = NULL;

    if (left_subtree_size >= cut_number)
    {
        pair = TreapSplit (tree->left, cut_number);
        tree->left = pair->tree2;

        TreapUpdateSubtreeSize (tree);
        pair->tree2 = tree;

        return pair;
    }

    else
    {
        pair = TreapSplit (tree->right, cut_number - left_subtree_size - 1);
        tree->right = pair->tree1;

        TreapUpdateSubtreeSize (tree);
        pair->tree1 = tree;

        return pair;
    }
}

treap*
TreapMerge (treap* const tree1,
            treap* const tree2)
{
    if (tree1 == NULL) return tree2;
    if (tree2 == NULL) return tree1;

    if (tree1->priority > tree2->priority)
    {
        tree1->right = TreapMerge (tree1->right, tree2);
        TreapUpdateSubtreeSize (tree1);
        return tree1;
    }

    else
    {
        tree2->left = TreapMerge (tree1, tree2->left);
        TreapUpdateSubtreeSize (tree2);
        return tree2;
    }
}

treap_pair*
TreapPairConstructor (treap* const tree1,
                      treap* const tree2)
{
    treap_pair* const pair =
        (treap_pair*) malloc (sizeof (treap_pair));
    if (pair == NULL) return NULL;

    pair->tree1 = tree1;
    pair->tree2 = tree2;

    return pair;
}

treap_pair*
TreapPairDestructor (treap_pair* const pair)
{
    free (pair);
    return NULL;
}

void
TreapUpdateSubtreeSize (treap* const tree)
{
    if (tree == NULL) return;
    tree->subtree_size = 1 + TreapSize (tree->left) + TreapSize (tree->right);
}

size_t
TreapSize (const treap* const tree)
{
    if (tree == NULL) return 0;
    return tree->subtree_size;
}
//-------------------------------------

//-------------------------------------
//
