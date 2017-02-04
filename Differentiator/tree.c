/*
 * \file
 * DTreeNode Data Structore (sources)
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

DTreeNode *DTreeNodeCtor(int type, double data)
{
    DTreeNode *this_ = (DTreeNode *)calloc(1, sizeof(DTreeNode));
    if (!this_)
        return NULL;

    this_->leftCanary  = CANARY_TREENODE;
    this_->type        = type;
    this_->left        = NULL;
    this_->right       = NULL;
    this_->data        = data;
    this_->rightCanary = CANARY_TREENODE;

    ASSERT_TREENODE_OK(this_);
    return this_;
}

int DTreeNodeDtor(DTreeNode *this_)
{
    if (!this_)
        return 0;

    DTreeNodeDtor(this_->left);
    DTreeNodeDtor(this_->right);
    this_->left  = NULL;
    this_->right = NULL;
    this_->data = POISON_TREENODE;
    this_->type = 0;
    free(this_);
    return 0;
}

int DTreeNodeVerify(const DTreeNode *this_)
{
    if (!this_) 
        return TREE_ERR_BROKEN_PTR;
    if (this_->leftCanary != CANARY_TREENODE || this_->rightCanary != CANARY_TREENODE)
        return TREE_ERR_BROKEN_CANARY;

    if (this_->left)
    {
        int left_ok = DTreeNodeVerify(this_->left);
        if (left_ok != TREE_OK)
            return left_ok;
    }
    if (this_->right)
    {
        int right_ok = DTreeNodeVerify(this_->right);
        if (right_ok != TREE_OK)
            return right_ok;
    }

    return TREE_OK;
}

void DTreeNodeDump(const DTreeNode *this_, FILE *dumpster)
{
    assert(dumpster);
    if (!this_)
        return;

    fprintf(dumpster, 
            "DTreeNode_%p [label=\"DTreeNode[%p]\\l"
            "{\\l"
            "   leftCanary  = 0x%X;\\l"
            "   parent      = %p;\\l"
            "   type        = %c;\\l"
            "   data        = \'%lg\';\\l"
            "   rightCanary = 0x%X;\\l"
            "}\\l\"]\n",
            this_,
            this_,
            this_->leftCanary,
            this_->parent,
            (char)this_->type,
            this_->data,
            this_->rightCanary);

    DTreeNodeDump(this_->left, dumpster);
    DTreeNodeDump(this_->right, dumpster);

    if (!this_->left)
    {
        fprintf(dumpster, "DTreeNode_%p_Left [label=\"Empty\"]\n", this_);
        fprintf(dumpster, "DTreeNode_%p -> DTreeNode_%p_Left\n", this_, this_);
    }
    else
        fprintf(dumpster, "DTreeNode_%p -> DTreeNode_%p\n", this_, this_->left);

    if (!this_->right)
    {
        fprintf(dumpster, "DTreeNode_%p_Right [label=\"Empty\"]\n", this_);
        fprintf(dumpster, "DTreeNode_%p -> DTreeNode_%p_Right\n", this_, this_);
    }
    else
        fprintf(dumpster, "DTreeNode_%p -> DTreeNode_%p\n", this_, this_->right);
}

void DTreeNodeAttachLeft(DTreeNode *this_, DTreeNode *child)
{
    ASSERT_TREENODE_OK(this_);
    ASSERT_TREENODE_OK(child);

    if (this_->left)
    {
        DTreeNodeDtor(this_->left);
        this_->left = NULL;
    }
    this_->left   = child;
    child->parent = this_;

    ASSERT_TREENODE_OK(this_);
}

void DTreeNodeAttachRight(DTreeNode *this_, DTreeNode *child)
{
    ASSERT_TREENODE_OK(this_);
    ASSERT_TREENODE_OK(child);

    if (this_->right)
    {
        DTreeNodeDtor(this_->right);
        this_->right = NULL;
    }
    this_->right  = child;
    child->parent = this_;

    ASSERT_TREENODE_OK(this_);
}

DTreeNode *DTreeNodeDetach(DTreeNode *this_)
{
    ASSERT_TREENODE_OK(this_->parent);
    DTreeNode *parent = this_->parent;

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

void DTreeNodeSerialize(const DTreeNode *this_, FILE *output)
{
    assert(output);

    if (!this_)
    {
        fputs(NIL_NODE, output);
        return;
    }
    
    fputc(L_BRACKET, output);
    DTreeNodeSerialize(this_->left, output);
    fprintf(output, " %d ", this_->type);
    fprintf(output, "%c%lg%c ", STR_QUOTE, this_->data, STR_QUOTE);
    DTreeNodeSerialize(this_->right, output);
    fputc(R_BRACKET, output);
}

#define SKIP_SPACES(_s_) { while(isspace(**_s_)) (*_s_)++; }
#define SET_NULL_AND_RET(_ptr_, _res_) { *_ptr_ = NULL; return _res_; }
static int _Deserialize(DTreeNode **this_, const char **s)
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
        *this_ = NULL;
        *s += NIL_NODE_LEN; 
        return TREE_OK;
    }
    else
    {
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }

    int result = TREE_OK;
    DTreeNode *left = NULL;
    if ((result = _Deserialize(&left, s)) != TREE_OK)
        SET_NULL_AND_RET(this_, result);

    // data reading
    int chars_read = 0;
    int type       = 0;
    if (sscanf(*s, "%d%n", &type, &chars_read) != 1)
    {
        DTreeNodeDtor(left);
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }
    *s += chars_read;
    
    SKIP_SPACES(s);
    if (**s != STR_QUOTE)
    {
        DTreeNodeDtor(left);
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }
    (*s)++;

    double data = 0;
    if (sscanf(*s, "%lg%n", &data, &chars_read) != 1)
    {
        DTreeNodeDtor(left);
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }
    *s += chars_read;

    SKIP_SPACES(s);
    if (**s != STR_QUOTE)
    {
        DTreeNodeDtor(left);
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }
    (*s)++;

    DTreeNode *right = NULL;
    if ((result = _Deserialize(&right, s)) != TREE_OK)
    {
        DTreeNodeDtor(right);
        DTreeNodeDtor(left);
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }

    SKIP_SPACES(s);
    if (**s != R_BRACKET)
    {
        DTreeNodeDtor(left);
        DTreeNodeDtor(right);
        SET_NULL_AND_RET(this_, TREE_ERR_WRONG_SYNTAX);
    }
    (*s)++;
    
    DTreeNode *node = DTreeNodeCtor(type, data);
    if (!node)
    {
        DTreeNodeDtor(left);
        DTreeNodeDtor(right);
        SET_NULL_AND_RET(this_, TREE_ERR_BROKEN_PTR);
    }

    if (left)
        DTreeNodeAttachLeft(node, left);
    if (right)
        DTreeNodeAttachRight(node, right);

    ASSERT_TREENODE_OK(node);

    *this_ = node;
    return TREE_OK;
}

int DTreeNodeDeserialize(DTreeNode **this_, const char *str)
{
    return _Deserialize(this_, &str);
}

DTreeNode *DTreeNodeSearch(DTreeNode *this_, int type, double data)
{
    if (!this_)
        return NULL;

    if (this_->type == type && CompareDouble(this_->data, data))
        return this_;
    else
    {
        DTreeNode *left = DTreeNodeSearch(this_->left, type, data);
        if (left)
            return left;
        else
            return DTreeNodeSearch(this_->right, type, data);
    }    
}

bool DTreeNodeIsParent(DTreeNode *this_, DTreeNode *child)
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
           leftParent = DTreeNodeIsParent(this_->left, child);

        bool rightParent = false;
        if (this_->right)
           rightParent = DTreeNodeIsParent(this_->right, child);

        return leftParent || rightParent;
    }
}

DTreeNode *DTreeNodeCopy(DTreeNode *this_)
{
    assert(this_);

    DTreeNode *node = DTreeNodeCtor(this_->type, this_->data);
    if (this_->left)
        DTreeNodeAttachLeft(node, DTreeNodeCopy(this_->left));
    if (this_->right)
        DTreeNodeAttachRight(node, DTreeNodeCopy(this_->right));

    return node;
}
