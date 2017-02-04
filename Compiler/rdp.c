#include "rdp.h"
#include "hashmap.h"
#include <assert.h>

/*!
 * Set error code if it hasn't already been set.
 */
#define SET_ERR(_code_) { if (rdp_error != ZCC_OK) { rdp_error = _code_; rdp_error_token = **t; } }

/*!
 * Error code.
 */
static int rdp_error = ZCC_OK;
static Token *rdp_error_token = NULL;

/*!
 * Get const.
 */
static TreeNode *get_n(Token ***t);
/*!
 * Get ID.
 */
static TreeNode *get_id(Token ***t);
/*!
 * Get operator.
 */
static TreeNode *get_op(Token ***t);
/*!
 * Get assignment.
 */
static TreeNode *get_a(Token ***t);
/*!
 * Get E.
 */
static TreeNode *get_e(Token ***t);
/*!
 * Get conjunction.
 */
static TreeNode *get_c(Token ***t);
/*!
 * Get disjunction.
 */
static TreeNode *get_d(Token ***t);
/*!
 * Get equal.
 */
static TreeNode *get_eq(Token ***t);
/*!
 * Get comparison.
 */
static TreeNode *get_com(Token ***t);
/*!
 * Get T.
 */
static TreeNode *get_t(Token ***t);
/*!
 * Get P.
 */
static TreeNode *get_p(Token ***t);
/*!
 * Get semicolon (operator split).
 */
static TreeNode *get_sp(Token ***t);
/*!
 * Get function call.
 */
static TreeNode *get_fn_call(Token ***t);
/*!
 * Get function
 */
static TreeNode *get_fn(Token ***t, FuncMap *funcmap);
/*!
 * Get function
 */
static TreeNode *get_prg(Token ***t, FuncMap *funcmap);
/*!
 * Get operand.
 */
static TreeNode *get_x(Token ***t);

#define ASSERT_T() { assert(t); assert(*t); assert(**t); }
#define TOK (**t)
#define SHIFT_TOK() (*t)++

#define GET_BINARY_OPERATOR(_name_, _next_, _cond_) \
TreeNode *_name_(Token ***t) \
{  \
    ASSERT_T(); \
    \
    TreeNode *result = _next_(t); \
    if (!result) \
        return NULL; \
    \
    while (_cond_) \
    { \
        int op = TOK->type; \
        SHIFT_TOK(); \
        \
        TreeNode *result2 = _next_(t); \
        if (!result2) \
        { \
            treenode_del(result); \
            return NULL; \
        } \
        \
        TreeNode *r = treenode_new(op, NULL); \
        if (!r) \
        { \
            SET_ERR(ZCC_ERR_ALLOC); \
            treenode_del(result); \
            treenode_del(result2); \
            return NULL; \
        } \
        treenode_attachl(r, result); \
        treenode_attachr(r, result2); \
        result = r; \
    } \
    \
    return result; \
}

GET_BINARY_OPERATOR(get_d, get_c, TOK->type == ZCC_TOKEN_OR)
GET_BINARY_OPERATOR(get_c, get_eq, TOK->type == ZCC_TOKEN_AND)
GET_BINARY_OPERATOR(get_eq, get_com, TOK->type == ZCC_TOKEN_EQ)
GET_BINARY_OPERATOR(get_com, get_e, TOK->type == ZCC_TOKEN_ABOVE_OR_EQ || 
                                    TOK->type == ZCC_TOKEN_BELOW_OR_EQ ||
                                    TOK->type == ZCC_TOKEN_ABOVE ||
                                    TOK->type == ZCC_TOKEN_BELOW)
GET_BINARY_OPERATOR(get_e, get_t, TOK->type == ZCC_TOKEN_PLUS || TOK->type == ZCC_TOKEN_MINUS)
GET_BINARY_OPERATOR(get_t, get_p, TOK->type == ZCC_TOKEN_MUL || TOK->type == ZCC_TOKEN_DIV)

TreeNode *get_x(Token ***t)
{
    ASSERT_T();
    
    TreeNode *result = get_n(t);
    if (result)
        return result;
    
    return get_id(t);
}

