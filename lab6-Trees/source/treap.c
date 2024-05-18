#include "../include/treap.h"



//-----------------------------------------------------------------------------
// Treap static functions prototypes
//-----------------------------------------------------------------------------
//-------------------------------------
// Destructor helper function
static treap_node*
TreapDestructorImpl (treap_node* const node);
//-------------------------------------

//-------------------------------------
// Key and value copy
static treap_key*
TreapKeyCopy   (const treap_key* const key);

static treap_value*
TreapValueCopy (const treap_value* const value);
//-------------------------------------

//-------------------------------------
// Split and merge
static treap_pair*
TreapSplit             (treap*           const tree,
                        const treap_key* const key);

static treap_pair*
TreapSplitImpl         (treap_node*      const tree,
                        const treap_key* const key,
                        int (*key_cmp)  (const treap_key* const,
                                         const treap_key* const));
static treap_node*
TreapMerge             (treap_node* const tree1,
                        treap_node* const tree2);

static treap_pair*
TreapPairConstructor   (treap_node* const tree1,
                        treap_node* const tree2);

static treap_pair*
TreapPairDestructor    (treap_pair* const pair);
//-------------------------------------

//-------------------------------------
// Delete helpers
static treap_error_t
TreapDeleteImpl      (treap*      const tree,
                      treap_node* const found_node);

static treap_error_t
TreapDeleteFoundNode (treap_pair* const pair,
                      treap_node* const found_node);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Treap functions implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Constructor and destructor
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
//-------------------------------------

//-------------------------------------
// Node constructor and destructor
treap_node*
TreapNodeConstructor (const treap_key*   const key,
                      const treap_value* const value)
{
    treap_node* const node =
        (treap_node*) calloc (1, sizeof (treap_node));
    if (node == NULL) return NULL;

    node->priority = rand ();
    node->key      = TreapKeyCopy   (key);
    node->value    = TreapValueCopy (value);

    return node;
}

treap_node*
TreapNodeDestructor (treap_node* const node)
{
    if (node == NULL) return NULL;

    node->key   = TreapKeyDestructor   (node->key);
    node->value = TreapValueDestructor (node->value);

    free (node);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Key and value constructor and destructor
treap_key*
TreapKeyConstructor (const void* const data,
                     const size_t key_size)
{
    treap_key* const key =
        (treap_key*) calloc (1, sizeof (treap_key));
    if (key == NULL) return NULL;

    if (data != NULL && key_size != 0)
    {
        key->data = malloc (key_size);
        if (key->data == NULL)
            return TreapKeyDestructor (key);

        memcpy (key->data, data, key_size);
        key->key_size = key_size;
    }

    return key;
}

treap_key*
TreapKeyDestructor (treap_key* const key)
{
    if (key == NULL) return NULL;

    free (key->data);
    free (key);
    return NULL;
}

treap_value*
TreapValueConstructor (const void* const data,
                       const size_t value_size)
{
    treap_value* const value =
        (treap_value*) calloc (1, sizeof (treap_value));
    if (value == NULL) return NULL;

    if (data != NULL && value_size != 0)
    {
        value->data = malloc (value_size);
        if (value->data == NULL)
            return TreapValueDestructor (value);

        memcpy (value->data, data, value_size);
        value->value_size = value_size;
    }

    return value;
}

treap_value*
TreapValueDestructor (treap_value* const value)
{
    if (value == NULL) return NULL;

    free (value->data);
    free (value);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Find, insert and delete
treap_node*
TreapFind (const treap*     const tree,
           const treap_key* const key)
{
    if (tree          == NULL ||
        tree->key_cmp == NULL ||
        key           == NULL)
        return NULL;

    int cmp_status = TREAP_CMP_EQUAL;
    treap_node* cur_node = tree->root;

    while (cur_node != NULL)
    {
        cmp_status = tree->key_cmp (key, cur_node->key);

        if (cmp_status == TREAP_CMP_LESS)
            cur_node = cur_node->left;

        else if (cmp_status == TREAP_CMP_GREATER)
            cur_node = cur_node->right;

        else return cur_node;
    }

    return NULL;
}

treap_error_t
TreapInsert (treap*             const tree,
             const treap_key*   const key,
             const treap_value* const value)
{
    if (tree          == NULL ||
        tree->key_cmp == NULL ||
        key           == NULL)
        return TREAP_ERROR;

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
        return TREAP_ERROR;

    return TreapDeleteImpl (tree, found_node);
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Treap static functions implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Destructor helper
static treap_node*
TreapDestructorImpl (treap_node* const node)
{
    if (node == NULL) return NULL;

    node->left  = TreapDestructorImpl (node->left);
    node->right = TreapDestructorImpl (node->right);

    return TreapNodeDestructor (node);
}
//-------------------------------------

//-------------------------------------
// Key and value copy
static treap_key*
TreapKeyCopy (const treap_key* const key)
{
    if (key == NULL) return NULL;
    return TreapKeyConstructor (key->data, key->key_size);
}

static treap_value*
TreapValueCopy (const treap_value* const value)
{
    if (value == NULL) return NULL;
    return TreapValueConstructor (value->data, value->value_size);
}
//-------------------------------------

//-------------------------------------
// Split and merge
static treap_pair*
TreapSplit (treap*           const tree,
            const treap_key* const key)
{
    if (tree == NULL) return TreapPairConstructor (NULL, NULL);

    return TreapSplitImpl (tree->root, key, tree->key_cmp);
}

static treap_pair*
TreapSplitImpl (treap_node*      const tree,
                const treap_key* const key,
                int (*key_cmp)  (const treap_key* const,
                                 const treap_key* const))
{
    if (tree == NULL)
        return TreapPairConstructor (NULL, NULL);

    treap_pair* pair = NULL;

    if (key_cmp (key, tree->key) == TREAP_CMP_GREATER)
    {
        pair = TreapSplitImpl (tree->right, key, key_cmp);
        tree->right = pair->tree1;
        pair->tree1 = tree;

        return pair;
    }

    else
    {
        pair = TreapSplitImpl (tree->left, key, key_cmp);
        tree->left  = pair->tree2;
        pair->tree2 = tree;

        return pair;
    }
}

static treap_node*
TreapMerge (treap_node* const tree1,
            treap_node* const tree2)
{
    if (tree1 == NULL) return tree2;
    if (tree2 == NULL) return tree1;

    if (tree1->priority <= tree2->priority)
    {
        tree2->left = TreapMerge (tree1, tree2->left);
        return tree2;
    }

    else
    {
        tree1->right = TreapMerge (tree1->right, tree2);
        return tree1;
    }
}

static treap_pair*
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

static treap_pair*
TreapPairDestructor (treap_pair* const pair)
{
    free (pair);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Delete helpers
static treap_error_t
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

static treap_error_t
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
        cur  = next;
        next = next->left;
    }

    cur->left = found_node->right;
    TreapNodeDestructor (found_node);

    return TREAP_SUCCESS;
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
