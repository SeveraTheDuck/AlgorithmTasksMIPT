#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>



//-----------------------------------------------------------------------------
// Structs and types
//-----------------------------------------------------------------------------
typedef struct treap_key
{
    void*  key;
    size_t size;  ///< size in bytes
}
treap_key;

typedef struct treap_value
{
    void*  value;
    size_t size;  ///< size in bytes
}
treap_value;

typedef struct treap_node
{
    int                priority;
    treap_key*         key;
    treap_value*       value;

    size_t             subtree_size;

    struct treap_node* left;
    struct treap_node* right;
}
treap_node;

typedef struct treap
{
    treap_node* root;
    int (*key_cmp) (const treap_key* const,
                    const treap_key* const);
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
TreapConstructor    (int (*key_cmp) (const treap_key* const,
                                     const treap_key* const));

treap*
TreapDestructor     (treap* const tree);

treap_node*
TreapDestructorImpl (treap_node* const node);
//-------------------------------------

//-------------------------------------
// Treap node constructor and destructor
treap_node*
TreapNodeConstructor (const treap_key*   const key,
                      const treap_value* const value);

treap_node*
TreapNodeDestructor  (treap_node* const node);
//-------------------------------------

//-------------------------------------
// Treap key and value constructor, destructor and copy
treap_key*
TreapKeyConstructor   (const void* const key,
                       const size_t size);

treap_key*
TreapKeyDestructor    (treap_key* const key);

treap_key*
TreapKeyCopy          (const treap_key* const key);

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
TreapSplit             (treap*           const tree,
                        const treap_key* const key);

treap_pair*
TreapSplitImpl         (treap_node*      const tree,
                        const treap_key* const key,
                        int (*key_cmp)  (const treap_key* const,
                                         const treap_key* const));
treap_node*
TreapMerge             (treap_node* const tree1,
                        treap_node* const tree2);

treap_pair*
TreapPairConstructor   (treap_node* const tree1,
                        treap_node* const tree2);

treap_pair*
TreapPairDestructor    (treap_pair* const pair);

void
TreapUpdateSubtreeSize (treap_node* const tree);

size_t
TreapSize              (const treap_node* const tree);
//-------------------------------------

//-------------------------------------
// Treap find, insert and delete
treap_node*
TreapFind            (const treap*     const tree,
                      const treap_key* const key);

treap_node*
TreapFindImpl        (treap_node*      const tree,
                      const treap_key* const key,
                      int (*key_cmp)  (const treap_key* const,
                                       const treap_key* const));

treap_error_t
TreapInsert          (treap*             const tree,
                      const treap_key*   const key,
                      const treap_value* const value);

treap_error_t
TreapInsertImpl      (treap*             const tree,
                      const treap_key*   const key,
                      const treap_value* const value);

treap_error_t
TreapDelete          (treap*           const tree,
                      const treap_key* const key);

treap_error_t
TreapDeleteImpl      (treap*      const tree,
                      treap_node* const found_node);

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
TreapConstructor (int (*key_cmp) (const treap_key* const,
                                  const treap_key* const))
{
    if (key_cmp == NULL) return NULL;

    treap* const tree = (treap*) malloc (sizeof(treap));
    if (tree == NULL) return NULL;

    tree->root    = NULL;
    tree->key_cmp = key_cmp;

    return tree;
}

treap*
TreapDestructor (treap* const tree)
{
    if (tree == NULL) return NULL;

    tree->root    = TreapDestructorImpl (tree->root);
    tree->key_cmp = NULL;

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
TreapNodeConstructor (const treap_key*   const key,
                      const treap_value* const value)
{
    treap_node* const node =
        (treap_node*) calloc (1, sizeof (treap_node));
    if (node == NULL) return NULL;

    node->priority     = rand ();
    node->key          = TreapKeyCopy   (key);
    node->value        = TreapValueCopy (value);
    node->subtree_size = 1;

    return node;
}

treap_node*
TreapNodeDestructor (treap_node* const node)
{
    if (node == NULL) return NULL;

    node->key =   TreapKeyDestructor   (node->key);
    node->value = TreapValueDestructor (node->value);

    free (node);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Treap key and value constructor, destructor and copy
treap_key*
TreapKeyConstructor (const void* const key,
                     const size_t size)
{
    treap_key* const new_key =
        (treap_key*) calloc (1, sizeof (treap_key));
    if (new_key == NULL) return NULL;

    if (key != NULL && size != 0)
    {
        new_key->key = malloc (size);
        if (new_key->key == NULL)
            return TreapKeyDestructor (new_key);

        memcpy (new_key->key, key, size);
        new_key->size = size;
    }

    return new_key;
}

treap_key*
TreapKeyDestructor (treap_key* const key)
{
    if (key == NULL) return NULL;

    free (key->key);
    free (key);
    return NULL;
}

treap_key*
TreapKeyCopy (const treap_key* const key)
{
    if (key == NULL) return NULL;
    return TreapKeyConstructor (key->key, key->size);
}

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
TreapSplit (treap*           const tree,
            const treap_key* const key)
{
    if (tree == NULL) return TreapPairConstructor (NULL, NULL);

    return TreapSplitImpl (tree->root, key, tree->key_cmp);
}

treap_pair*
TreapSplitImpl (treap_node*      const tree,
                const treap_key* const key,
                int (*key_cmp)  (const treap_key* const,
                                 const treap_key* const))
{
    if (tree == NULL)
        return TreapPairConstructor (NULL, NULL);

    treap_pair* pair = NULL;

    if (key_cmp (key, tree->key) == TREAP_GREATER)
    {
        pair = TreapSplitImpl (tree->right, key, key_cmp);
        tree->right = pair->tree1;

        TreapUpdateSubtreeSize (tree);
        pair->tree1 = tree;

        return pair;
    }

    else
    {
        pair = TreapSplitImpl (tree->left, key, key_cmp);
        tree->left = pair->tree2;

        TreapUpdateSubtreeSize (tree);
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
        TreapUpdateSubtreeSize (tree2);
        return tree2;
    }

    else
    {
        tree1->right = TreapMerge (tree1->right, tree2);
        TreapUpdateSubtreeSize (tree1);
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

    pair->tree1   = tree1;
    pair->tree2   = tree2;

    return pair;
}

treap_pair*
TreapPairDestructor (treap_pair* const pair)
{
    free (pair);
    return NULL;
}

void
TreapUpdateSubtreeSize (treap_node* const tree)
{
    if (tree == NULL) return;
    tree->subtree_size = 1 + TreapSize (tree->left) + TreapSize (tree->right);
}

size_t
TreapSize (const treap_node* const tree)
{
    if (tree == NULL) return 0;
    return tree->subtree_size;
}
//-------------------------------------

//-------------------------------------
// Treap find, insert and delete
treap_node*
TreapFind (const treap*     const tree,
           const treap_key* const key)
{
    if (tree          == NULL ||
        tree->key_cmp == NULL ||
        key           == NULL)
        return NULL;

    return TreapFindImpl (tree->root, key, tree->key_cmp);
}

treap_node*
TreapFindImpl (treap_node*      const tree,
               const treap_key* const key,
               int (*key_cmp)  (const treap_key* const,
                                const treap_key* const))
{
    if (tree == NULL) return NULL;

    int cmp_status = key_cmp (key, tree->key);

    if (cmp_status == TREAP_EQUAL)
        return tree;

    if (cmp_status == TREAP_LESS)
        return TreapFindImpl (tree->left,  key, key_cmp);

    else
        return TreapFindImpl (tree->right, key, key_cmp);
}

treap_error_t
TreapInsert (treap*             const tree,
             const treap_key*   const key,
             const treap_value* const value)
{
    if (tree          == NULL ||
        tree->key_cmp == NULL ||
        key           == NULL ||
        value         == NULL)
        return TREAP_ERROR;

    treap_node* const found_node = TreapFind (tree, key);

    if (found_node != NULL)
        return TREAP_SUCCESS;

    return TreapInsertImpl (tree, key, value);
}

treap_error_t
TreapInsertImpl (treap*             const tree,
                 const treap_key*   const key,
                 const treap_value* const value)
{
    treap_pair* const split_pair = TreapSplit (tree, key);
    if (split_pair == NULL) return TREAP_ERROR;

    treap_node* const new_node = TreapNodeConstructor (key, value);
    if (new_node == NULL)
    {
        TreapPairDestructor (split_pair);
        return TREAP_ERROR;
    }

    treap_node* const new_tree =
        TreapMerge (TreapMerge (split_pair->tree1, new_node),
                    split_pair->tree2);

    tree->root = new_tree;
    TreapPairDestructor (split_pair);

    return TREAP_SUCCESS;
}

treap_error_t
TreapDelete (treap*           const tree,
             const treap_key* const key)
{
    if (tree          == NULL ||
        tree->key_cmp == NULL ||
        key           == NULL)
        return TREAP_ERROR;

    treap_node* const found_node = TreapFind (tree, key);

    if (found_node == NULL)
        return TREAP_SUCCESS;

    return TreapDeleteImpl (tree, found_node);
}

treap_error_t
TreapDeleteImpl (treap*      const tree,
                 treap_node* const found_node)
{
    treap_pair* const split_pair = TreapSplit (tree, found_node->key);
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
const size_t STRING_MAX_LEN = 7;
const char   INSERT_STR[] = "insert";
const char   DELETE_STR[] = "delete";
const char   EXISTS_STR[] = "exists";
const char   NEXT_STR  [] = "next";
const char   PREV_STR  [] = "prev";
const char   KTH_STR   [] = "kth";
const char   TRUE_STR  [] = "true";
const char   FALSE_STR [] = "false";
const char   NONE_STR  [] = "none";

enum operation
{
    INSERT = 0,
    DELETE = 1,
    EXISTS = 2,
    NEXT   = 3,
    PREV   = 4,
    KTH    = 5,
    ERROR  = 6
};

typedef unsigned char operation_t;
//-------------------------------------

//-------------------------------------
// Help functions
int key_cmp (const treap_key* const key1,
             const treap_key* const key2)
{
    assert (key1);
    assert (key2);

    const int key1_int = *(int*) key1->key;
    const int key2_int = *(int*) key2->key;

    if (key1_int < key2_int) return TREAP_LESS;
    if (key1_int > key2_int) return TREAP_GREATER;
    return TREAP_EQUAL;
}

operation_t
GetOperation (const char* const operation_str)
{
    assert (operation_str);

    if (strcmp (operation_str, INSERT_STR) == 0)
        return INSERT;

    if (strcmp (operation_str, DELETE_STR) == 0)
        return DELETE;

    if (strcmp (operation_str, EXISTS_STR) == 0)
        return EXISTS;

    if (strcmp (operation_str, NEXT_STR)   == 0)
        return NEXT;

    if (strcmp (operation_str, PREV_STR)   == 0)
        return PREV;

    if (strcmp (operation_str, KTH_STR)    == 0)
        return KTH;

    return ERROR;
}

void
InsertOperation (treap*           const tree,
                 const treap_key* const key)
{
    assert (tree);
    assert (key);

    const treap_value* const value = (const treap_value*) key;

    assert (TreapInsert (tree, key, value) == TREAP_SUCCESS);
}

void
DeleteOperation (treap*           const tree,
                 const treap_key* const key)
{
    assert (tree);
    assert (key);

    assert (TreapDelete (tree, key) == TREAP_SUCCESS);
}

void
ExistsOperation (const treap*     const tree,
                 const treap_key* const key)
{
    assert (tree);
    assert (key);

    const treap_node* const found_node = TreapFind (tree, key);

    if (found_node != NULL)
        printf ("%s\n", TRUE_STR);
    else
        printf ("%s\n", FALSE_STR);
}

void
NextOperation (const treap*     const tree,
               const treap_key* const key)
{
    assert (tree);
    assert (key);

    treap_node* cur = tree->root;
    treap_node* found_next = NULL;

    int cmp_status = TREAP_EQUAL;

    while (cur != NULL)
    {
        cmp_status = tree->key_cmp (key, cur->key);

        if (cmp_status == TREAP_LESS)
        {
            if (found_next == NULL) found_next = cur;
            else if (tree->key_cmp (cur->key, found_next->key) == TREAP_LESS)
                found_next = cur;

            cur = cur->left;
        }

        else cur = cur->right;
    }

    if (found_next == NULL)
        printf ("%s\n", NONE_STR);

    else
        printf ("%d\n", *(int*)found_next->key->key);
}

void
PrevOperation (const treap*     const tree,
               const treap_key* const key)
{
    assert (tree);
    assert (key);

    treap_node* cur = tree->root;
    treap_node* found_prev = NULL;

    int cmp_status = TREAP_EQUAL;

    while (cur != NULL)
    {
        cmp_status = tree->key_cmp (key, cur->key);

        if (cmp_status == TREAP_GREATER)
        {
            if (found_prev == NULL) found_prev = cur;
            else if (tree->key_cmp (cur->key, found_prev->key) == TREAP_GREATER)
                found_prev = cur;

            cur = cur->right;
        }

        else cur = cur->left;
    }

    if (found_prev == NULL)
        printf ("%s\n", NONE_STR);

    else
        printf ("%d\n", *(int*)found_prev->key->key);
}

void
KthOperation (const treap* const tree,
              const size_t index)
{
    assert (tree);

    const treap_node* cur = tree->root;
    size_t cur_size = index;
    size_t left_subtree_size = 0;

    while (cur != NULL)
    {
        left_subtree_size = TreapSize (cur->left);

        if (left_subtree_size == cur_size)
            break;

        else if (left_subtree_size > cur_size)
            cur = cur->left;

        else
        {
            cur_size -= left_subtree_size + 1;
            cur = cur->right;
        }
    }

    if (cur == NULL)
        printf ("%s\n", NONE_STR);
    else
        printf ("%d\n", *(int*)cur->key->key);
}
//-------------------------------------

//-------------------------------------
// Requests execution function
void
ExecuteRequests (treap* const tree)
{
    assert (tree);

    int input_value = 0;
    treap_key* key  = TreapKeyConstructor (&input_value, sizeof (int));
    assert (key);

    char input_str[STRING_MAX_LEN] = {};

    operation_t operation = 0;

    while (scanf ("%6s", input_str) != EOF)
    {
        operation = GetOperation (input_str);
        if (operation == ERROR) break;

        scanf ("%d", (int*)key->key);

        switch (operation)
        {
            case INSERT:
                InsertOperation (tree, key);
                break;

            case DELETE:
                DeleteOperation (tree, key);
                break;

            case EXISTS:
                ExistsOperation (tree, key);
                break;

            case NEXT:
                NextOperation (tree, key);
                break;

            case PREV:
                PrevOperation (tree, key);
                break;

            case KTH:
                KthOperation (tree, *(int*)key->key);
                break;

            default:
                return;
        }
    }

    key = TreapKeyDestructor (key);
}

int main (void)
{
    srand (time (NULL));

    treap* tree = TreapConstructor (key_cmp);
    assert (tree);

    ExecuteRequests (tree);

    tree = TreapDestructor (tree);
    return 0;
}
//-------------------------------------
