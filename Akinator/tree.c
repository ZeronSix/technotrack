/*
 * \file
 * TreeNode Data Structore (sources)
 */
#include "tree.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*!
 * Free and return null.
 */
#define FREE_AND_RET_NULL(_ptr_) { free(_ptr_); return NULL; }

TreeNode *TreeNodeCtor(int type, const char *data)
{
    assert(data);

    TreeNode *this_ = (TreeNode *)calloc(1, sizeof(TreeNode));
    if (!this_)
        return NULL;

    strncpy(this_->data, data, TREENODE_DATA_BUFFER_SIZE - 1);
    this_->leftCanary = CANARY_TREENODE;
    this_->type       = type;
    this_->left       = NULL;
    this_->right      = NULL;
    this_->rightCanary = CANARY_TREENODE;

    ASSERT_TREENODE_OK(this_);
    return this_;
}

void TreeNodeDtor(TreeNode *this_)
{
    if (!this_)
        return;

    TreeNodeDtor(this_->left);
    TreeNodeDtor(this_->right);
    this_->left  = NULL;
    this_->right = NULL;
    memset(this_->data, '\0', TREENODE_DATA_BUFFER_SIZE);
    this_->type = 0;
}

int TreeNodeVerify(const TreeNode *this_)
{
    if (!this_) 
        return TREE_ERR_BROKEN_PTR;
    if (this_->leftCanary != CANARY_TREENODE || this_->rightCanary != CANARY_TREENODE)
        return TREE_ERR_BROKEN_CANARY;

    if (this_->left)
    {
        int left_ok = TreeNodeVerify(this_->left);
        if (left_ok != TREE_OK)
            return left_ok;
    }
    if (this_->right)
    {
        int right_ok = TreeNodeVerify(this_->right);
        if (right_ok != TREE_OK)
            return right_ok;
    }

    return TREE_OK;
}

void TreeNodeDump(const TreeNode *this_, FILE *dumpster)
{
    assert(dumpster);
    if (!this_)
        return;

    fprintf(dumpster, 
            "TreeNode_%p [label=\"TreeNode[%p]\\l"
            "{\\l"
            "   leftCanary  = 0x%X;\\l"
            "   parent      = %p;\\l"
            "   type        = %d;\\l"
            "   data        = \'%s\';\\l"
            "   rightCanary = 0x%x;\\l"
            "}\\l\"]\n",
            this_,
            this_,
            this_->leftCanary,
            this_->parent,
            this_->type,
            this_->data,
            this_->rightCanary);

    TreeNodeDump(this_->left, dumpster);
    TreeNodeDump(this_->right, dumpster);

    if (!this_->left)
    {
        fprintf(dumpster, "TreeNode_%p_Left [label=\"Empty\"]\n", this_);
        fprintf(dumpster, "TreeNode_%p -> TreeNode_%p_Left\n", this_, this_);
    }
    else
        fprintf(dumpster, "TreeNode_%p -> TreeNode_%p\n", this_, this_->left);

    if (!this_->right)
    {
        fprintf(dumpster, "TreeNode_%p_Right [label=\"Empty\"]\n", this_);
        fprintf(dumpster, "TreeNode_%p -> TreeNode_%p_Right\n", this_, this_);
    }
    else
        fprintf(dumpster, "TreeNode_%p -> TreeNode_%p\n", this_, this_->right);
}

void TreeNodeAttachLeft(TreeNode *this_, TreeNode *child)
{
    ASSERT_TREENODE_OK(this_);
    ASSERT_TREENODE_OK(child);

    if (this_->left)
    {
        TreeNodeDtor(this_->left);
        this_->left = NULL;
    }
    this_->left   = child;
    child->parent = this_;

    ASSERT_TREENODE_OK(this_);
}

void TreeNodeAttachRight(TreeNode *this_, TreeNode *child)
{
    ASSERT_TREENODE_OK(this_);
    ASSERT_TREENODE_OK(child);

    if (this_->right)
    {
        TreeNodeDtor(this_->right);
        this_->right = NULL;
    }
    this_->right  = child;
    child->parent = this_;

    ASSERT_TREENODE_OK(this_);
}

TreeNode *TreeNodeDetach(TreeNode *this_)
{
    ASSERT_TREENODE_OK(this_->parent);
    TreeNode *parent = this_->parent;

    if (this_->parent->left == this_)
        this_->parent->left = NULL;
    else if (this_->parent->right == this_)
        this_->parent->right = NULL;
    this_->parent = NULL;

    ASSERT_TREENODE_OK(this_);
    ASSERT_TREENODE_OK(parent);
    return this_;
}

