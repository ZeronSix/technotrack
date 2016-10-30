/*!
 * \file
 * CPU structure.
 */
#ifndef ZCPU_H
#define ZCPU_H 

#include "double_stack.h"
#include "cpu_arch.h"

#define MAX_STACK 1024

typedef struct
{
    DoubleStack stack;
    double ax;
    double bx;
    double cx;
    double dx;
} CPU;

int cpu_new(CPU *this_);
void cpu_del(CPU *this_);
double *cpu_get_sp(CPU *this_);

#endif /* ifndef ZCPU_H */
