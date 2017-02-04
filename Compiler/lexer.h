/*!
 * \file
 * Lexical analyzer.
 */
#ifndef ZCC_LEXER_H
#define ZCC_LEXER_H
#include "common.h"
#include "io.h"

/*!
 * Tokens.
 */
enum
{
    ZCC_TOKEN_EMPTY           = 0,
    ZCC_TOKEN_CONST           = 1,
    ZCC_TOKEN_PLUS            = 2,
    ZCC_TOKEN_MINUS           = 3,
    ZCC_TOKEN_MUL             = 4,
    ZCC_TOKEN_DIV             = 5,
    ZCC_TOKEN_ID              = 6,
    ZCC_TOKEN_ASSIGN          = 7,
    ZCC_TOKEN_BRACKET_RND_OPN = 8,
    ZCC_TOKEN_BRACKET_RND_CLS = 9,
    ZCC_TOKEN_BRACKET_CRL_OPN = 10,
    ZCC_TOKEN_BRACKET_CRL_CLS = 11,
    ZCC_TOKEN_COMMA           = 12,
    ZCC_TOKEN_SEMICOLON       = 13,
    ZCC_TOKEN_WHILE           = 14,
    ZCC_TOKEN_IF              = 15,
    ZCC_TOKEN_AND             = 16,
    ZCC_TOKEN_OR              = 17,
    ZCC_TOKEN_ELSE            = 18,
    ZCC_TOKEN_VAR             = 19,
    ZCC_TOKEN_FN              = 20,
    ZCC_TOKEN_ABOVE           = 21,
    ZCC_TOKEN_BELOW           = 22,
    ZCC_TOKEN_ABOVE_OR_EQ     = 23,
    ZCC_TOKEN_BELOW_OR_EQ     = 24,
    ZCC_TOKEN_EQ              = 25,
    ZCC_TOKEN_RET             = 26,
    ZCC_TOKEN_FN_CALL         = 27
};

/*! Max token str size. */
enum { ZCC_MAX_TOKEN_STR_SIZE = 256 };

/*!
 * Token data.
 */
typedef struct 
{
    int  type; 
    void *data;
    size_t line;
    size_t column;
} Token;

/*!
 * Variable token
 */
typedef struct
{
    size_t args;
    size_t locals;
    Token ** tokens;    
} VarToken;

/*!
 * Analyze an expression.
 */
Token **analyze(const Text *expr);
/*!
 * Delete tokens.
 */
int del_tokens(Token **tokens);

#endif /* ifndef LEXER_H */
