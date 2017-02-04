#include "tree.h"
#include "lexer.h"
#include "hashmap.h"
#include <stdlib.h>
#include <assert.h>

TreeNode *treenode_new(int type, void *data)
{
    TreeNode *this_ = (TreeNode *)calloc(1, sizeof(TreeNode));
    if (!this_)
        return NULL; 
    this_->type  = type;
    this_->left  = NULL;
    this_->right = NULL;
    this_->data  = data;

    return this_;
}

int treenode_del(TreeNode *this_)
{
    if (!this_)
        return ZCC_OK;

    if (this_->type == ZCC_TOKEN_FN)
        varmap_del((VarMap *)this_->data);
    treenode_del(this_->left);
    treenode_del(this_->right);
    this_->left  = NULL;
    this_->right = NULL;
    this_->data  = NULL;
    this_->type  = ZCC_TOKEN_EMPTY;
    free(this_);

    return ZCC_OK;
}

void treenode_dump(const TreeNode *this_, FILE *dumpster)
{
    assert(dumpster);
    if (!this_)
        return;

    fprintf(dumpster,
            "TreeNode_%p [label=\"TreeNode[%p]\\l"
            "{\\l"
            "    parent = %p;\\l"
            "    type = '",
            this_,
            this_,
            this_->parent);
    switch (this_->type)
    {
        case ZCC_TOKEN_ID:
            fprintf(dumpster, "ID");
            break;
        case ZCC_TOKEN_CONST:
            fprintf(dumpster, "CONST");
            break;
        case ZCC_TOKEN_PLUS:
            fprintf(dumpster, "+");
            break;
        case ZCC_TOKEN_MINUS:
            fprintf(dumpster, "-");
            break;
        case ZCC_TOKEN_MUL:
            fprintf(dumpster, "*");
            break;
        case ZCC_TOKEN_DIV:
            fprintf(dumpster, "/");
            break;
        case ZCC_TOKEN_ASSIGN:
            fprintf(dumpster, "=");
            break;
        case ZCC_TOKEN_OR:
            fprintf(dumpster, "or");
            break;
        case ZCC_TOKEN_AND:
            fprintf(dumpster, "and");
            break;
        case ZCC_TOKEN_SEMICOLON:
            fprintf(dumpster, ";");
            break;
        case ZCC_TOKEN_IF:
            fprintf(dumpster, "if");
            break;
        case ZCC_TOKEN_WHILE:
            fprintf(dumpster, "while");
            break;
        case ZCC_TOKEN_FN:
            fprintf(dumpster, "fn");
            break;
        case ZCC_TOKEN_ABOVE_OR_EQ:
            fprintf(dumpster, ">=");
            break;
        case ZCC_TOKEN_BELOW_OR_EQ:
            fprintf(dumpster, "<=");
            break;
        case ZCC_TOKEN_EQ:
            fprintf(dumpster, "==");
            break;
        case ZCC_TOKEN_ABOVE:
            fprintf(dumpster, ">");
            break;
        case ZCC_TOKEN_BELOW:
            fprintf(dumpster, "<");
            break;
        case ZCC_TOKEN_RET:
            fprintf(dumpster, "ret");
            break;
        case ZCC_TOKEN_FN_CALL:
            fprintf(dumpster, "fn call");
            break;
        default:
            fprintf(dumpster, "%d", this_->type);
            break;
    }

    fprintf(dumpster, "';\\l");

    switch (this_->type)
    {
        case ZCC_TOKEN_ID:
            fprintf(dumpster, "    data = %s;\\l", (char_t *)this_->data);
            break;
        case ZCC_TOKEN_CONST:
            fprintf(dumpster, "    data = %lg;\\l", *(double *)this_->data);
            break;
        default:
            break;
    }
    
    fprintf(dumpster, "}\\l\"]");

    treenode_dump(this_->left, dumpster);
    treenode_dump(this_->right, dumpster);

    if (!this_->left)
    {
        fprintf(dumpster, "TreeNode_%p_left [label=\"(nil)\"]\n", this_);
        fprintf(dumpster, "TreeNode_%p -> TreeNode_%p_left\n", this_, this_);
    }
    else
        fprintf(dumpster, "TreeNode_%p -> TreeNode_%p\n", this_, this_->left);

    if (!this_->right)
    {
        fprintf(dumpster, "TreeNode_%p_right [label=\"(nil)\"]\n", this_);
        fprintf(dumpster, "TreeNode_%p -> TreeNode_%p_right\n", this_, this_);
    }
    else
        fprintf(dumpster, "TreeNode_%p -> TreeNode_%p\n", this_, this_->right);
}

void treenode_attachl(TreeNode *this_, TreeNode *child)
{
    assert(this_);
    assert(child);

    if (this_->left)
    {
        treenode_del(this_->left);
        this_->left = NULL;
    }
    this_->left   = child;
    child->parent = this_;
}

void treenode_attachr(TreeNode *this_, TreeNode *child)
{
    assert(this_);
    assert(child);

    if (this_->right)
    {
        treenode_del(this_->right);
        this_->right = NULL;
    }
    this_->right  = child;
    child->parent = this_;
}

TreeNode *treenode_detach(TreeNode *this_)
{
    assert(this_);

    if (!this_->parent)
        return this_;

    if (this_->parent->left == this_)
        this_->parent->left = NULL;
    else if (this_->parent->right == this_)
        this_->parent->right = NULL;
    this_->parent = NULL;
    return this_;
}

TreeNode *treenode_copy(const TreeNode *this_)
{
    assert(this_);

    TreeNode *root = treenode_new(this_->type, this_->data);
    if (!root)
        return NULL;
    if (this_->left)
    {
        TreeNode *left = treenode_new(this_->type, this_->data);
        if (!left)
        {
            treenode_del(root);
            return NULL;
        }
    }
    if (this_->right)
    {
        TreeNode *right = treenode_new(this_->type, this_->data);
        if (!right)
        {
            treenode_del(root);
            return NULL;
        }
    }

    return root;
}