TreeNode *get_n(Token ***t)
{
    ASSERT_T();

    if (TOK->type == ZCC_TOKEN_CONST)
    {
        TreeNode *result = treenode_new(ZCC_TOKEN_CONST, TOK->data);
        assert(TOK->data);
        SHIFT_TOK();
        return result;
    }
    else
    {
        return NULL;
    }
}

#define TOK_NEXT (*(*t + 1))
#define GET_ID_TOKEN() \
id = treenode_new(ZCC_TOKEN_ID, TOK->data); \
SHIFT_TOK(); \
if (!id) \
    SET_ERR(ZCC_ERR_ALLOC);

TreeNode *get_id(Token ***t)
{
    ASSERT_T();

    if (TOK->type == ZCC_TOKEN_ID)
    {
        if (TOK_NEXT->type == ZCC_TOKEN_BRACKET_RND_OPN)
        {
            TreeNode *call = get_fn_call(t);
            return call;
        }
        else 
        {
            TreeNode *id = NULL;
            GET_ID_TOKEN();
            return id;
        }
    }

    rdp_error = ZCC_ERR_EXPECTED_ID;
    return NULL;
}

TreeNode *get_fn_call(Token ***t)
{
    ASSERT_T();
   
    TreeNode *id = NULL;
    GET_ID_TOKEN(); 
    if (!id)
        return NULL;
    TreeNode *prev = id;

    SHIFT_TOK(); // skip open bracket
    while (TOK->type != ZCC_TOKEN_BRACKET_RND_CLS && TOK->type != ZCC_TOKEN_EMPTY)    
    {
        TreeNode *d = get_d(t);
        if (!d)
        {
            SET_ERR(ZCC_ERR_EXPECTED_EXPR);
            treenode_del(id);
            return NULL;
        }
        TreeNode *sp = treenode_new(ZCC_TOKEN_SEMICOLON, NULL);
        if (!sp)
        {
            treenode_del(id);
            treenode_del(d);
            SET_ERR(ZCC_ERR_ALLOC);
            return NULL;
        }
        treenode_attachl(sp, d);
        treenode_attachr(prev, sp);
        prev = sp;

        if (TOK->type == ZCC_TOKEN_COMMA)
            SHIFT_TOK();
        else
            break;
    }

    if (TOK->type != ZCC_TOKEN_BRACKET_RND_CLS)
    {
        SET_ERR(ZCC_ERR_EXPECTED_BRACKET);
        treenode_del(id);
        return NULL;
    }
    SHIFT_TOK();
    
    id->type = ZCC_TOKEN_FN_CALL;
    return id;
}

TreeNode *get_a(Token ***t)
{
    ASSERT_T();

    TreeNode *var = get_id(t);
    if (!var)
        return NULL;

    if (TOK->type != ZCC_TOKEN_ASSIGN)
    {
        SET_ERR(ZCC_ERR_EXPECTED_ASSIGN);
        treenode_del(var);
        return NULL;
    }
    SHIFT_TOK();

    TreeNode *expr = get_d(t);
    if (!expr)
    {
        treenode_del(var);
        return NULL;
    }

    TreeNode *assign = treenode_new(ZCC_TOKEN_ASSIGN, NULL);
    if (!assign)
    {
        SET_ERR(ZCC_ERR_ALLOC);
        treenode_del(var);
        treenode_del(expr);
        return NULL;
    }
    treenode_attachl(assign, var);
    treenode_attachr(assign, expr);

    return assign;
}

/*!
 * Get if.
 */
