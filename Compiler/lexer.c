#include "lexer.h"
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define SKIP_SPACES() \
{ \
    while (true) \
    { \
        if (*s == '\n') \
        {\
            line_start = s + 1; \
            line++; \
        } \
        if (!isspace(*s)) \
            break; \
        s++; \
    } \
}
#define KILL_TOKENS() { for (size_t i = 0; i < index; i++) free(tokens[i]); free(tokens); }
#define ELSE_IF_CHECK_CHAR(_char_, _code_) \
else if (*s == _char_) \
{ \
    token->type = _code_; \
    token->data = NULL; \
    s++; \
} 
#define ELSE_IF_CHECK_CHAR(_char_, _code_) \
else if (*s == _char_) \
{ \
    token->type = _code_; \
    token->data = NULL; \
    s++; \
} 
#define ELSE_
#define ELSE_IF_CHECK_KEYWORD(_wrd_, _len_, _type_) \
else if (strncmp(s, _wrd_, _len_) == 0 && !isalpha(s[_len_]) && !isdigit(s[_len_])) \
{ \
    token->type = _type_; \
    token->data = NULL; \
    s += _len_; \
}
#define ELSE_IF_CHECK_SEQUENCE(_seq_, _len_, _type_) \
else if (strncmp(s, _seq_, _len_) == 0) \
{ \
    token->type = _type_; \
    token->data = NULL; \
    s += _len_; \
}

Token **analyze(const Text *expr)
{
    assert(expr);
    assert(expr->str);

    if (expr->size == 0)
        return NULL;

    Token **tokens = (Token **)calloc(expr->size + 2, sizeof(*tokens));
    const char *s = expr->str;
    size_t index = 0;
    bool   quit  = false;
    size_t line = 0;
    const char *line_start = s;

    while (!quit)
    {
        double num_data    = 0;
        int    chars_read  = 0;
        char_t str_data[ZCC_MAX_TOKEN_STR_SIZE] = "";

        SKIP_SPACES();
        fputs(s, stderr);
        if (strncmp(s, "//", 2) == 0)
        {
            char_t *next = strchr(s, '\n');
            if (!next)
                next = strchr(s, '\0');
            s = next;
            continue;
        }

        Token *token = (Token *)calloc(1, sizeof(*token));
        if (!token)
        {
            free(token);
            KILL_TOKENS();
            return NULL;
        }
        token->type = ZCC_TOKEN_EMPTY;
        token->line = line;
        token->column = (size_t)(s - line_start);

        if (sscanf(s, "%lg%n", &num_data, &chars_read) == 1)
        { 
            token->type = ZCC_TOKEN_CONST;
            double *new_data = (double *)calloc(1, sizeof(*new_data));
            *new_data = num_data;
            token->data = (void *)new_data;
            s += chars_read;
        }
        ELSE_IF_CHECK_SEQUENCE("<=", 2, ZCC_TOKEN_BELOW_OR_EQ)
        ELSE_IF_CHECK_SEQUENCE(">=", 2, ZCC_TOKEN_ABOVE_OR_EQ)
        ELSE_IF_CHECK_SEQUENCE("==", 2, ZCC_TOKEN_EQ)
        ELSE_IF_CHECK_KEYWORD("if", 2, ZCC_TOKEN_IF)
        ELSE_IF_CHECK_KEYWORD("while", 5, ZCC_TOKEN_WHILE)
        ELSE_IF_CHECK_KEYWORD("and", 3, ZCC_TOKEN_AND)
        ELSE_IF_CHECK_KEYWORD("or", 2, ZCC_TOKEN_OR)
        ELSE_IF_CHECK_KEYWORD("else", 4, ZCC_TOKEN_ELSE)
        ELSE_IF_CHECK_KEYWORD("var", 3, ZCC_TOKEN_VAR)
        ELSE_IF_CHECK_KEYWORD("fn", 2, ZCC_TOKEN_FN)
        ELSE_IF_CHECK_KEYWORD("ret", 3, ZCC_TOKEN_RET)
        ELSE_IF_CHECK_CHAR('(', ZCC_TOKEN_BRACKET_RND_OPN)
        ELSE_IF_CHECK_CHAR(')', ZCC_TOKEN_BRACKET_RND_CLS)
        ELSE_IF_CHECK_CHAR('{', ZCC_TOKEN_BRACKET_CRL_OPN)
        ELSE_IF_CHECK_CHAR('}', ZCC_TOKEN_BRACKET_CRL_CLS)
        ELSE_IF_CHECK_CHAR('+', ZCC_TOKEN_PLUS)
        ELSE_IF_CHECK_CHAR('-', ZCC_TOKEN_MINUS)
        ELSE_IF_CHECK_CHAR('/', ZCC_TOKEN_DIV)
        ELSE_IF_CHECK_CHAR('*', ZCC_TOKEN_MUL)
        ELSE_IF_CHECK_CHAR(',', ZCC_TOKEN_COMMA)
        ELSE_IF_CHECK_CHAR(';', ZCC_TOKEN_SEMICOLON)
        ELSE_IF_CHECK_CHAR('=', ZCC_TOKEN_ASSIGN)
        ELSE_IF_CHECK_CHAR('<', ZCC_TOKEN_BELOW)
        ELSE_IF_CHECK_CHAR('>', ZCC_TOKEN_ABOVE)
        else if (sscanf(s, "%255[A-Za-z0-9_]%n", str_data, &chars_read) == 1)
        {
            token->type = ZCC_TOKEN_ID;
            char_t *new_data = (char_t *)calloc((size_t)chars_read + 1, sizeof(char_t *));
            if (!new_data)
            {
                free(new_data);
                free(token);
                KILL_TOKENS();
                return NULL;
            }
            strcpy(new_data, str_data);
            token->data = new_data;
            fprintf(stderr, "%s\n", new_data);
            s += chars_read;
        }
        else
            quit = true;

        fprintf(stderr, "-%d\n", token->type);
        SKIP_SPACES();
        tokens[index] = token;
        index++;
    }

    return tokens;
}

int del_tokens(Token **tokens)
{
    if (!tokens)
        return ZCC_OK;

    Token **t = tokens;
    while (*t != NULL)
    {
        free((*t)->data);
        free(*t);
        t++;
    }
    free(tokens);

    return ZCC_OK;
}
