/*!
 * \file
 * Header for assembler.
 */
#ifndef ZASM_H
#define ZASM_H 

#include "common.h"
#include "cpu_arch.h"

/* Maximum label count */
enum { MAX_LABEL = 512 };
enum
{ 
    LABEL = 1,
    NOLABEL = 2
};
enum { MAX_LABEL_STACK = 512 };

#endif /* ifndef ZASM_H */
