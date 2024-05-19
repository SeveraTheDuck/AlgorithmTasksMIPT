#pragma once



#include <stdbool.h>
#include <stdlib.h>
#include <string.h>



/******************************************************************************
 * @name Splay tree structs and types.
 * 
 * @{
 ******************************************************************************/
/**
 * @brief Splay key structure.
 * 
 * @details A container for key of the node.
 */
typedef struct splay_key
{
    void*  key;                                 ///< buffer for the key
    size_t key_len;                             ///< size of the key buffer in bytes
}
splay_key;

/**
 * @brief Splay value structure.
 * 
 * @details A container for value of the node.
 */
typedef struct splay_value
{
    void*  value;                               ///< buffer for the value
    size_t value_len;                           ///< size of the value buffer in bytes
}
splay_value;

/**
 * @brief Splay node structure.
 * 
 * @details A container for node and its information.
 * Includes key and value fields and pointers to child nodes and parent node.
 */
typedef struct splay_node
{
    splay_key*         key;                     ///< key structure
    splay_value*       value;                   ///< value structure
    struct splay_node* left;                    ///< pointer to the left child node
    struct splay_node* right;                   ///< pointer to the right child node
    struct splay_node* parent;                  ///< pointer to the parent node
}
splay_node;

/**
 * @brief Splay tree structure.
 * 
 * @details Includes pointer to the root node and pointer to key comparator function.
 */
typedef struct splay_tree
{
    splay_node* root;                           ///< pointer to the root node
    int (*key_cmp) (const splay_key* const,     ///< key comparator function
                    const splay_key* const);
}
splay_tree;

/**
 * @brief Splay tree errors enumeration.
 * 
 * @details List of possible error codes, including no-error status.
 */
enum splay_errors
{
    SPLAY_TREE_SUCCESS = 0,                     ///< no error occured during function
    SPLAY_TREE_ERROR   = 1                      ///< error occured during function
};

/**
 * @brief Splay tree error type.
 * 
 * @details Return type for functions error codes.
 */
typedef size_t splay_error_t;

enum splay_tree_cmp_status
{
    SPLAY_TREE_CMP_LESS    = -1,                ///< the first key is less than the second key
    SPLAY_TREE_CMP_EQUAL   =  0,                ///< the first and the second keys are equal
    SPLAY_TREE_CMP_GREATER =  1                 ///< the first key is greater than the second key
};

/******************************************************************************
 * @}
 ******************************************************************************/



/******************************************************************************
 * @name Splay tree functions prototypes
 * 
 * @{
 ******************************************************************************/
//-------------------------------------
// Splay tree constructor and destructor.
//-------------------------------------
/**
 * @brief Splay tree constructor.
 * 
 * @param[in] key_cmp key comparator function.
 * 
 * @retval Pointer to the allocated structure.
 * @retval NULL if allocation error occurred.
 * 
 * @details Allocates memory for the splay_tree structure and 
 * fills its key_cmp field with given compare function.
 */
splay_tree*
SplayTreeConstructor (int (*key_cmp) (const splay_key* const,
                                      const splay_key* const));

/**
 * @brief Splay tree destructor.
 * 
 * @param[in] tree Pointer to the tree to destroy.
 * 
 * @retval NULL in all cases.
 * 
 * @details Destroys all nodes using SplayTreeDeleteKey() 
 * and frees allocated memory.
 */
splay_tree*
SplayTreeDestructor (splay_tree* const tree);
//-------------------------------------

//-------------------------------------
// Splay node constructor and destructor.
//-------------------------------------
/**
 * @brief Splay node constructor.
 * 
 * @param[in] key   structure of the key   to copy. No copying if NULL is given.
 * @param[in] value structure of the value to copy. No copying if NULL is given.
 * 
 * @retval Pointer to the allocated structure.
 * @retval NULL if allocation error occurred.
 * 
 * @details Allocates memory for the splay_node structure and 
 * fills its key and value fields with given structures.
 * Key and value are being copied using SplayKeyCopy()
 * and SplayValueCopy() functions.
 */
splay_node*
SplayNodeConstructor (const splay_key*   const key,
                      const splay_value* const value);

