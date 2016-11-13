#ifndef EZ_COMMON_H
#define EZ_COMMON_H 

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

/*!
 * A simple macro for unit testing.
 * \param[in] x Expression to test.
 */
#define test(x) { printf("# Testing %s... ", #x); x ? printf("correct!\n") : printf("incorrect!\n"); }

typedef enum
{
    EZ_OK = 0,
    EZ_ERR_MEMORY_ALLOC,
    EZ_ERR_VERIFY,
    EZ_ERR_STACK_INCORRECT_PUSH,
    EZ_ERR_STACK_INCORRECT_POP,
    EZ_ERR_STACK_OVERFLOW,
    EZ_ERR_STACK_UNDERFLOW
} ErrorCode; 

#endif /* ifndef EZ_COMMON_H */
