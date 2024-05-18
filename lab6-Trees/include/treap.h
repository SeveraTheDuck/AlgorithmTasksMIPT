#include <stdlib.h>
#include <string.h>



//-----------------------------------------------------------------------------
// Structs and types
//-----------------------------------------------------------------------------
typedef struct treap_key
{
    void*  data;
    size_t key_size;  ///< size in bytes
}
treap_key;

typedef struct treap_value
{
    void*  data;
    size_t value_size;  ///< size in bytes
}
treap_value;

typedef struct treap_node
{
    int                priority;
    treap_key*         key;
    treap_value*       value;

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
    TREAP_CMP_LESS    = -1,
    TREAP_CMP_EQUAL   =  0,
    TREAP_CMP_GREATER =  1
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Treap functions' prototypes
//-----------------------------------------------------------------------------
//-------------------------------------
// Treap constructor and destructor
treap*
TreapConstructor (int (*key_cmp) (const treap_key* const,
                                  const treap_key* const));

treap*
TreapDestructor  (treap* const tree);
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
// Treap key and value constructor and destructor and copy
treap_key*
TreapKeyConstructor   (const void* const data,
                       const size_t key_size);

treap_key*
TreapKeyDestructor    (treap_key* const key);

treap_value*
TreapValueConstructor (const void* const data,
                       const size_t value_size);

treap_value*
TreapValueDestructor  (treap_value* const value);
//-------------------------------------

//-------------------------------------
// Treap interface
treap_node*
TreapFind   (const treap*     const tree,
             const treap_key* const key);

treap_error_t
TreapInsert (treap*             const tree,
             const treap_key*   const key,
             const treap_value* const value);

treap_error_t
TreapDelete (treap*           const tree,
             const treap_key* const key);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
