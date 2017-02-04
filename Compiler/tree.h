/*!
 * \file
 * Tree data structure.
 */
#ifndef ZCC_TREE_H
#define ZCC_TREE_H 
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

/*!
 * TreeNode
 */
struct TreeNode
{
    int             type;
    void            *data;
    struct TreeNode *parent;
    struct TreeNode *left;
    struct TreeNode *right;  
};

/*!
 * TreeNode.
 */
typedef struct TreeNode TreeNode;

TreeNode *treenode_new(int type, void *data);
int      treenode_del(TreeNode *this_);
void     treenode_dump(const TreeNode *this_, FILE *dumpster);
void     treenode_attachl(TreeNode *this_, TreeNode *child);
void     treenode_attachr(TreeNode *this_, TreeNode *child);
TreeNode *treenode_detach(TreeNode *this_);
TreeNode *treenode_copy(const TreeNode *this_);

/*!
 * TreeNode dump macro.
 */
#define TREENODE_DUMP(_ptr_, _out1_, _out2_)  \
{ \
    FILE *_fptr_ = fopen(_out1_, "w"); \
    if (_fptr_) \
    { \
        fprintf(_fptr_, "digraph G {\n"); \
        treenode_dump(_ptr_, _fptr_); \
        fprintf(_fptr_, "}\n"); \
        fclose(_fptr_); \
        system("dot -Tpng " _out1_ " -o " _out2_); \
    } \
}

#endif /* ifndef ZCC_TREE_H */
