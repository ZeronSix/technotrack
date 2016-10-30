/*!
 * \file
 * Stack of double variables.
 */
#ifndef DOUBLE_STACK_H
#define DOUBLE_STACK_H 

#include <stdlib.h>
#include "common.h"

/*!
 * Stack of double variables.
 */
typedef struct
{
    size_t index;
    size_t alloc_size;
    double *data;
} DoubleStack;

/*!
 * Double stack constructor.
 */
int doublestack_new(DoubleStack *this_, size_t alloc_size);
/*!
 * Stack destructor.
 */
void doublestack_del(DoubleStack *this_);
/*!
 * Stack push method.
 */ 
int doublestack_push(DoubleStack *this_, double item);
/*!
 * Stack pop method.
 */
int doublestack_pop(DoubleStack *this_, double *item);

#endif /* ifndef DOUBLE_STACK_H */
