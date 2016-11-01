/*!
 * \file
 * \author Vyacheslav "ZeronSix" Zhdanovskiy
 * Basic stack CPU emulator.
 */
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "common.h"
#include "io.h"
#include "zcpu.h"

/*!
 * Maximum input string length.
 */
#define MAX_BUF_SIZE 1000000

/*!
 * Reads a double from buffer
 */
static int get_const_arg(Byte **buf, double *arg)
{
    assert(buf);
    assert(*buf);
    assert(arg);
 
    *arg = *(double *)(*buf);
    *buf += sizeof(double) / sizeof(Byte);
    return ZCPU_OK;
}

/*!
 * Reads a byte from buffer
 */
static int get_byte(Byte **buf, Byte *cmd)
{
    assert(buf);
    assert(*buf);
    assert(cmd);

    *cmd = **buf;
    (*buf)++;
    return ZCPU_OK;
}

/*!
 * Univrsal argument read function.
 */
static int get_arg(CPU *cpu, Byte **buf, double *arg)
{
    assert(cpu);
    assert(buf);
    assert(*buf);
    assert(arg);

    Byte ad_mode = CMD_UNKNOWN;
    if (get_byte(buf, &ad_mode) != ZCPU_OK)
    {
        return ZCPU_ERR_WRONG_BINARY;
    }

    double tmp = 0;
    Byte reg = 0;
    switch (ad_mode)
    {
        case ADDR_MODE_CONST:
            if (get_const_arg(buf, &tmp) != ZCPU_OK)
            {
                return ZCPU_ERR_WRONG_BINARY;
            }
            break;
        case ADDR_MODE_REGISTER:
            if (get_byte(buf, &reg) != ZCPU_OK) 
            {
                return ZCPU_ERR_WRONG_BINARY;
            }
            switch (reg)
            {
                case REG_AX:
                    tmp = cpu->ax;
                    break;
                case REG_BX:
                    tmp = cpu->bx;
                    break;
                case REG_CX:
                    tmp = cpu->cx;
                    break;
                case REG_DX:
                    tmp = cpu->dx;
                    break;
                case REG_SP:
                    tmp = *cpu_get_sp(cpu);
                    break;
                default:
                    return ZCPU_ERR_UNKNOWN_REGISTER;
            }
            break;
        default:
            return ZCPU_ERR_UNKNOWN_ADDR_MODE;
    }
    *arg = tmp;

    return ZCPU_OK;
}

/*!
 * Read an argument from stdin
 */
static int input_arg(double *arg)
{
    assert(arg);

    double tmp = 0;
    if (scanf("%lg", &tmp) != 1)
    {
        return ZCPU_ERR_WRONG_INPUT;
    }

    *arg = tmp;
    return ZCPU_OK;
}

/*!
 * Jump function
 */
static int jump(Byte *buffer, Byte **buf_ptr, double index, size_t buf_size)
{
    assert(buffer);
    assert(buf_ptr);
    assert(*buf_ptr);
    assert(index > 0);
    assert(buf_size > 0);

    if ((size_t)index > buf_size)
    {
        return ZCPU_ERR_OUT_OF_BOUNDS;
    }
    *buf_ptr = buffer + (size_t)index;

    return ZCPU_OK;
}

/*!
 * Run cycle.
 */
