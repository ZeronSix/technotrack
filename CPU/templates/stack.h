/*!
 * \file
 * Stack of double variables.
 */
#ifndef {filename} 
#define {filename} 

#include <stdlib.h>
#include "common.h"

/*!
 * Stack of {T} variables.
 */
typedef struct
{{
    size_t index;
    size_t alloc_size;
    {T} *data;
}} {name};

/*!
 * {T} stack constructor.
 */
int {prefix}new({name} *this_, size_t alloc_size);
/*!
 * {T} destructor.
 */
void {prefix}del({name} *this_);
/*!
 * {T} push method.
 */ 
int {prefix}push({name} *this_, double item);
/*!
 * {T} stack pop method.
 */
int {prefix}pop({name} *this_, double *item);

#endif /* ifndef {filename} */