static const char *NIL_NODE = "nil";
static const size_t NIL_NODE_LEN = 3;
static const char L_BRACKET = '(';
static const char R_BRACKET = ')';
static const char STR_QUOTE = '\'';

void TreeNodeSerialize(const TreeNode *this_, FILE *output)
{
    assert(output);

    if (!this_)
    {
        fputs(NIL_NODE, output);
        return;
    }
    
    fputc(L_BRACKET, output);
    TreeNodeSerialize(this_->left, output);
    fprintf(output, " %d ", this_->type);
    fprintf(output, "%c%s%c ", STR_QUOTE, this_->data, STR_QUOTE);
    TreeNodeSerialize(this_->right, output);
    fputc(R_BRACKET, output);
}

#define SKIP_SPACES(_s_) { while(isspace(**_s_)) (*_s_)++; }
#define SET_NULL_AND_RET(_ptr_, _res_) { *_ptr_ = NULL; return _res_; }
int _Deserialize(TreeNode **this_, const char **s)
{
    assert(s);
    assert(*s);
    assert(this_);

    SKIP_SPACES(s);

    if (**s == L_BRACKET)
    {
        (*s)++;
    }
    else if (strncmp(*s, NIL_NODE, NIL_NODE_LEN) == 0)
    {
        *s += NIL_NODE_LEN; 
        return TREE_OK;
    }
    else
    {
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }

    int result = TREE_OK;
    TreeNode *left = NULL;
    if ((result = _Deserialize(&left, s)) != TREE_OK)
        SET_NULL_AND_RET(this_, result);

    // data reading
    int chars_read = 0;
    int type       = 0;
    if (sscanf(*s, "%d%n", &type, &chars_read) != 1)
    {
        TreeNodeDtor(left);
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }
    *s += chars_read;
    
    SKIP_SPACES(s);
    if (**s != STR_QUOTE)
    {
        TreeNodeDtor(left);
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }
    (*s)++;
    char data[TREENODE_DATA_BUFFER_SIZE] = "";
    // TODO: fix the format string
    if (sscanf(*s, "%255[^']%n", data, &chars_read) != 1)
    {
        TreeNodeDtor(left);
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }
    *s += chars_read;
    if (**s != STR_QUOTE)
    {
        TreeNodeDtor(left);
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }
    (*s)++;

    TreeNode *right = NULL;
    if ((result = _Deserialize(&right, s)) != TREE_OK)
    {
        TreeNodeDtor(left);
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }

    SKIP_SPACES(s);
    if (**s != R_BRACKET)
    {
        TreeNodeDtor(left);
        TreeNodeDtor(right);
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }
    (*s)++;
    
    TreeNode *node = TreeNodeCtor(type, data);
    if (!node)
    {
        TreeNodeDtor(left);
        TreeNodeDtor(right);
        SET_NULL_AND_RET(this_, TREE_ERR_BROKEN_PTR);
    }

    if (left)
        TreeNodeAttachLeft(node, left);
    if (right)
        TreeNodeAttachRight(node, right);

    ASSERT_TREENODE_OK(node);

    *this_ = node;
    return TREE_OK;
}

int TreeNodeDeserialize(TreeNode **this_, const char *str)
{
    return _Deserialize(this_, &str);
}

TreeNode *TreeNodeSearch(TreeNode *this_, int type, const char *str)
{
    assert(str);
    if (!this_)
        return NULL;

    if (this_->type == type && strcmp(str, this_->data) == 0)
        return this_;
    else
    {
        TreeNode *left = TreeNodeSearch(this_->left, type, str);
        if (left)
            return left;
        else
            return TreeNodeSearch(this_->right, type, str);
    }    
}

bool TreeNodeIsParent(TreeNode *this_, TreeNode *child)
{
    assert(this_);
    assert(child);
    ASSERT_TREENODE_OK(this_);
    ASSERT_TREENODE_OK(child);
    
    if (this_ == child)
        return true;
    else 
    {
        bool leftParent = false;
        if (this_->left)
           leftParent = TreeNodeIsParent(this_->left, child);

        bool rightParent = false;
        if (this_->right)
           rightParent = TreeNodeIsParent(this_->right, child);

        return leftParent || rightParent;
    }
}