static int run(CPU *cpu, Byte *buffer, size_t buf_size)
{
    assert(cpu);
    assert(buffer);

    Byte *buf_ptr = buffer;
    DoubleStack callstack = {};
    DO_OR_RET_ERR(doublestack_new(&callstack, MAX_STACK), ZCPU_ERR_INIT);

    while (*buf_ptr)  
    {
        unsigned char cmd = CMD_UNKNOWN;
        if (get_byte(&buf_ptr, &cmd) != ZCPU_OK)
        {
            return ZCPU_ERR_WRONG_CMD;
        }
        else if (cmd == CMD_PUSH)
        {
            double a = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &a), ZCPU_ERR_WRONG_INPUT);
            DO_OR_RET_ERR(doublestack_push(&cpu->stack, a),
                          ZCPU_ERR_STACK_OVERFLOW);
        }
        else if (cmd == CMD_POP)
        {
            DO_OR_RET_ERR(doublestack_pop(&cpu->stack, NULL),
                          ZCPU_ERR_STACK_UNDERFLOW);
        }
        else if (cmd == CMD_ADD)
        {
            double arg1 = 0, arg2 = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg1),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg2),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(doublestack_push(&cpu->stack, arg1 + arg2),
                          ZCPU_ERR_STACK_OVERFLOW);
        }
        else if (cmd == CMD_SUB)
        {
            double arg1 = 0, arg2 = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg1),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg2),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(doublestack_push(&cpu->stack, arg1 - arg2),
                          ZCPU_ERR_STACK_OVERFLOW);
        }
        else if (cmd == CMD_MUL)
        {
            double arg1 = 0, arg2 = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg1),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg2),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(doublestack_push(&cpu->stack, arg1 * arg2),
                          ZCPU_ERR_STACK_OVERFLOW);

        }
        else if (cmd == CMD_DIV)
        {
            double arg1 = 0, arg2 = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg1),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg2),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(doublestack_push(&cpu->stack, arg2 / arg1),
                          ZCPU_ERR_STACK_OVERFLOW);
        }
        else if (cmd == CMD_SIN)
        {
            double arg = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(doublestack_push(&cpu->stack, sin(arg)),
                          ZCPU_ERR_STACK_OVERFLOW);
        }
        else if (cmd == CMD_COS)
        {
            double arg = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(doublestack_push(&cpu->stack, cos(arg)),
                          ZCPU_ERR_STACK_OVERFLOW);
        }
        else if (cmd == CMD_SQRT)
        {
            double arg = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(doublestack_push(&cpu->stack, sqrt(arg)),
                          ZCPU_ERR_STACK_OVERFLOW);
        }
        else if (cmd == CMD_IN) 
        {
            double a = 0;
            DO_OR_RET_ERR(input_arg(&a), ZCPU_ERR_WRONG_INPUT);
            DO_OR_RET_ERR(doublestack_push(&cpu->stack, a),
                          ZCPU_ERR_STACK_OVERFLOW);
        }
        else if (cmd == CMD_OUT)
        {
            double out = 0;
            DO_OR_RET_ERR(doublestack_pop(&cpu->stack, &out),
                          ZCPU_ERR_STACK_UNDERFLOW);
            printf("%lg\n", out);
        }
        else if (cmd == CMD_MOV)
        {
            Byte reg = 0;
            double arg = 1;
            Byte ad_mode = 0;

            DO_OR_RET_ERR(get_byte(&buf_ptr, &ad_mode),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_byte(&buf_ptr, &reg),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg),
                          ZCPU_ERR_WRONG_BINARY);

            switch (reg)
            {
                case REG_AX:
                    cpu->ax = arg;
                    break;
                case REG_BX:
                    cpu->bx = arg;
                    break;
                case REG_CX:
                    cpu->cx = arg;
                    break;
                case REG_DX:
                    cpu->dx = arg;
                    break;
                case REG_SP:
                    *cpu_get_sp(cpu) = arg;
                    break;
                default:
                    return ZCPU_ERR_UNKNOWN_REGISTER;
            }
        }
        else if (cmd == CMD_JMP)
        {
            double arg = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(jump(buffer, &buf_ptr, arg, buf_size),
                          ZCPU_ERR_OUT_OF_BOUNDS);
        }
        else if (cmd == CMD_JE)
        {
            double arg1 = 0, arg2 = 0, arg3 = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg1),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg2),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg3),
                          ZCPU_ERR_WRONG_BINARY);
            if (arg1 == arg2)
            {
                DO_OR_RET_ERR(jump(buffer, &buf_ptr, arg3, buf_size),
                              ZCPU_ERR_OUT_OF_BOUNDS); 
            }
        }
        else if (cmd == CMD_JNE)
        {
            double arg1 = 0, arg2 = 0, arg3 = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg1),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg2),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg3),
                          ZCPU_ERR_WRONG_BINARY);
            if (arg1 != arg2)
            {
                DO_OR_RET_ERR(jump(buffer, &buf_ptr, arg3, buf_size),
                              ZCPU_ERR_OUT_OF_BOUNDS); 
            }
        }
        else if (cmd == CMD_JA)
        {
            double arg1 = 0, arg2 = 0, arg3 = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg1),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg2),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg3),
                          ZCPU_ERR_WRONG_BINARY);
            if (arg1 > arg2)
            {
                DO_OR_RET_ERR(jump(buffer, &buf_ptr, arg3, buf_size),
                              ZCPU_ERR_OUT_OF_BOUNDS); 
            }
        }
        else if (cmd == CMD_JAE)
        {
            double arg1 = 0, arg2 = 0, arg3 = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg1),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg2),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg3),
                          ZCPU_ERR_WRONG_BINARY);
            if (arg1 >= arg2)
            {
                DO_OR_RET_ERR(jump(buffer, &buf_ptr, arg3, buf_size),
                              ZCPU_ERR_OUT_OF_BOUNDS); 
            }
        }
        else if (cmd == CMD_JBE)
        {
            double arg1 = 0, arg2 = 0, arg3 = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg1),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg2),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg3),
                          ZCPU_ERR_WRONG_BINARY);
            if (arg1 <= arg2)
            {
                DO_OR_RET_ERR(jump(buffer, &buf_ptr, arg3, buf_size),
                              ZCPU_ERR_OUT_OF_BOUNDS); 
            }
        }
        else if (cmd == CMD_JB)
        {
            double arg1 = 0, arg2 = 0, arg3 = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg1),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg2),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg3),
                          ZCPU_ERR_WRONG_BINARY);
            if (arg1 < arg2)
            {
                DO_OR_RET_ERR(jump(buffer, &buf_ptr, arg3, buf_size),
                              ZCPU_ERR_OUT_OF_BOUNDS); 
            }
        }
        else if (cmd == CMD_CALL)
        {
            double arg = 0;
            DO_OR_RET_ERR(get_arg(cpu, &buf_ptr, &arg),
                          ZCPU_ERR_WRONG_BINARY);
            DO_OR_RET_ERR(jump(buffer, &buf_ptr, arg, buf_size),
                          ZCPU_ERR_OUT_OF_BOUNDS);
            DO_OR_RET_ERR(doublestack_push(&callstack, buf_ptr - buffer),
                          ZCPU_ERR_STACK_OVERFLOW);
        }
        else if (cmd == CMD_RET)
        {
            double addr = 0;
            DO_OR_RET_ERR(doublestack_pop(&callstack, &addr),
                          ZCPU_ERR_STACK_UNDERFLOW);
            DO_OR_RET_ERR(jump(buffer, &buf_ptr, addr, buf_size),
                          ZCPU_ERR_OUT_OF_BOUNDS);
        }
        else 
        { 
            return ZCPU_ERR_WRONG_CMD;
        }
    }

    doublestack_del(&callstack);
    return ZCPU_OK;    
}

