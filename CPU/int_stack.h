/*!
 * \file
 * Stack of double variables.
 */
#ifndef INT_STACK_H 
#define INT_STACK_H 

#include <stdlib.h>
#include "common.h"

/*!
 * Stack of int variables.
 */
typedef struct
{
    size_t index;
    size_t alloc_size;
    int *data;
} IntStack;

/*!
 * int stack constructor.
 */
int intstack_new(IntStack *this_, size_t alloc_size);
/*!
 * int destructor.
 */
void intstack_del(IntStack *this_);
/*!
 * int push method.
 */ 
int intstack_push(IntStack *this_, double item);
/*!
 * int stack pop method.
 */
int intstack_pop(IntStack *this_, double *item);

#endif /* ifndef INT_STACK_H */
