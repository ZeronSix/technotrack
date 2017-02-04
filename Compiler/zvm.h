/*!
 * \file
 * VM structure.
 */
#ifndef ZVM_H
#define ZVM_H 

#include "stack.h"
#include "vmarch.h"

/*!
 * VM Max stack size.
 */
enum { VM_MAX_STACK = 4096 };
/*!
 * VM max memory size (buffer).
 */
enum { VM_MAX_MEM_SIZE = 4096 };

typedef struct
{
    DoubleStack memory_stack;
    PtrStack call_stack;
    SizeStack stack_frames;
} VM;

/*!
 * VM constructor.
 */
int vm_new(VM *this_);
/*!
 * VM destructor.
 */
int vm_del(VM *this_);

#endif /* ifndef ZVM_H */