/**
 * @brief Splay node destructor.
 * 
 * @param[in] node a node to destroy.
 * @retval NULL in all cases.
 * 
 * @details Calls for key and value destructors and frees allocated memory.
 */
splay_node*
SplayNodeDestructor  (splay_node* const node);
//-------------------------------------

//-------------------------------------
// Splay key and value constructor, destructor and copy.
//-------------------------------------
/**
 * @brief Splay key constructor
 * 
 * @param[in] key buffer of the key.
 * @param[in] key_len buffer length.
 * 
 * @retval Pointer to the allocated structure.
 * @retval NULL if allocation error occurred.
 * 
 * @details Allocates memory for the splay_key structure.
 * Allocates key_len bytes for a new buffer and
 * copies key_len bytes from the given buffer.
 * If buffer is NULL or key_len is 0, only allocates splay_key structure.
 */
splay_key*
SplayKeyConstructor (const void* const key,
                     const size_t key_len);



/**
 * @brief Splay key destructor.
 * 
 * @param[in] key splay_key structure to destroy.
 * 
 * @retval NULL in all cases.
 * 
 * @details Frees buffer inside the structure.
 * Frees given splay_key structure.
 */
splay_key*
SplayKeyDestructor (splay_key* const key);

/**
 * @brief Splay value constructor.
 * 
 * @param[in] value buffer of the value.
 * @param[in] value_len buffer length.
 * 
 * @retval Pointer to the allocated structure.
 * @retval NULL if allocation error occurred.
 * 
 * @see SplayKeyConstructor(). Same thing but for splay_value.
 */
splay_value*
SplayValueConstructor (const void* const value,
                       const size_t value_len);



/**
 * @brief Splay value destructor.
 * 
 * @param[in] value splay_value structure to destroy.
 * 
 * @retval NULL in all cases.
 * 
 * @see SplayKeyDestructor(). Same thing but for splay_value.
 */
splay_value*
SplayValueDestructor (splay_value* const value);
//-------------------------------------

//-------------------------------------
// Interface
//-------------------------------------
/**
 * @brief Splay tree insert function.
 * 
 * @param[in] tree  splay_tree structure to insert into.
 * @param[in] key   splay_key   to insert.
 * @param[in] value splay_value to insert.
 * 
 * @retval SPLAY_TREE_ERROR   if error occured.
 * @retval SPLAY_TREE_SUCCESS otherwise.
 * 
 * @details If tree has no root, calls for SplayInsertRoot() function, returns its status.
 * Calls for SplayTreeInsertImpl() otherwise.
 * Calls for Splay function in the end to keep new node in the root.
 */
splay_error_t
SplayTreeInsert (splay_tree*        const tree,
                 const splay_key*   const key,
                 const splay_value* const value);

/**
 * @brief Splay tree delete by key function.
 * 
 * @param[in] tree splay_tree structure to delete from.
 * @param[in] key  splay_key to find the node to delete.
 * 
 * @retval SPLAY_TREE_ERROR   if error occured.
 * @retval SPLAY_TREE_SUCCESS otherwise.
 * 
 * @details Searches for the node to delete using SplayTreeFind() function.
 * If no node found, returns SPLAY_TREE_ERROR.
 * Otherwise, after SplayTreeFind() function the node is located in the root.
 * Replaces it with the node from SplayFindMaxKey()
 * and deletes the node using SplayNodeDestructor() function.
 */
splay_error_t
SplayTreeDeleteKey (splay_tree*      const tree,
                    const splay_key* const key);

/**
 * @brief Splay tree find by key function.
 * 
 * @param[in] tree splay_tree structure to find the node.
 * @param[in] key  splay_key  structure to compare to.
 * 
 * @retval pointer to the node, NULL if not found.
 * 
 * @details The function searches for the node with equal key
 * using key_cmp function from the tree structure.
 * Calls for Splay() function from the found node to place it in the root.
*/
splay_node*
SplayTreeFind (splay_tree*      const tree,
               const splay_key* const key);
//-------------------------------------
/******************************************************************************
 * @}
 ******************************************************************************/