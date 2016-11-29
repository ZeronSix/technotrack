#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
h
enum 
{
    ERR_OK = 0,
    ERR_EMPTY_NUMBER = 1,
    ERR_UNEXPECTED_END = 2
};

char *s = NULL;
int err_code = ERR_OK;

void SkipSpaces(void);
/*! 
 * Evals a number.
 */
int GetN(void);
/*!
 * Evals an expression.
 */
int GetE(void);
/*!
 * Evals a multiply expression.
 */
int GetT(void);
/*!
 * Evals an expr with parenthesis.
 */
int GetP(void);
/*!
 * Main expression.
 */
int GetG0(char *str);

enum { MAX_INPUT_BUF_SIZE = 1024 };

int main(void)
{
    char buffer[MAX_INPUT_BUF_SIZE] = "";
    fgets(buffer, MAX_INPUT_BUF_SIZE, stdin);
    char *term = strchr(buffer, '\n');
    if (term)
        *term = '\0';
    
    int result = GetG0(buffer);
    if (err_code == ERR_OK)
        printf("I think it is equal to %d\n", result);
    else
    {
        // print enough space to shift the arrow
        for (int i = 0; i < (s - buffer) - 1; i++) 
            putchar(' ');
        putchar('^');
        printf("\nError code: %d\n", err_code);
    }

    return 0;
}

void SkipSpaces(void)
{
    while (isspace(*s))
        s++;
}

int GetN(void)
{
    int result = 0;
    bool empty_flag = true;

    SkipSpaces();

    while (isdigit(*s))
    {
        result = result * 10 + (*s - '0');
        s++;
        empty_flag = false;
    }

    if (empty_flag)
        err_code = ERR_EMPTY_NUMBER;

    SkipSpaces();

    return result;
}

int GetE(void)
{
    SkipSpaces(); 
    int res = GetT();
    SkipSpaces();

    while (*s == '+' || *s == '-')
    {
        char op = *s;
        s++;

        SkipSpaces();

        int res2 = GetT();
        if (op == '+')
            res += res2;
        else if (op == '-')
            res -= res2;

        SkipSpaces();
    }

    return res;
}

int GetT(void)
{
    SkipSpaces(); 
    int res = GetP();
    SkipSpaces();

    while (*s == '*' || *s == '/')
    {
        char op = *s;
        s++;

        SkipSpaces();

        int res2 = GetP();
        if (op == '*')
            res *= res2;
        else if (op == '/')
            res /= res2;

        SkipSpaces();
    }

    return res;
}

int GetP(void)
{
    SkipSpaces();
    int res = 0;

    if (*s == '(')
    {
        s++;
        SkipSpaces();
        res = GetE(); 
        SkipSpaces();

        if (*s == ')')
            s++;

        SkipSpaces();
    }
    else 
        res = GetN();

    return res;
}

int GetG0(char *str)
{
    s = str;
    int result = GetE();
    if (*s != '\0' && err_code == ERR_OK)
        err_code = ERR_UNEXPECTED_END;

    return result;
}
