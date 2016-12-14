#include "diff.h"
#include "treemath.h"
#include <assert.h>

#define GET_OR_RET_NULL(_node_, _action_) { _node_ = _action_; if (!_node_) return NULL; }
DTreeNode *Diff(DTreeNode *f) {
    assert(f);
    if (!f)
        return NULL;

    DTreeNode *numerator = NULL;
    DTreeNode *denominator = NULL;
    DTreeNode *l = NULL, *r = NULL;
    DTreeNode *cl = NULL, *cr = NULL;
    DTreeNode *mul = NULL, *index = NULL;

#define COPY(n) DTreeNodeCopy(n)
    switch (f->type)
    {
        case NODE_CONST:
            return CONST(0);
        case NODE_VAR:
            return CONST(1);
        case NODE_SIN:
            return Mul(Cos(COPY(f->right)), Diff(f->right));
        case NODE_COS:
            return Mul(Mul(CONST(-1), Sin(COPY(f->right))), Diff(f->right));
        case NODE_LN:
            return Mul(Div(CONST(1), COPY(f->right)), Diff(f->right));
        case NODE_SUM:
            assert(f->left);
            assert(f->right);

            GET_OR_RET_NULL(l, Diff(f->left));
            GET_OR_RET_NULL(r, Diff(f->right));
            return Sum(l, r);
        case NODE_DIFF:
            assert(f->left);
            assert(f->right);

            GET_OR_RET_NULL(l, Diff(f->left));
            GET_OR_RET_NULL(r, Diff(f->right));
            return Minus(l, r);
        case NODE_MUL:
            assert(f->left);
            assert(f->right);

            GET_OR_RET_NULL(l, Diff(f->left));
            GET_OR_RET_NULL(r, Diff(f->right));

            GET_OR_RET_NULL(cl, DTreeNodeCopy(f->left));
            GET_OR_RET_NULL(cr, DTreeNodeCopy(f->right));
            return Sum(Mul(l, cr), Mul(cl, r));
        case NODE_DIV:
            assert(f->left);
            assert(f->right);

            numerator = Minus(Mul(Diff(f->left), COPY(f->right)),
                                         Mul(Diff(f->right), COPY(f->left)));
            denominator = Mul(COPY(f->right), COPY(f->right));
            return Div(numerator, denominator);
        case NODE_POW:
            assert(f->left);
            assert(f->right);

            if (f->right->type == NODE_CONST)
            {
                mul   = CONST(f->right->data);
                index = DTreeNodeCtor(NODE_CONST, f->right->data - 1);
            
                return Mul(Mul(mul, Pow(COPY(f->left), index)), Diff(f->left));
            }
            else if (f->left->type == NODE_CONST)
            {
                return Mul(Mul(COPY(f), Ln(COPY(f->left))), Diff(f->right));
            }
            else
                return NULL;
        default:
            return NULL;
    }
#undef COPY
}
