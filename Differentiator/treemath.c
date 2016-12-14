#include "treemath.h"
#include <assert.h>
#include <math.h>
#include "common.h"
#include "treetex.h"

/*!
 * DSL for creating operators.
 */
#define MAKE_OP(_name_, _op_) DTreeNode *_name_(DTreeNode *l, DTreeNode *r) \
{ \
    assert(l); \
    assert(r); \
    \
    if (!l || !r) \
        return NULL; \
    DTreeNode *result = DTreeNodeCtor(_op_, 0); \
    if (!result) \
        return NULL; \
    DTreeNodeAttachLeft(result, l); \
    DTreeNodeAttachRight(result, r); \
    \
    return result; \
}

MAKE_OP(Sum, NODE_SUM);
MAKE_OP(Minus, NODE_DIFF);
MAKE_OP(Mul, NODE_MUL);
MAKE_OP(Div, NODE_DIV);
MAKE_OP(Pow, NODE_POW);

#define IS_ZERO(_t_) (_t_->type == NODE_CONST && CompareDouble(_t_->data, 0))
#define IS_ONE(_t_) (_t_->type == NODE_CONST && CompareDouble(_t_->data, 1))
#define KILL_CHILDREN(_t_) \
{\
    DTreeNodeDtor(_t_->left);\
    DTreeNodeDtor(_t_->right);\
    _t_->left = NULL;\
    _t_->right = NULL; \
}

static void NodeSurfCopy(DTreeNode *t1, DTreeNode *t2)
{
    assert(t1);
    assert(t2);

    t1->type = t2->type;
    t1->data = t2->data;
    t1->left = t2->left;
    t1->right = t2->right;
    if (t1->left)
        t1->left->parent = t1;
    if (t1->right)
        t1->right->parent = t1;
    t2->left = t2->right = NULL;
    DTreeNodeDtor(t2);
}

int Simplify(DTreeNode *node, DTreeNode *root, FILE *file, const char *prefix)
{
    assert(node);
    assert(root);
    assert(prefix);

    int result = DIFF_OK;
    bool changed = false;

    if (node->left)
    {
        if ((result = Simplify(node->left, root, file, prefix)) != DIFF_OK)
            return result;
    }
    if (node->right)
    {
        if ((result = Simplify(node->right, root, file, prefix)) != DIFF_OK)
            return result;
    }

    switch (node->type)
    {
        case NODE_SUM:
            if (node->left->type == NODE_CONST && node->right->type == NODE_CONST)
            {
                node->type = NODE_CONST;
                node->data = node->left->data + node->right->data;
                KILL_CHILDREN(node);
                changed = true;
            }
            else if (IS_ZERO(node->left))
            {
                DTreeNodeDtor(node->left);
                NodeSurfCopy(node, node->right);
                changed = true;
            }
            else if (IS_ZERO(node->right))
            {
                DTreeNodeDtor(node->right);
                NodeSurfCopy(node, node->left);
                changed = true;
            }
            break;
        case NODE_DIFF:
            if (node->left->type == NODE_CONST && node->right->type == NODE_CONST)
            {
                node->type = NODE_CONST;
                node->data = node->left->data - node->right->data;
                KILL_CHILDREN(node);
                changed = true;
            }
            else if (IS_ZERO(node->left))
            {
                DTreeNodeDtor(node->left);
                NodeSurfCopy(node, Mul(CONST(-1), node->right));
                changed = true;
            }
            else if (IS_ZERO(node->right))
            {
                DTreeNodeDtor(node->right);
                NodeSurfCopy(node, node->left);
                changed = true;
            }
            break;
        case NODE_MUL:
            if (node->left->type == NODE_CONST && node->right->type == NODE_CONST)
            {
                node->type = NODE_CONST;
                node->data = node->left->data * node->right->data;
                KILL_CHILDREN(node);
                changed = true;
            }
            else if (IS_ZERO(node->left) || IS_ZERO(node->right))
            {
                node->type = NODE_CONST;
                node->data = 0;
                KILL_CHILDREN(node);
                changed = true;
            }
            else if (IS_ONE(node->right))
            {
                DTreeNodeDtor(node->right);
                NodeSurfCopy(node, node->left);
                changed = true;
            }
            else if (IS_ONE(node->left))
            {
                DTreeNodeDtor(node->left);
                NodeSurfCopy(node, node->right);
                changed = true;
            }
            break;
        case NODE_DIV:
            if (IS_ZERO(node->right))
                return DIFF_ERR_DIV_BY_ZERO;

            if (node->left->type == NODE_CONST && node->right->type == NODE_CONST)
            {
                node->type = NODE_CONST;
                node->data = node->left->data / node->right->data;
                KILL_CHILDREN(node);
                changed = true;
            }
            else if (IS_ZERO(node->left))
            {
                node->type = NODE_CONST;
                node->data = 0;
                KILL_CHILDREN(node);
                changed = true;
            }
            else if (IS_ONE(node->right))
            {
                DTreeNodeDtor(node->right);
                NodeSurfCopy(node, node->left);
                changed = true;
            }
            break;
        case NODE_POW:
            if (node->left->type == NODE_CONST && node->right->type == NODE_CONST)
            {
                if (node->left->data <= 0 && node->left->data <= 0)
                    return DIFF_DOMAIN_ERROR;
                
                node->type = NODE_CONST;
                node->data = pow(node->left->data, node->right->data);
                KILL_CHILDREN(node);
                changed = true;
            }
            else if (IS_ONE(node->right))
            {
                DTreeNodeDtor(node->right);
                NodeSurfCopy(node, node->left);
                changed = true;
            }
            else if (IS_ZERO(node->right))
            {
                node->type = NODE_CONST;
                node->data = 1;
                KILL_CHILDREN(node);
                changed = true;
            }
            break;
        case NODE_SIN:
            if (node->right->type == NODE_CONST)
            {
                node->type = NODE_CONST;
                node->data = sin(node->right->data);
                KILL_CHILDREN(node);
                changed = true;
            }
            break;
        case NODE_COS:
            if (node->right->type == NODE_CONST)
            {
                node->type = NODE_CONST;
                node->data = cos(node->right->data);
                KILL_CHILDREN(node);
                changed = true;
            }
            break;
        case NODE_LN:
            if (IS_ONE(node->right) || (node->right->type == NODE_CONST && node->right->data < 0))
                return DIFF_DOMAIN_ERROR;
            if (node->right->type == NODE_CONST)
            {
                node->type = NODE_CONST;
                node->data = log(node->right->data);
                KILL_CHILDREN(node);
                changed = true;
            }
            break;
        default:
            break;
    } 

    if (changed)
        PrintTeXEquation(file, root, prefix);

    return DIFF_OK;
}

#define MAKE_FN(_name_, _c_) DTreeNode *_name_(DTreeNode *arg) \
{ \
    assert(arg); \
    if (!arg) \
        return NULL; \
    \
    DTreeNode *node = DTreeNodeCtor(_c_, 0); \
    if (!node) \
        return NULL; \
    DTreeNodeAttachRight(node, arg); \
    \
    return node; \
}

MAKE_FN(Sin, NODE_SIN);
MAKE_FN(Cos, NODE_COS);
MAKE_FN(Ln, NODE_LN);
