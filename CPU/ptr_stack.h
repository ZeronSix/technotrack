/*!
 * \file
 * Stack of double variables.
 */
#ifndef PTR_STACK_H 
#define PTR_STACK_H 

#include <stdlib.h>
#include "common.h"

/*!
 * Stack of void* variables.
 */
typedef struct
{
    size_t index;
    size_t alloc_size;
    void* *data;
} PtrStack;

/*!
 * void* stack constructor.
 */
int ptrstack_new(PtrStack *this_, size_t alloc_size);
/*!
 * void* destructor.
 */
void ptrstack_del(PtrStack *this_);
/*!
 * void* push method.
 */ 
int ptrstack_push(PtrStack *this_, void* item);
/*!
 * void* stack pop method.
 */
int ptrstack_pop(PtrStack *this_, void* *item);

#endif /* ifndef PTR_STACK_H */
