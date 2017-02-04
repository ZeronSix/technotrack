#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "rdp.h"
#include "treemath.h"

/*!
 * RDP Error Code
 */
static int rdpError = RDP_ERR_OK;
/*!
 * RDP Error Pos.
 */
static size_t rdpErrorPos = 0;

/*!
 * Handful assert macro.
 */
#define CHECK_S(s) { assert(s); assert(*s); }

/*!
 * Get a constant.
 */
static DTreeNode *GetN(const char **s);
/*!
 * Get a variable.
 */
static DTreeNode *GetVar(const char **s);
/*!
 * Get an expression.
 */
static DTreeNode *GetE(const char **s);
/*!
 * Get a multiplier.
 */
static DTreeNode *GetT(const char **s);
/*!
 * Get a parenthesis operation.
 */
static DTreeNode *GetP(const char **s);
/*!
 * Get a power operation.
 */
static DTreeNode *GetD(const char **s);
/*!
 * Get an elementary function.
 */
static DTreeNode *GetFn(const char **s);
/*!
 * Core RDP function.
 */
static DTreeNode *GetG0(const char **s);

/*!
 * Function that skips space characters in the buffer.
 */
static void SkipSpaces(const char **s)
{
    CHECK_S(s);

    while (isspace(**s))
        (*s)++;
}

DTreeNode *GetN(const char **s)
{
    double value  = 0;
    int charsRead = 0;

    CHECK_S(s);

    SkipSpaces(s);

    if (sscanf(*s, "%lg%n", &value, &charsRead) != 1)
    {
        return NULL;
    }
    *s += charsRead;

    SkipSpaces(s);

    return DTreeNodeCtor(NODE_CONST, value);
}

DTreeNode *GetVar(const char **s)
{
    CHECK_S(s);

    SkipSpaces(s);
    if (**s == 'x')
    {
        (*s)++;
        DTreeNode *result = DTreeNodeCtor(NODE_VAR, 0);
        SkipSpaces(s);
        return result;
    }
    else
    {
        rdpError = RDP_ERR_EXPECTED_VARIABLE;
        return NULL;
    }
}

DTreeNode *GetE(const char **s)
{
    DTreeNode *result = NULL;

    SkipSpaces(s);
    result = GetT(s);
    if (!result)
        return NULL;
    SkipSpaces(s);

    while (**s == '+' || **s == '-')
    {
        char op = **s;
        (*s)++;

        SkipSpaces(s);

        DTreeNode *result2 = GetT(s);
        if (result2)
        {
            if (op == '+') 
                result = Sum(result, result2);
            else if (op == '-')
                result = Minus(result, result2);
        }
        else
        {
            DTreeNodeDtor(result);
            return NULL;
        }

        SkipSpaces(s);
    }

    return result;
}

DTreeNode *GetT(const char **s)
{
    DTreeNode *result = NULL;

    SkipSpaces(s);
    result = GetD(s);
    if (!result)
        return NULL;
    SkipSpaces(s);

    while (**s == '*' || **s == '/')
    {
        char op = **s;
        (*s)++;

        SkipSpaces(s);

        DTreeNode *result2 = GetD(s);
        if (result2)
        {
            if (op == '*') 
                result = Mul(result, result2);
            else if (op == '/')
                result = Div(result, result2);
        }
        else
        {
            DTreeNodeDtor(result);
            return NULL;
        }

        SkipSpaces(s);
    }

    return result;
}
static DTreeNode *GetD(const char **s)
{
    CHECK_S(s);

    SkipSpaces(s);
    DTreeNode *result = GetP(s);
    if (!result)
        return NULL;
    SkipSpaces(s);

    while (**s == '^')
    {
        (*s)++;
        SkipSpaces(s);

        DTreeNode *result2 = GetP(s);
        if (!result2)
        {
            DTreeNodeDtor(result);
            return NULL;
        }
        result = Pow(result, result2);

        SkipSpaces(s);
    }

    return result;
}

#define GET_FN_RESULT() \
{ \
    SkipSpaces(s); \
    if (**s != '(') \
    { \
        rdpError = RDP_ERR_EXPECTED_BRACKETS; \
        return NULL; \
    } \
    (*s)++; \
    \
    result = GetE(s); \
    if (!result) \
        return NULL; \
    \
    SkipSpaces(s); \
    if (**s != ')') \
    { \
        rdpError = RDP_ERR_EXPECTED_BRACKETS; \
        return NULL; \
    } \
    (*s)++; \
}

DTreeNode *GetFn(const char **s)
{
    CHECK_S(s);
    SkipSpaces(s);
    DTreeNode *result = NULL;

    if (strncmp(*s, "sin", 3) == 0)
    {
        *s += 3;
        GET_FN_RESULT();
        return Sin(result);
    }
    else if (strncmp(*s, "cos", 3) == 0)
    {
        *s += 3;
        GET_FN_RESULT();
        return Cos(result);
    }
    else if (strncmp(*s, "ln", 2) == 0)
    {
        *s += 2;
        GET_FN_RESULT();
        return Ln(result);
    }
    else
    {
        return NULL;
    }
}

DTreeNode *GetP(const char **s)
{
    CHECK_S(s);
    DTreeNode *result = NULL;

    SkipSpaces(s);
    if (**s == '(')
    {
        (*s)++;
        SkipSpaces(s);
        result = GetE(s);
        if (!result)
            return NULL;
        SkipSpaces(s);

        if (**s == ')')
        {
            (*s)++;
        }

        SkipSpaces(s);
    }
    else if ((result = GetN(s)) != NULL)
        return result;
    else if ((result = GetFn(s)) != NULL)
        return result;
    else if (rdpError != RDP_ERR_EXPECTED_BRACKETS)
        return GetVar(s);

    return result;
}

DTreeNode *GetG0(const char **s)
{
    CHECK_S(s);

    DTreeNode *result = GetE(s);
    if (**s != '\0' && rdpError == RDP_ERR_OK)
        rdpError = RDP_ERR_UNEXPECTED_END;


    return result;
}

int RDPGetError(void)
{
    return rdpError;
}

DTreeNode *RDP(const char *s)
{
    assert(s);
    const char *start = s;
    DTreeNode *result = GetG0(&s);
    if (rdpError != RDP_ERR_OK)
        rdpErrorPos = (size_t)labs(s - start);

    return result;
}

size_t RDPGetErrorPos(void)
{
    return rdpErrorPos; 
}
