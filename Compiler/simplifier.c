#include "simplifier.h"
#include "lexer.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define LEFT (node->left)
#define RIGHT (node->right)
#define KILL_CHILDREN() { treenode_del(node->left); treenode_del(node->right); node->left = node->right = NULL; }
#define IS_ZERO(t) (t->type == ZCC_TOKEN_CONST && fabs(*(double *)t->data) <= 1E10-6)
int simplify(TreeNode *node)
{
    if (!node)
        return ZCC_OK;

    int result = ZCC_OK;
    if ((result = simplify(LEFT)) != ZCC_OK) 
        return result;
    if ((result = simplify(RIGHT)) != ZCC_OK) 
        return result;

    switch (node->type)
    {
        case ZCC_TOKEN_PLUS:
            assert(LEFT);
            assert(RIGHT);

            if (LEFT->type == ZCC_TOKEN_CONST && RIGHT->type == ZCC_TOKEN_CONST)
            {
                double *data = (double *)LEFT->data;
                *data = *(double *)LEFT->data + *(double *)RIGHT->data;
                node->type = ZCC_TOKEN_CONST;
                node->data = data; 
                KILL_CHILDREN();
            }
            return ZCC_OK;
        case ZCC_TOKEN_MINUS:
            assert(LEFT);
            assert(RIGHT);

            if (LEFT->type == ZCC_TOKEN_CONST && RIGHT->type == ZCC_TOKEN_CONST)
            {
                double *data = (double *)LEFT->data;
                *data = *(double *)LEFT->data - *(double *)RIGHT->data;
                node->type = ZCC_TOKEN_CONST;
                node->data = data; 
                KILL_CHILDREN();
            }
            return ZCC_OK;
        case ZCC_TOKEN_MUL:
            assert(LEFT);
            assert(RIGHT);

            if (LEFT->type == ZCC_TOKEN_CONST && RIGHT->type == ZCC_TOKEN_CONST)
            {
                double *data = (double *)LEFT->data;
                *data = *(double *)LEFT->data * *(double *)RIGHT->data;
                node->type = ZCC_TOKEN_CONST;
                node->data = data; 
                KILL_CHILDREN();
            }
            return ZCC_OK;
        case ZCC_TOKEN_DIV:
            assert(LEFT);
            assert(RIGHT);

            if (IS_ZERO(RIGHT))
                return ZCC_ERR_DIVISION_BY_ZERO;

            if (LEFT->type == ZCC_TOKEN_CONST && RIGHT->type == ZCC_TOKEN_CONST)
            {
                double *data = (double *)LEFT->data;
                *data = *(double *)LEFT->data / *(double *)RIGHT->data;
                node->type = ZCC_TOKEN_CONST;
                node->data = data; 
                KILL_CHILDREN();
            }
            return ZCC_OK;
        default:
            return ZCC_OK;
    }

    return ZCC_OK;
}