/*!
 *
 */
int main(int argc, const char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Wrong arguments!\n");
        return ZCPU_ERR_WRONG_ARGS;
    }

    Byte buffer[MAX_BUF_SIZE + 1] = {};
    size_t buf_size = 0;
    const char* in_filename = argv[1];

    if (read_bytes(in_filename, buffer, MAX_BUF_SIZE, &buf_size) != ZCPU_OK)
    {
        fprintf(stderr, "Can't read %s\n", in_filename);
        return ZCPU_ERR_FILE_READ;
    }

    CPU cpu = {};
    if (cpu_new(&cpu) != ZCPU_OK)
    {
        return ZCPU_ERR_INIT;
    }
    
    int result = run(&cpu, buffer, buf_size);
    if (result != ZCPU_OK)
    {
        switch (result)
        {
            default:
                fprintf(stderr, "Unknown error! Code %d\n", result);
                break;
        }
    }

    return ZCPU_OK;
}

int cpu_new(CPU *this_)
{
    assert(this_);
    
    return doublestack_new(&this_->stack, MAX_STACK);
}

void cpu_del(CPU *this_)
{
    if (this_ == NULL)
    {
        return;
    }

    doublestack_del(&this_->stack);
}

double *cpu_get_sp(CPU *this_)
{
    assert(this_);
    return this_->stack.data + this_->stack.index - 1;
}