static TreeNode *get_if(Token ***t)
{
    assert(t);
    
    if (TOK->type != ZCC_TOKEN_IF)
        return NULL;

    SHIFT_TOK();
    if (TOK->type != ZCC_TOKEN_BRACKET_RND_OPN)
    {
        SET_ERR(ZCC_ERR_EXPECTED_BRACKET);
        return NULL;
    }
    SHIFT_TOK();

    TreeNode *d = get_d(t); 
    if (!d)
        return NULL;

    if (TOK->type != ZCC_TOKEN_BRACKET_RND_CLS)
    {
        SET_ERR(ZCC_ERR_EXPECTED_BRACKET);
        treenode_del(d);
        return NULL;
    }
    SHIFT_TOK();

    TreeNode *op = get_op(t);
    if (!op)
    {
        treenode_del(d);
        return NULL;
    }
    
    TreeNode *else_op = NULL;
    if (TOK->type == ZCC_TOKEN_ELSE)
    {
        SHIFT_TOK();
        else_op = get_op(t);
        if (!else_op)
        {
            treenode_del(d);
            treenode_del(op);
            return NULL;
        }
    }

    TreeNode *ifnode = treenode_new(ZCC_TOKEN_IF, NULL);
    if (!ifnode)
    {
        SET_ERR(ZCC_ERR_ALLOC);
        treenode_del(else_op);
        treenode_del(d);
        treenode_del(op);
        return NULL;
    }
    TreeNode *condnode = treenode_new(ZCC_TOKEN_SEMICOLON, NULL);
    if (!condnode)
    {
        SET_ERR(ZCC_ERR_ALLOC);
        treenode_del(ifnode);
        treenode_del(else_op);
        treenode_del(d);
        treenode_del(op);
        return NULL;
    }
    treenode_attachl(condnode, d);
    if (else_op)
        treenode_attachr(condnode, else_op);
    treenode_attachl(ifnode, condnode);
    treenode_attachr(ifnode, op);
    TreeNode *sp = treenode_new(ZCC_TOKEN_SEMICOLON, NULL);
    if (!condnode)
    {
        SET_ERR(ZCC_ERR_ALLOC);
        treenode_del(ifnode);
        return NULL;
    }

    treenode_attachl(sp, ifnode);

    return sp;
}

/*!
 * Get while loop.
 */
static TreeNode *get_while(Token ***t)
{
    ASSERT_T();

    if (TOK->type != ZCC_TOKEN_WHILE)
        return NULL;
    SHIFT_TOK();

    if (TOK->type != ZCC_TOKEN_BRACKET_RND_OPN)
    {
        SET_ERR(ZCC_ERR_EXPECTED_BRACKET);
        return NULL;
    }
    SHIFT_TOK();

    TreeNode *d = get_d(t); 
    if (!d)
        return NULL;

    if (TOK->type != ZCC_TOKEN_BRACKET_RND_CLS)
    {
        SET_ERR(ZCC_ERR_EXPECTED_BRACKET);
        treenode_del(d);
        return NULL;
    }
    SHIFT_TOK();

    TreeNode *op = get_op(t);
    if (!op)
    {
        treenode_del(d);
        return NULL;
    }
    
    TreeNode *whilenode = treenode_new(ZCC_TOKEN_WHILE, NULL);
    if (!whilenode)
    {
        SET_ERR(ZCC_ERR_ALLOC);
        treenode_del(d);
        treenode_del(op);
        return NULL;
    }

    treenode_attachl(whilenode, d);
    treenode_attachr(whilenode, op);
    TreeNode *sp = treenode_new(ZCC_TOKEN_SEMICOLON, NULL);
    if (!sp)
    {
        SET_ERR(ZCC_ERR_ALLOC);
        treenode_del(whilenode);
    }
    treenode_attachl(sp, whilenode);
    
    return sp;
}

/*!
 * Get return operator.
 */
static TreeNode *get_ret(Token ***t)
{
    ASSERT_T();
    SHIFT_TOK();
    TreeNode *r = treenode_new(ZCC_TOKEN_RET, NULL);
    if (!r)
    {
        SET_ERR(ZCC_ERR_ALLOC);
        return NULL;
    }
    TreeNode *d = get_d(t);
    if (!d)
    {
        treenode_del(d);
        return NULL;
    }

    TreeNode *sp = get_sp(t);
    if (!sp)
    {
        treenode_del(d);
        treenode_del(sp);
        return NULL;
    }
    treenode_attachl(r, d);
    treenode_attachl(sp, r);
    return sp;
}

