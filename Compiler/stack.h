/*!
 * \file
 * Contains stacks.
 */
#ifndef STACK_H
#define STACK_H 

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
 * double stack constructor.
 */
int doublestack_new(DoubleStack *this_, size_t alloc_size);
/*!
 * double destructor.
 */
void doublestack_del(DoubleStack *this_);
/*!
 * double push method.
 */ 
int doublestack_push(DoubleStack *this_, double item);
/*!
 * double stack pop method.
 */
int doublestack_pop(DoubleStack *this_, double *item);

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
int intstack_push(IntStack *this_, int item);
/*!
 * int stack pop method.
 */
int intstack_pop(IntStack *this_, int *item);

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
/*!
 * get item without popping.
 */
void *ptrstack_get(PtrStack *this_, size_t index);

/*!
 * Stack of size_t variables.
 */
typedef struct
{
    size_t index;
    size_t alloc_size;
    size_t *data;
} SizeStack;

/*!
 * int stack constructor.
 */
int sizestack_new(SizeStack *this_, size_t alloc_size);
/*!
 * int destructor.
 */
void sizestack_del(SizeStack *this_);
/*!
 * int push method.
 */ 
int sizestack_push(SizeStack *this_, size_t item);
/*!
 * int stack pop method.
 */
int sizestack_pop(SizeStack *this_, size_t *item);
size_t sizestack_get(SizeStack *this_, size_t index);

#endif /* ifndef STACK_H */
