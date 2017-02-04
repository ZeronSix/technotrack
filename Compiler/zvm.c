/*!
 * \file
 * Virtual machine (CPU).
 */
#include "common.h"
#include "zvm.h"
#include "io.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

/*!
 * Get double.
 */
#define GET_DOUBLE() \
{ \
    if (b + sizeof(double) > buffer + buf_size) \
        return ZCC_ERR_OUT_OF_BOUNDS; \
    d_arg = *(double *)b; \
    b += sizeof(double); \
} 

/*!
 * Get size_t.
 */
#define GET_SIZET() \
{ \
    if (b + sizeof(size_t) > buffer + buf_size) \
        return ZCC_ERR_OUT_OF_BOUNDS; \
    st_arg = *(size_t *)b; \
    b += sizeof(size_t); \
} 

/*!
 * Jump function
 */
static int jump(byte_t *buffer, byte_t **buf_ptr, size_t index, size_t buf_size)
{
    assert(buffer);
    assert(buf_ptr);
    assert(*buf_ptr);
    assert(buf_size > 0);

    if ((size_t)index > buf_size)
    {
        return ZCC_ERR_OUT_OF_BOUNDS;
    }
    *buf_ptr = buffer + (size_t)index;

    return ZCC_OK;
}

/*!
 * Run program.
 */
static int run(VM *vm, byte_t *buffer, size_t buf_size)
{
    assert(vm);
    assert(buffer);
    
    byte_t *b = buffer;
    double d_arg = 0;
    double d_arg2 = 0;
    size_t st_arg = 0;
    byte_t *addr = NULL;
    size_t stackframe = 0;
    while (*b)
    {
        byte_t cmd = CMD_UNKNOWN;
        cmd = *b;
        b++;
        
        switch (cmd)
        {
            case CMD_PUSH: 
                GET_DOUBLE();
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg),
                              ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_POP:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                break;
            case CMD_OUT:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                printf("%lg\n", d_arg);
                break;
            case CMD_IN:
                if (scanf("%lg", &d_arg) != 1)
                    return ZCC_ERR_WRONG_ARGS;
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg), ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_ADD:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg2), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg + d_arg2),
                              ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_SUB:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg2), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg2 - d_arg),
                              ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_MUL:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg2), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg * d_arg2),
                              ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_DIV:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg2), ZCC_ERR_STACK_UNDERFLOW);
                if (fabs(d_arg) < 1E-18)
                    return ZCC_ERR_DIVISION_BY_ZERO;
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg2 / d_arg),
                              ZCC_ERR_STACK_OVERFLOW);
            case CMD_AND:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg2), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg && d_arg2),
                              ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_OR:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg2), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg || d_arg2),
                              ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_ABV:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg2), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg < d_arg2),
                              ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_BLW:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg2), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg > d_arg2),
                              ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_AOE:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg2), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg <= d_arg2),
                              ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_BOE:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg2), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg >= d_arg2),
                              ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_EQ:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg2), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg == d_arg2),
                              ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_JMP:
                GET_SIZET();
                DO_OR_RET_ERR(jump(buffer, &b, st_arg, buf_size), ZCC_ERR_OUT_OF_BOUNDS);
                break;
            case CMD_JMPC:
                GET_SIZET();
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                if (d_arg)
                {
                    DO_OR_RET_ERR(jump(buffer, &b, st_arg, buf_size), ZCC_ERR_OUT_OF_BOUNDS);
                }
                break;
            case CMD_CALL:
                GET_SIZET();
                DO_OR_RET_ERR(ptrstack_push(&vm->call_stack, b), ZCC_ERR_STACK_OVERFLOW);
                DO_OR_RET_ERR(jump(buffer, &b, st_arg, buf_size), ZCC_ERR_OUT_OF_BOUNDS);
                break;
            case CMD_SET:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                stackframe = sizestack_get(&vm->stack_frames, vm->stack_frames.index - 1);
                GET_SIZET();
                if (st_arg + stackframe >= vm->memory_stack.index)
                    return ZCC_ERR_OUT_OF_BOUNDS;
                vm->memory_stack.data[st_arg + stackframe] = d_arg;
                break;
            case CMD_GET:
                stackframe = sizestack_get(&vm->stack_frames, vm->stack_frames.index - 1);
                GET_SIZET();
                if (st_arg + stackframe >= vm->memory_stack.index)
                    return ZCC_ERR_OUT_OF_BOUNDS;
                d_arg = vm->memory_stack.data[st_arg + stackframe];
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg), ZCC_ERR_STACK_OVERFLOW);
                break;
            case CMD_RET:
                DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg), ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(sizestack_pop(&vm->stack_frames, &stackframe),
                              ZCC_ERR_STACK_UNDERFLOW);
                DO_OR_RET_ERR(ptrstack_pop(&vm->call_stack, (void **)&addr),
                              ZCC_ERR_STACK_UNDERFLOW);
                while (vm->memory_stack.index != stackframe)
                {
                    DO_OR_RET_ERR(doublestack_pop(&vm->memory_stack, &d_arg2), ZCC_ERR_STACK_UNDERFLOW);
                }
                DO_OR_RET_ERR(doublestack_push(&vm->memory_stack, d_arg), ZCC_ERR_STACK_OVERFLOW);
                DO_OR_RET_ERR(jump(buffer, &b, (size_t)(addr - buffer), buf_size),
                              ZCC_ERR_OUT_OF_BOUNDS);
                break;
            case CMD_STACKFRAME:
                GET_SIZET();
                if (st_arg > vm->memory_stack.index)
                    return ZCC_ERR_OUT_OF_BOUNDS;
                DO_OR_RET_ERR(sizestack_push(&vm->stack_frames, vm->memory_stack.index - st_arg), 
                              ZCC_ERR_STACK_OVERFLOW);
                break;
            default:
                return ZCC_ERR_UNKNOWN_CMD;
        }

        //printf("%ld\n", b - buffer);
    }

    return ZCC_OK;
}

