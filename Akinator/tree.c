//
// Created by zeronsix on 14.11.16.
//

#include "tree.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int treenode_ctor(TreeNode **this_, TreeNode *parent, int type, char *data)
{
    assert(this_);

    size_t to_alloc = strlen(data) + 1;
    char *str = (char *)calloc(to_alloc, sizeof(*str));
    if (!str)
        return TREENODE_ERR_MEMORY_ALLOC;
    strcpy(str, data);

    TreeNode *ptr = (TreeNode *)calloc(1, sizeof(TreeNode));
    if (!ptr)
        return TREENODE_ERR_MEMORY_ALLOC;

    ptr->left_canary = TREENODE_CANARY;
    ptr->type = type;
    ptr->parent = parent;
    ptr->left = NULL;
    ptr->parent = NULL;
    ptr->data = str;
    ptr->right_canary = TREENODE_CANARY;

    ASSERT_TREENODE_OK(ptr);
    *this_ = ptr;
    return TREENODE_OK;
}

void treenode_dtor(TreeNode *this_)
{
    if (!this_)
        return;

    treenode_dtor(this_->left);
    this_->left = NULL;
    treenode_dtor(this_->right);
    this_->right = NULL;
    free(this_->data);
    this_->data = NULL;
    this_->type = TREENODE_TYPE_NONE;
    free(this_);
}

int treenode_verify(TreeNode *this_)
{
    if (!this_)
       return TREENODE_ERR_BROKEN_PTR;
    if (!this_->data)
        return TREENODE_ERR_BROKEN_DATA;
    if (this_->left_canary != TREENODE_CANARY || this_->right_canary != TREENODE_CANARY)
        return TREENODE_ERR_BROKEN_CANARY;

    if (this_->left)
    {
        int left_ok = treenode_verify(this_->left);
        if (left_ok != TREENODE_OK)
            return left_ok;
    }
    if (this_->right)
    {
        int right_ok = treenode_verify(this_->right);
        if (right_ok != TREENODE_OK)
            return right_ok;
    }

    return TREENODE_OK;
}

void treenode_dump(TreeNode *this_, FILE *dumpster)
{
    assert(dumpster);
    if (!this_)
        return;

    static const char *BROKEN_DATA = "!!! BROKEN DATA !!!";
    const char *data = this_->data ? this_->data : BROKEN_DATA;

    fprintf(dumpster, "treenode_%p [label=\"TreeNode[%p]\n"
                      "{\n"
                      "    left_canary = 0x%X;\n"
                      "    parent = %p;\n"
                      "    type = %d;\n"
                      "    data = \'%s\';\n"
                      "    right_canary = 0x%X;\n"
                      "}\"]\n", this_, this_,
                      this_->left_canary,
                      this_->parent,
                      this_->type,
                      data,
                      this_->right_canary);

    treenode_dump(this_->left, dumpster);
    treenode_dump(this_->right, dumpster);

    if (this_->left)
        fprintf(dumpster, "treenode_%p -> treenode_%p\n", this_, this_->left);
    if (this_->right)
        fprintf(dumpster, "treenode_%p -> treenode_%p\n", this_, this_->right);
}

void treenode_attach_left(TreeNode *this_, TreeNode *child)
{
    ASSERT_TREENODE_OK(this_);
    ASSERT_TREENODE_OK(child);

    if (this_->left)
    {
        treenode_dtor(this_->left);
        this_->left = NULL;
    }
    this_->left = child;
    child->parent = this_;
    ASSERT_TREENODE_OK(this_);
}

void treenode_attach_right(TreeNode *this_, TreeNode *child)
{
    ASSERT_TREENODE_OK(this_);
    ASSERT_TREENODE_OK(child);

    if (this_->right)
    {
        treenode_dtor(this_->right);
        this_->right = NULL;
    }
    this_->right = child;
    child->parent = this_;
    ASSERT_TREENODE_OK(this_);
}