/*!
 * Get operator that starts with an id.
 */
static TreeNode *get_op_id(Token ***t)
{
    if (TOK_NEXT->type == ZCC_TOKEN_ASSIGN) 
    {
        TreeNode *a = get_a(t);
        if (!a)
            return NULL;
        TreeNode *sp = get_sp(t);
        if (!sp)
        {
            treenode_del(a);
            return NULL;
        }
        treenode_attachl(sp, a);

        return sp;
    }    
    else
    {
        TreeNode *call = get_fn_call(t);
        if (!call)
            return NULL;
        TreeNode *sp = get_sp(t);
        if (!sp)
        {
            treenode_del(call);
            return NULL;
        }
        treenode_attachl(sp, call);
        return sp;
    }
}

TreeNode *get_op(Token ***t)
{
    ASSERT_T(); 

    if (TOK->type != ZCC_TOKEN_BRACKET_CRL_OPN)
    {
        switch (TOK->type)
        {
        case ZCC_TOKEN_IF:
            return get_if(t);
        case ZCC_TOKEN_WHILE:
            return get_while(t);
        case ZCC_TOKEN_RET:
            return get_ret(t);
        case ZCC_TOKEN_ID:
            return get_op_id(t);
        default:
            SET_ERR(ZCC_ERR_EXPECTED_OP);
            return NULL;
        }
    }
    else
    {
        SHIFT_TOK();

        TreeNode *op = get_op(t);
        if (!op)
            return NULL;
        TreeNode *prev = op;

        while (TOK->type != ZCC_TOKEN_BRACKET_CRL_CLS && TOK->type != ZCC_TOKEN_EMPTY)
        {
            TreeNode *op_new = get_op(t);
            if (!op_new)
            {
                treenode_del(op);
                return NULL;
            }
            treenode_attachr(prev, op_new);
            prev = op_new;
        }

        if (TOK->type != ZCC_TOKEN_BRACKET_CRL_CLS)
        {
            SET_ERR(ZCC_ERR_EXPECTED_BRACKET);
            treenode_del(op);
            return NULL;
        }
        SHIFT_TOK();

        return op;
    } 
}

TreeNode *get_sp(Token ***t)
{
    ASSERT_T();
    if (TOK->type == ZCC_TOKEN_SEMICOLON)
    {
        SHIFT_TOK();
        return treenode_new(ZCC_TOKEN_SEMICOLON, NULL);
    }
    else
    {
        SET_ERR(ZCC_ERR_EXPECTED_SEMICOLON);
        return NULL;
    }
}

TreeNode *get_p(Token ***t)
{
    ASSERT_T();
    if (TOK->type == ZCC_TOKEN_BRACKET_RND_OPN)
    {
        SHIFT_TOK();

        TreeNode *d = get_d(t);
        if (TOK->type != ZCC_TOKEN_BRACKET_RND_CLS)
        {
            treenode_del(d);
            SET_ERR(ZCC_ERR_EXPECTED_BRACKET)
            return NULL;
        }
        SHIFT_TOK();
        return d;
    }
    else
        return get_x(t);
}

static TreeNode *get_g0(Token **tokens, FuncMap *funcmap)
{
    assert(tokens);
    assert(funcmap);
    
    Token **t = tokens;
    TreeNode *result = get_prg(&t, funcmap);
    if ((*t)->type == ZCC_TOKEN_EMPTY && rdp_error == ZCC_OK)
    {
        return result;
    }
    else if ((*t)->type != ZCC_TOKEN_EMPTY && rdp_error == ZCC_OK)
    {
        rdp_error = ZCC_ERR_UNEXPECTED_END;
        rdp_error_token = *t;
    }

    printf("%d\n", (*t)->type);
    treenode_del(result);
    return NULL;
}

#define INSERT_VAR(_name_) \
{ \
    if (varmap_search(vars, _name_) != NULL) \
        SET_ERR(ZCC_ERR_VAR_REDEF)  \
    int _result_ = varmap_insert(vars, _name_, curoffset); \
    if (_result_ == ZCC_OK) \
        curoffset++; \
    else \
    { \
        SET_ERR(_result_);\
    } \
}