int main(int argc, char_t **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Wrong VM args!\n");
        return ZCC_ERR_WRONG_ARGS;
    }

    byte_t buffer[VM_MAX_MEM_SIZE + 1] = {};
    size_t buf_size = 0;
    
    if (read_bytes(argv[1], buffer, VM_MAX_MEM_SIZE, &buf_size) != ZCC_OK)
    {
        fprintf(stderr, "VM couldn't read input binary %s!\n", argv[1]);
        return ZCC_ERR_READ_FILE;
    }

    VM vm = {};
    int result = vm_new(&vm);
    if (result != ZCC_OK)
    {
        vm_del(&vm);
        return result;
    }

    result = run(&vm, buffer, buf_size);
    if (result != ZCC_OK)
        fprintf(stderr, "VM error %d!\n", result);
    vm_del(&vm);

    return result;
}

int vm_new(VM *this_)
{
    assert(this_);

    int result = doublestack_new(&this_->memory_stack, VM_MAX_STACK);
    if (result != ZCC_OK)
        return result;
    result = ptrstack_new(&this_->call_stack, VM_MAX_STACK);
    if (result != ZCC_OK)
    {
        doublestack_del(&this_->memory_stack);
        return result;
    }
    result = sizestack_new(&this_->stack_frames, VM_MAX_STACK);
    if (result != ZCC_OK)
    {
        doublestack_del(&this_->memory_stack);
        ptrstack_del(&this_->call_stack);
        return result;
    }

    return ZCC_OK;
}

int vm_del(VM *this_)
{
    if (!this_)
        return ZCC_OK;

    doublestack_del(&this_->memory_stack);
    ptrstack_del(&this_->call_stack);
    sizestack_del(&this_->stack_frames);
    
    return ZCC_OK;
}
