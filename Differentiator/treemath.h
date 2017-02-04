/*!
 * Tree math.
 */
#ifndef TREEMATH_H
#define TREEMATH_H 
#include "tree.h"

/*!
 * Node types.
 */
enum
{
    NODE_CONST = 'C',
    NODE_SUM   = '+',
    NODE_DIFF  = '-',
    NODE_MUL   = '*',
    NODE_DIV   = '/',
    NODE_POW   = '^',
    NODE_VAR   = 'x',
    NODE_SIN   = 'S',
    NODE_COS   = 'c',
    NODE_LN    = 'L'
}; 

/*! 
 * DSL for creating operator declarations.
 */
#define MAKE_OP_DECL(_name_, _op_) DTreeNode *_name_(DTreeNode *l, DTreeNode *r);

MAKE_OP_DECL(Sum, NODE_SUM);
MAKE_OP_DECL(Minus, NODE_DIFF);
MAKE_OP_DECL(Mul, NODE_MUL);
MAKE_OP_DECL(Div, NODE_DIV);
MAKE_OP_DECL(Pow, NODE_POW);

#undef MAKE_OP_DECL

/*!
 * DSL for creating functions.
 */
DTreeNode *Sin(DTreeNode *arg);
DTreeNode *Cos(DTreeNode *arg);
DTreeNode *Ln(DTreeNode *arg);

/*!
 * Make const
 */
#define CONST(n) DTreeNodeCtor(NODE_CONST, n)
/*!
 * Make var.
 */
#define VAR() DTreeNodeCtor(NODE_VAR, 0)

/*! 
 * Simplify
 */
int Simplify(DTreeNode *node, DTreeNode *root, FILE *file, const char *prefix);

#endif /* ifndef TREEMATH_H */
