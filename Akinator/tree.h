/*!
 * \file
 * Contains tree structure.
 */
#ifndef AKINATOR_TREE_H
#define AKINATOR_TREE_H
#include <stdio.h>
#include <stdlib.h>

/*!
 * Canary
 */
typedef int Canary;
enum { TREENODE_CANARY = 0xFD };

/*!
 * Tree node types.
 */
enum
{
    TREENODE_TYPE_NONE,
    TREENODE_TYPE_QUESTION,
    TREENODE_TYPE_ANSWER
};

/*!
 * Tree node error codes.
 */
enum
{
    TREENODE_OK = 0,
    TREENODE_ERR_MEMORY_ALLOC = 1,
    TREENODE_ERR_BROKEN_PTR = 2,
    TREENODE_ERR_BROKEN_DATA = 3,
    TREENODE_ERR_BROKEN_CANARY = 4
};

/*!
 * Tree node.
 */
struct TreeNode
{
    Canary left_canary;
    int type;
    char *data;
    struct TreeNode *parent;
    struct TreeNode *left;
    struct TreeNode *right;
    Canary right_canary;
};

/*!
 * Tree node typedef.
 */
typedef struct TreeNode TreeNode;

#ifndef NDEBUG
#define ASSERT_TREENODE_OK(_ptr_) { \
    int _err_code_ = TREENODE_ERR_BROKEN_PTR; \
    if (!_ptr_ || (_err_code_ = treenode_verify(_ptr_)) != TREENODE_OK) \
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
#define TREENODE_DUMP(_ptr_, _out1_, _out2_) { \
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

/*!
 * Tree node constructor.
 * @param this_ pointer to treenode.
 * @param type  type.
 * @param data data.
 * @return error code.
 */
int treenode_ctor(TreeNode **this_, TreeNode *parent, int type, char *data);
/*!
 * Tree node destructor.
 * @param this_
 */
void treenode_dtor(TreeNode *this_);
/*!
 * Tree node verifier.
 * @param this_
 * @return error code.
 */
int treenode_verify(TreeNode *this_);
/*!
 * Tree node dump method.
 * @param this_
 * @param dumpster
 */
void treenode_dump(TreeNode *this_, FILE *dumpster);
/*!
 * Append the left child.
 * @param this_
 * @param child
 */
void treenode_attach_left(TreeNode *this_, TreeNode *child);
/*!
 * Append the right child.
 * @param this_
 * @param child
 */
void treenode_attach_right(TreeNode *this_, TreeNode *child);


#endif //AKINATOR_TREE_H
