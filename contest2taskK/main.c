#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>



//-----------------------------------------------------------------------------
// Structs and types
//-----------------------------------------------------------------------------
typedef struct treap_node
{
    int priority;
    int value;

    size_t subtree_size;
    int    subtree_sum;

    struct treap_node* left;
    struct treap_node* right;
}
treap_node;

typedef struct treap
{
    treap_node* root;
}
treap;

typedef struct treap_pair
{
    treap_node* tree1;
    treap_node* tree2;
}
treap_pair;

typedef size_t treap_error_t;

enum treap_errors
{
    TREAP_SUCCESS = 0,
    TREAP_ERROR   = 1
};

enum treap_cmp_status
{
    TREAP_LESS    = -1,
    TREAP_EQUAL   =  0,
    TREAP_GREATER =  1
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Treap functions' prototypes
//-----------------------------------------------------------------------------
//-------------------------------------
// Treap constructor and destructor
treap*
TreapConstructor    (void);

treap*
TreapDestructor     (treap* const tree);

treap_node*
TreapDestructorImpl (treap_node* const node);
//-------------------------------------

//-------------------------------------
// Treap node constructor and destructor
treap_node*
TreapNodeConstructor (const int value);

treap_node*
TreapNodeDestructor  (treap_node* const node);
//-------------------------------------

//-------------------------------------
// Treap split and merge
// cut_number is number of nodes to leave in pair->tree1
treap_pair*
TreapSplit             (treap* const tree,
                        const size_t split_index);

treap_pair*
TreapSplitImpl         (treap_node* const tree,
                        const size_t split_index);

treap_node*
TreapMerge             (treap_node* const tree1,
                        treap_node* const tree2);

treap_pair*
TreapPairConstructor   (treap_node* const tree1,
                        treap_node* const tree2);

treap_pair*
TreapPairDestructor    (treap_pair* const pair);
//-------------------------------------

//-------------------------------------
// Update recursive parameters
void
TreapUpdateRecursiveParams (treap_node* const node);

void
TreapUpdateSubtreeSum      (treap_node* const node);

void
TreapUpdateSubtreeSize     (treap_node* const node);

int
TreapValue                 (const treap_node* const node);

int
TreapSum                   (const treap_node* const node);

size_t
TreapSize                  (const treap_node* const node);
//-------------------------------------

//-------------------------------------
// Treap find, insert and delete
treap_node*
TreapFind            (const treap* const tree,
                      const size_t find_index);

treap_node*
TreapFindImpl        (treap_node* const tree,
                      const size_t find_index);

treap_error_t
TreapInsert          (treap* const tree,
                      const size_t insert_index,
                      const int    value);

treap_error_t
TreapDelete          (treap* const tree,
                      const size_t delete_index);

treap_error_t
TreapDeleteFoundNode (treap_pair* const pair,
                      treap_node* const found_node);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Treap functions' implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Treap destructor
treap*
TreapConstructor (void)
{
    treap* const tree = (treap*) malloc (sizeof(treap));
    if (tree == NULL) return NULL;

    tree->root = NULL;

    return tree;
}

treap*
TreapDestructor (treap* const tree)
{
    if (tree == NULL) return NULL;

    tree->root = TreapDestructorImpl (tree->root);

    free (tree);
    return NULL;
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
TreapNodeConstructor (const int value)
{
    treap_node* const node =
        (treap_node*) calloc (1, sizeof (treap_node));
    if (node == NULL) return NULL;

    node->priority     = rand ();
    node->value        = value;
    node->subtree_size = 1;

    return node;
}

treap_node*
TreapNodeDestructor (treap_node* const node)
{
    free (node);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Treap split and merge
treap_pair*
TreapSplit (treap* const tree,
            const size_t split_index)
{
    if (tree == NULL) return TreapPairConstructor (NULL, NULL);

    return TreapSplitImpl (tree->root, split_index);
}

treap_pair*
TreapSplitImpl (treap_node* const tree,
                const size_t split_index)
{
    if (tree == NULL)
        return TreapPairConstructor (NULL, NULL);

    treap_pair* pair = NULL;

    const size_t left_subtree_size = TreapSize (tree->left);
    size_t new_split_index = split_index;

    if (left_subtree_size < split_index)
    {
        new_split_index -= left_subtree_size + 1;
        pair = TreapSplitImpl (tree->right, new_split_index);
        tree->right = pair->tree1;

        TreapUpdateRecursiveParams (tree);
        pair->tree1 = tree;

        return pair;
    }

    else
    {
        pair = TreapSplitImpl (tree->left, new_split_index);
        tree->left = pair->tree2;

        TreapUpdateRecursiveParams (tree);
        pair->tree2 = tree;

        return pair;
    }
}

treap_node*
TreapMerge (treap_node* const tree1,
            treap_node* const tree2)
{
    if (tree1 == NULL) return tree2;
    if (tree2 == NULL) return tree1;

    if (tree1->priority <= tree2->priority)
    {
        tree2->left = TreapMerge (tree1, tree2->left);
        TreapUpdateRecursiveParams (tree2);
        return tree2;
    }

    else
    {
        tree1->right = TreapMerge (tree1->right, tree2);
        TreapUpdateRecursiveParams (tree1);
        return tree1;
    }
}

treap_pair*
TreapPairConstructor (treap_node* const tree1,
                      treap_node* const tree2)
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
//-------------------------------------

//-------------------------------------
// Update recursive parameters
void
TreapUpdateRecursiveParams (treap_node* const node)
{
    if (node == NULL) return;

    TreapUpdateSubtreeSum  (node);
    TreapUpdateSubtreeSize (node);
}

void
TreapUpdateSubtreeSum  (treap_node* const node)
{
    node->subtree_sum = node->value +
                        TreapSum (node->left) +
                        TreapSum (node->right);
}

void
TreapUpdateSubtreeSize (treap_node* const node)
{
    node->subtree_size = 1 + TreapSize (node->left) +
                             TreapSize (node->right);
}

int
TreapValue (const treap_node* const node)
{
    if (node == NULL) return 0;
    return node->value;
}

int
TreapSum (const treap_node* const node)
{
    if (node == NULL) return 0;
    return node->subtree_sum;
}

size_t
TreapSize (const treap_node* const node)
{
    if (node == NULL) return 0;
    return node->subtree_size;
}
//-------------------------------------

//-------------------------------------
// Treap find, insert and delete
treap_node*
TreapFind (const treap* const tree,
           const size_t find_index)
{
    if (tree == NULL) return NULL;

    return TreapFindImpl (tree->root, find_index);
}

treap_node*
TreapFindImpl (treap_node* const tree,
               const size_t find_index)
{
    if (tree == NULL) return NULL;

    const size_t left_subtree_size = TreapSize (tree->left);
    size_t new_find_index = find_index;

    if (left_subtree_size == find_index)
        return tree;

    if (left_subtree_size < find_index)
    {
        new_find_index -= left_subtree_size + 1;
        return TreapFindImpl (tree->right, new_find_index);
    }

    else
        return TreapFindImpl (tree->left, new_find_index);
}

treap_error_t
TreapInsert (treap* const tree,
             const size_t insert_index,
             const int    value)
{
    if (tree  == NULL) return TREAP_ERROR;

    treap_pair* const split_pair = TreapSplit (tree, insert_index);
    if (split_pair == NULL) return TREAP_ERROR;

    treap_node* const new_node = TreapNodeConstructor (value);
    if (new_node == NULL)
    {
        TreapPairDestructor (split_pair);
        return TREAP_ERROR;
    }

    tree->root = TreapMerge (TreapMerge (split_pair->tree1, new_node),
                             split_pair->tree2);

    TreapPairDestructor (split_pair);

    return TREAP_SUCCESS;
}

treap_error_t
TreapDelete (treap* const tree,
             const size_t delete_index)
{
    if (tree == NULL) return TREAP_ERROR;

    treap_node* const found_node = TreapFind (tree, delete_index);
    if (found_node == NULL)
        return TREAP_SUCCESS;

    treap_pair* const split_pair = TreapSplit (tree, delete_index);
    if (split_pair == NULL) return TREAP_ERROR;

    TreapDeleteFoundNode (split_pair, found_node);

    tree->root = TreapMerge (split_pair->tree1, split_pair->tree2);

    TreapPairDestructor (split_pair);

    return TREAP_SUCCESS;
}

treap_error_t
TreapDeleteFoundNode (treap_pair* const pair,
                      treap_node* const found_node)
{
    if (pair->tree2 == found_node)
    {
        pair->tree2 = found_node->right;
        TreapNodeDestructor (found_node);
        return TREAP_SUCCESS;
    }

    treap_node* cur  = pair->tree2;
    treap_node* next = cur->left;

    while (next != found_node)
    {
        --cur->subtree_size;
        cur  = next;
        next = next->left;
    }

    cur->left = found_node->right;
    --cur->subtree_size;
    TreapNodeDestructor (found_node);

    return TREAP_SUCCESS;
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Main functions
//-----------------------------------------------------------------------------
//-------------------------------------
// Constants and types
enum operation
{
    SUM              = 1,
    INSERT           = 2,
    DELETE           = 3,
    SET_VALUE        = 4,
    ADD_VALUE        = 5,
    NEXT_PERMUTATION = 6,
    PREV_PERMUTATION = 7
};

typedef unsigned int operation_t;
//-------------------------------------

//-------------------------------------
// Help functions
void
PrintTreapInOrder (const treap_node* const node)
{
    if (node == NULL)
    {
        printf ("nil ");
        return;
    }

    printf ("(");
    PrintTreapInOrder (node->left);
    printf ("%d ", node->value);
    PrintTreapInOrder (node->right);
    printf (")");
}

void
SumOperation (treap* const tree,
              const size_t left_index,
              const size_t right_index)
{
    assert (tree);

    treap_pair* first_pair =
        TreapSplit (tree, left_index);
    assert (first_pair);

    treap_pair* second_pair =
        TreapSplitImpl (first_pair->tree2, right_index - left_index + 1);
    assert (second_pair);

    printf ("%d\n", TreapSum (second_pair->tree1));

    tree->root = TreapMerge (first_pair->tree1,
                 TreapMerge (second_pair->tree1, second_pair->tree2));

    first_pair  = TreapPairDestructor (first_pair);
    second_pair = TreapPairDestructor (second_pair);
}

void
InsertOperation (treap* const tree)
{
    assert (tree);

    int    value        = 0;
    size_t insert_index = 0;

    assert (scanf ("%d %zd", &value, &insert_index) == 2);
    assert (TreapInsert (tree, insert_index, value) == TREAP_SUCCESS);
}

void
DeleteOperation (treap* const tree)
{
    assert (tree);

    size_t delete_index = 0;

    assert (scanf ("%zd", &delete_index) == 1);
    assert (TreapDelete (tree, delete_index) == TREAP_SUCCESS);
}

void
SetValue (treap* const tree)
{
    assert (tree);

    size_t left_index  = 0;
    size_t right_index = 0;
    int    value       = 0;

    assert (scanf ("%d %zd %zd", &value, &left_index, &right_index) == 3);

    treap_node* const found_node = TreapFind (tree, set_index);
    if (found_node == NULL) return;

    found_node->value = value;
}
//-------------------------------------

//-------------------------------------
// Requests execution function
void
FillTreap (treap* const tree,
           const size_t elem_number)
{
    assert (tree);

    int input = 0;

    for (size_t i = 0; i < elem_number; ++i)
    {
        assert (scanf ("%d", &input) == 1);
        assert (TreapInsert (tree, i, input) == TREAP_SUCCESS);
    }

    PrintTreapInOrder (tree->root);
    printf ("\n");

//     input = 10;
//     TreapInsert (tree, 3, input);
//
//     PrintTreapInOrder (tree->root);
//     printf ("\n");
}

void
ExecuteRequests (treap* const tree,
                 const size_t requests_count)
{
    operation_t operation = 0;

    for (size_t i = 0; i < requests_count; ++i)
    {
        assert (scanf ("%u", &operation) == 1);

        switch (operation)
        {
            case SUM:
                SumOperation (tree);
                break;

            case INSERT:
                InsertOperation (tree);
                break;

            case DELETE:
                DeleteOperation (tree);
                break;

            case SET_VALUE:
                SetValueOperation (tree);
                break;

            case ADD_VALUE:
                AddValueOperation (tree);
                break;

            case NEXT_PERMUTATION:
                NextPermutationOperation (tree);
                break;

            case PREV_PERMUTATION:
                PrevPermutationOperation (tree);
                break;

            default:
                assert (0);
        }
    }
}

int main (void)
{
    srand (time (NULL));

    treap* tree = TreapConstructor ();
    assert (tree);

    size_t n = 0;
    assert (scanf ("%zd", &n) == 1);
    FillTreap (tree, n);

    // SumOperation (tree, 0, 7);

    size_t q = 0;
    assert (scanf ("%zd", &q) == 1);
    ExecuteRequests (tree, q);

    tree = TreapDestructor (tree);
    return 0;
}
//-------------------------------------
