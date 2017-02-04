/*!
 * \file
 * TreeNode Data structure.
 */
#ifndef TREE_H
#define TREE_H 
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*!
 * Canary
 */
typedef unsigned Canary;
/*!
 * Canary value for TreeNode.
 */
enum { CANARY_TREENODE = 0xFD };

/*!
 * Data buffer size.
 */
enum { TREENODE_DATA_BUFFER_SIZE = 256 };

/*!
 * Broken tree node type.
 */
enum 
{
    TREENODE_TYPE_UNKNOWN = -1
};

/*!
 * Tree node error codes.
 */
enum
{
    TREE_OK                = 0,
    TREE_ERR_BROKEN_PTR    = 1,
    TREE_ERR_BROKEN_CANARY = 2,
    TREE_ERR_WRONG_SYNTAX  = 3
};

/*!
 * Tree node.
 */
struct TreeNode 
{
    Canary leftCanary;
    int type;
    char data[TREENODE_DATA_BUFFER_SIZE];
    struct TreeNode *parent;
    struct TreeNode *left;
    struct TreeNode *right;
    Canary rightCanary;
};

/*!
 * Tree node definition.
 */
typedef struct TreeNode TreeNode;

#ifndef NDEBUG
/*!
 * Assert if node is verified correctly.
 */
#define ASSERT_TREENODE_OK(_ptr_) \
{ \
    int _err_code_ = TREE_ERR_BROKEN_PTR; \
    if (!_ptr_ || (_err_code_ = TreeNodeVerify(_ptr_)) != TREE_OK) \
    { \
        fprintf(stderr, "[%s:%s:%d] ASSERT_TREENODE_OK failed (error code %d).\n", \
                __FILE__, \
                __func__, \
                __LINE__, \
                _err_code_); \
        abort(); \
    } \
}
#else
#define ASSERT_TREENODE_OK(_ptr_) ;
#endif

/*!
 * TreeNode dump macro.
 */
#define TREENODE_DUMP(_ptr_, _out1_, _out2_)  \
{ \
    FILE *_fptr_ = fopen(_out1_, "w"); \
    if (_fptr_) \
    { \
        fprintf(_fptr_, "digraph G {\n"); \
        TreeNodeDump(_ptr_, _fptr_); \
        fprintf(_fptr_, "}\n"); \
        fclose(_fptr_); \
        system("dot -Tpng " _out1_ " -o " _out2_); \
    } \
}

/*!
 * Tree node constructor.
 */
TreeNode *TreeNodeCtor(int type, const char *data);
/*!
 * Tree node destructor.
 */
void TreeNodeDtor(TreeNode *this_);
/*!
 * Tree node verifier.
 */
int TreeNodeVerify(const TreeNode *this_);
/*!
 * Tree node dump function.
 */
void TreeNodeDump(const TreeNode *this_, FILE *dumpster);
/*!
 * Append the left child.
 */
void TreeNodeAttachLeft(TreeNode *this_, TreeNode *child);
/*!
 * Append the right child.
 */
void TreeNodeAttachRight(TreeNode *this_, TreeNode *child);
/*!
 * Detach the node from its parent.
 */
TreeNode *TreeNodeDetach(TreeNode *this_);
/*!
 * Serialize the tree node.
 */
void TreeNodeSerialize(const TreeNode *this_, FILE *output);
/*!
 * Deserialize.
 */
int TreeNodeDeserialize(TreeNode **this_, const char *str);
/*!
 * Search.
 */
TreeNode *TreeNodeSearch(TreeNode *this_, int type, const char *str);
/*!
 * Is parent?
 */
bool TreeNodeIsParent(TreeNode *this_, TreeNode *child);

#endif /* ifndef TREE_H */