#define VAR_LOOP() \
{ \
    INSERT_VAR((char *)TOK->data); \
    SHIFT_TOK(); \
    if (TOK->type == ZCC_TOKEN_COMMA) \
        SHIFT_TOK(); \
}

TreeNode *get_fn(Token ***t, FuncMap *funcmap)
{
    ASSERT_T();
    assert(funcmap);

    if (TOK->type != ZCC_TOKEN_FN)
    {
        SET_ERR(ZCC_ERR_EXPECTED_FUNC);
        return NULL;
    }
    SHIFT_TOK();

    TreeNode *id = NULL;
    if (TOK->type == ZCC_TOKEN_ID)
    {
        GET_ID_TOKEN();
    }
    if (!id)
        return NULL;

    if (funcmap_search(funcmap, (const char *)id->data))
    {
        SET_ERR(ZCC_ERR_FUNC_REDEF);
        treenode_del(id);
        return NULL;
    }
    
    if (TOK->type != ZCC_TOKEN_BRACKET_RND_OPN)
    {
        SET_ERR(ZCC_ERR_EXPECTED_BRACKET);
        treenode_del(id);
        return NULL;
    }
    SHIFT_TOK();

    VarMap *vars = varmap_new();
    if (!vars)
    {
        SET_ERR(ZCC_ERR_ALLOC)
        treenode_del(id);
    }
    size_t curoffset = 0;

    while (TOK->type == ZCC_TOKEN_ID)
    {
        VAR_LOOP();
    }

    size_t args = curoffset;

    if (TOK->type != ZCC_TOKEN_BRACKET_RND_CLS)
    {
        SET_ERR(ZCC_ERR_EXPECTED_BRACKET);
        treenode_del(id);
        varmap_del(vars);
        return NULL;
    }
    SHIFT_TOK();

    if (TOK->type == ZCC_TOKEN_VAR)
    {
        SHIFT_TOK();

        while (TOK->type == ZCC_TOKEN_ID)
            VAR_LOOP();

        if (TOK->type == ZCC_TOKEN_SEMICOLON)
            SHIFT_TOK();
    }

    TreeNode *body = get_op(t);
    if (!body)
    { 
        treenode_del(id);
        varmap_del(vars);
        return NULL;
    }

    TreeNode *fn = treenode_new(ZCC_TOKEN_FN, NULL);
    if (!fn)
    {
        SET_ERR(ZCC_ERR_ALLOC);
        treenode_del(id);
        varmap_del(vars);
        return NULL;
    }
    fn->data = vars;
    treenode_attachl(fn, id);
    treenode_attachr(fn, body);

    TreeNode *sp = treenode_new(ZCC_TOKEN_SEMICOLON, NULL);
    if (!sp)
    {
        SET_ERR(ZCC_ERR_ALLOC);
        treenode_del(id);
        varmap_del(vars);
        return NULL;
    }
    treenode_attachl(sp, fn);
    funcmap_insert(funcmap, (const char *)id->data, args, curoffset - args);
    return sp;
}

TreeNode *get_prg(Token ***t, FuncMap *funcmap)
{
    ASSERT_T();
    assert(funcmap);

    TreeNode *start = get_fn(t, funcmap);
    if (!start)
        return NULL;
    TreeNode *prev = start;

    while (TOK->type != ZCC_TOKEN_EMPTY)
    {
        TreeNode *fn = get_fn(t, funcmap);
        if (!fn)
        {
            treenode_del(start);
            return NULL;
        }
        treenode_attachr(prev, fn);
        prev = fn;
    }

    return start;
}

TreeNode *rdp(Token **tokens, FuncMap *funcmap)
{
    assert(tokens);
    assert(funcmap);
    
    return get_g0(tokens, funcmap);
}

int rdp_get_err(void)
{
    return rdp_error;
}

Token *rdp_get_err_token(void)
{
    return rdp_error_token;
}
