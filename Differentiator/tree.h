/*!
 * \file
 * DTreeNode Data structure.
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
 * Canary value for DTreeNode.
 */
enum { CANARY_TREENODE = 0xFD };

/*!
 * Poison value for tree data.
 */
enum { POISON_TREENODE = 666 };

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
struct DTreeNode 
{
    Canary leftCanary;
    int type;
    double data;
    struct DTreeNode *parent;
    struct DTreeNode *left;
    struct DTreeNode *right;
    Canary rightCanary;
};

/*!
 * Tree node definition.
 */
typedef struct DTreeNode DTreeNode;

#ifndef NDEBUG
/*!
 * Assert if node is verified correctly.
 */
#define ASSERT_TREENODE_OK(_ptr_) \
{ \
    int _err_code_ = TREE_ERR_BROKEN_PTR; \
    if (!_ptr_ || (_err_code_ = DTreeNodeVerify(_ptr_)) != TREE_OK) \
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
 * DTreeNode dump macro.
 */
#define TREENODE_DUMP(_ptr_, _out1_, _out2_)  \
{ \
    FILE *_fptr_ = fopen(_out1_, "w"); \
    if (_fptr_) \
    { \
        fprintf(_fptr_, "digraph G {\n"); \
        DTreeNodeDump(_ptr_, _fptr_); \
        fprintf(_fptr_, "}\n"); \
        fclose(_fptr_); \
        system("dot -Tpng " _out1_ " -o " _out2_); \
    } \
}

/*!
 * Tree node constructor.
 */
DTreeNode *DTreeNodeCtor(int type, double data);
/*!
 * Tree node destructor.
 */
int DTreeNodeDtor(DTreeNode *this_);
/*!
 * Tree node verifier.
 */
int DTreeNodeVerify(const DTreeNode *this_);
/*!
 * Tree node dump function.
 */
void DTreeNodeDump(const DTreeNode *this_, FILE *dumpster);
/*!
 * Append the left child.
 */
void DTreeNodeAttachLeft(DTreeNode *this_, DTreeNode *child);
/*!
 * Append the right child.
 */
void DTreeNodeAttachRight(DTreeNode *this_, DTreeNode *child);
/*!
 * Detach the node from its parent.
 */
DTreeNode *DTreeNodeDetach(DTreeNode *this_);
/*!
 * Serialize the tree node.
 */
void DTreeNodeSerialize(const DTreeNode *this_, FILE *output);
/*!
 * Deserialize.
 */
int DTreeNodeDeserialize(DTreeNode **this_, const char *str);
/*!
 * Search.
 */
DTreeNode *DTreeNodeSearch(DTreeNode *this_, int type, double data);
/*!
 * Is parent?
 */
bool DTreeNodeIsParent(DTreeNode *this_, DTreeNode *child);
/*!
 * Copy.
 */
DTreeNode *DTreeNodeCopy(DTreeNode *this_);

#endif /* ifndef TREE_H */
