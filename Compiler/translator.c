#include "translator.h"
#include "lexer.h"
#include <assert.h>

#define ASSERT_TRANSLATOR(_t_) { assert(_t_); assert(_t_->var_scope); }

#define TRANSLATE_BINARY_OP(_name_, _fun_) \
static int _name_(Translator *tr, const TreeNode *node) \
{ \
    ASSERT_TRANSLATOR(tr); \
    \
    int _result_ = translate(tr, node->left); \
    if (_result_ != ZCC_OK) \
        return _result_; \
    _result_ = translate(tr, node->right); \
    if (_result_ != ZCC_OK) \
        return _result_; \
    fprintf(stdout, #_fun_ "\n"); \
    return ZCC_OK;\
} 

TRANSLATE_BINARY_OP(translate_plus, add);
TRANSLATE_BINARY_OP(translate_minus, sub);
TRANSLATE_BINARY_OP(translate_mul, mul);
TRANSLATE_BINARY_OP(translate_div, div);
TRANSLATE_BINARY_OP(translate_and, and);
TRANSLATE_BINARY_OP(translate_or, or);
TRANSLATE_BINARY_OP(translate_aoe, aoe);
TRANSLATE_BINARY_OP(translate_boe, boe);
TRANSLATE_BINARY_OP(translate_eq, eq);
TRANSLATE_BINARY_OP(translate_blw, blw);
TRANSLATE_BINARY_OP(translate_abv, abv);

/*!
 * Translate variable.
 */
static int translate_var(Translator *tr, const TreeNode *node)
{
    ASSERT_TRANSLATOR(tr);
    assert(node->type == ZCC_TOKEN_ID);

    size_t *var = varmap_search(tr->var_scope, (const char *)node->data);
    if (!var)
    {
        fprintf(stderr, "Unknown identifier: %s.\n", (const char *)node->data);
        return ZCC_ERR_UNKNOWN_ID;
    }

    fprintf(stdout, "get %lu\n", *var);
    return ZCC_OK;
}

/*!
 * Translate constant.
 */
static int translate_const(TreeNode *node)
{
    assert(node);
    assert(node->type == ZCC_TOKEN_CONST);

    fprintf(stdout, "push %lg\n", *(double *)node->data);
    return ZCC_OK;
}

/*!
 * Translate assignment.
 */
static int translate_assign(Translator *tr, TreeNode *node)
{
    ASSERT_TRANSLATOR(tr);
    assert(node);
    assert(node->type == ZCC_TOKEN_ASSIGN);

    size_t *var = varmap_search(tr->var_scope, (const char *)node->left->data);
    if (!var)
    {
        fprintf(stderr, "Unknown identifier: %s.\n", (const char *)node->left->data);
        return ZCC_ERR_UNKNOWN_ID;
    }
    translate(tr, node->right);
    fprintf(stdout, "set %lu\n", *var);
    return ZCC_OK;
}

static int translate_func(Translator *tr, TreeNode *node)
{
    assert(tr);
    assert(node);
    assert(node->left); assert(node->right);

    Func *func = funcmap_search(tr->funcmap, (const char *)node->left->data);
    assert(func);
    tr->var_scope = (VarMap *)node->data;
    fprintf(stdout, "%s:\n", (const char *)node->left->data);
    fprintf(stdout, "stackframe %lu\n", func->args);
    for (size_t i = 0; i < func->locals; i++)
        fprintf(stdout, "push 0\n");
    translate(tr, node->right);
    fprintf(stdout, "push 0\nret\n");

    return ZCC_OK;
}

/*!
 * Translate return
 */
static int translate_ret(Translator *tr, TreeNode *node)
{
    assert(tr);
    assert(node);
    
    translate(tr, node->left);
    fprintf(stdout, "ret\n");
    return ZCC_OK;
}

/*!
 * Translate operator.
 */
static int translate_op(Translator *tr, TreeNode *node)
{
    assert(tr);
    assert(node);

    translate(tr, node->left);
    if (node->right)
        translate(tr, node->right);
    return ZCC_OK;
}

/*!
 * Translate if.
 */
static int translate_if(Translator *tr, TreeNode *node)
{
    assert(tr);
    assert(node);

    TreeNode *left = node->left;
    TreeNode *right = node->right;
    size_t id = tr->if_count;
    tr->if_count++;
    translate(tr, left->left);
    fprintf(stdout, "jmpc if_%lu: \n", id);
    fprintf(stdout, "jmp else_%lu:\n", id);
    fprintf(stdout, "if_%lu:\n", id);
    translate(tr, right->left);
    fprintf(stdout, "else_%lu:\n", id);
    if (left->right)
        translate(tr, left->right);
    
    if (right->right)
        translate(tr, right->right);
    return ZCC_OK;
}

/*!
 * Translate while.
 */
static int translate_while(Translator *tr, TreeNode *node)
{
    assert(tr);
    assert(node);

    size_t id = tr->while_count;
    fprintf(stdout, "while_%lu:\n", id);
    translate(tr, node->left);
    fprintf(stdout, "jmpc while_%lu_body: \n", id);
    fprintf(stdout, "jmp while_%lu_exit:\n", id);
    fprintf(stdout, "while_%lu_body:\n", id);
    tr->while_count++;
    translate(tr, node->right);
    fprintf(stdout, "jmp while_%lu:\n", id);
    fprintf(stdout, "while_%lu_exit:\n", id);

    return ZCC_OK;
}

static int translate_fn_call(Translator *tr, TreeNode *node)
{
    assert(tr);
    assert(node);

    Func *func;
    if (!(func = funcmap_search(tr->funcmap, (const char *)node->data)))
    {
        fprintf(stderr, "Unknown function: %s\n", (const char *)node->data);
        return ZCC_ERR_UNKNOWN_ID;
    }

    if (node->right)
        translate(tr, node->right);
    fprintf(stdout, "call %s:\n", (const char *)node->data);

    return ZCC_OK;
}

int translate(Translator *tr, TreeNode *root)
{
    assert(tr);
    assert(root);

    switch (root->type)
    {
        case ZCC_TOKEN_CONST:
            translate_const(root);
            break;
        case ZCC_TOKEN_ID:
            translate_var(tr, root);
            break;
        case ZCC_TOKEN_EQ:
            translate_eq(tr, root);
            break;
        case ZCC_TOKEN_PLUS:
            translate_plus(tr, root);
            break;
        case ZCC_TOKEN_MINUS:
            translate_minus(tr, root);
            break;
        case ZCC_TOKEN_MUL:
            translate_mul(tr, root);
            break;
        case ZCC_TOKEN_DIV:
            translate_div(tr, root);
            break;
        case ZCC_TOKEN_AND:
            translate_and(tr, root);
            break;
        case ZCC_TOKEN_OR:
            translate_or(tr, root);
            break;
        case ZCC_TOKEN_ABOVE_OR_EQ:
            translate_aoe(tr, root);
            break;
        case ZCC_TOKEN_BELOW_OR_EQ:
            translate_boe(tr, root);
            break;
        case ZCC_TOKEN_ABOVE:
            translate_abv(tr, root);
            break;
        case ZCC_TOKEN_BELOW:
            translate_blw(tr, root);
            break;
        case ZCC_TOKEN_ASSIGN:
            translate_assign(tr, root);
            break;
        case ZCC_TOKEN_SEMICOLON:
            translate_op(tr, root);
            break;
        case ZCC_TOKEN_FN:
            translate_func(tr, root);
            break;
        case ZCC_TOKEN_IF:
            translate_if(tr, root);
            break;
        case ZCC_TOKEN_WHILE:
            translate_while(tr, root);
            break;
        case ZCC_TOKEN_RET:
            translate_ret(tr, root);
            break;
        case ZCC_TOKEN_FN_CALL:
            translate_fn_call(tr, root);
            break;
        default:
            break;
    }

    return ZCC_OK;
}
