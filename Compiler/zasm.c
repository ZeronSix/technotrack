/*!
 * \file
 * Assembler.
 */
#include "common.h"
#include "io.h"
#include "vmarch.h"
#include "hashmap.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

/*!
 * Default output filename.
 */
static const char_t *DEFAULT_OUTPUT_FILENAME = "a.zbin";
enum 
{
    ZASM_CMD_SIZE = 16
};

#define ASSERT_S() { assert(s); assert(*s); }

/*
 * Get constant from input.
 */
static int get_double(char_t **s, double *arg)
{
    ASSERT_S();
    assert(arg);

    double tmp = 0;
    int    len = 0;
    DO_OR_RET_ERR(sscanf(*s, "%lg%n[^:]", &tmp, &len) != 1, ZCC_ERR_EXPECTED_CONST);
    *s += len;

    *arg = tmp;
    return ZCC_OK;
}

/*!
 * Get size_t from input.
 */
static int get_sizet(char_t **s, size_t *arg)
{
    ASSERT_S();
    assert(arg);

    size_t tmp = 0;
    int    len = 0;
    DO_OR_RET_ERR(sscanf(*s, "%lu%n[^:]", &tmp, &len) != 1, ZCC_ERR_EXPECTED_CONST);
    *s += len;

    *arg = tmp;
    return ZCC_OK;
}

/*!
 * Write a byte.
 */
static void write_byte(byte_t **buf, byte_t byte)
{
    assert(buf);
    assert(*buf);
    **buf = byte;
    (*buf)++;
}

/*!
 * Write a double.
 */
static void write_double(byte_t **buf, double arg)
{
    assert(buf);
    assert(*buf);
    *(double *)*buf = arg;
    (*buf) += sizeof(arg);
}

#define SKIP_SPACES() \
{ \
    assert(s); \
    \
    while (isspace(*s)) \
        s++; \
}

#define SKIP_COMMENTS() \
{ \
    assert(s); \
    SKIP_SPACES(); \
    \
    while (true) \
    { \
        if (*s == ';') \
        { \
            char_t *_c_ = strchr(s, '\n'); \
            if (!_c_) \
                s = strchr(s, '\0'); \
            else \
                s = _c_; \
        } \
        else \
            break; \
    } \
}

#define IF_CHECK_CMD(_code_, _len_) \
if (strncasecmp(*s, #_code_, _len_)) \
{ \
    *code = CMD_ ## _code_; \
    *s += _len_; \
    return ZCC_OK; \
}

/*! 
 * Read command.
 */
static int read_cmd(char_t **s, int *code)
{
    ASSERT_S();
    assert(code);

    IF_CHECK_CMD(PUSH, 4)
    else IF_CHECK_CMD(POP, 3)
    else IF_CHECK_CMD(ADD, 3)
    else IF_CHECK_CMD(SUB, 3)
    else IF_CHECK_CMD(MUL, 3)
    else IF_CHECK_CMD(DIV, 3)
    else IF_CHECK_CMD(AND, 3)
    else IF_CHECK_CMD(OR, 2)
    else IF_CHECK_CMD(ABV, 3)
    else IF_CHECK_CMD(BLW, 3)
    else IF_CHECK_CMD(AOE, 3)
    else IF_CHECK_CMD(BOE, 3)
    else IF_CHECK_CMD(EQ, 2)
    else IF_CHECK_CMD(RET, 3)
    else IF_CHECK_CMD(CALL, 4)
    else IF_CHECK_CMD(JMP, 3)
    else IF_CHECK_CMD(JMPC, 4)
    else IF_CHECK_CMD(STACKFRAME, 10)
    else
    {
        *code = CMD_UNKNOWN;
        return ZCC_ERR_UNKNOWN_CMD;
    }
}

enum { MAX_LABEL_STACK = 1024 };

#define FREE_DATA() \
{ \
    ptrstack_del(&addrstack); \
    ptrstack_del(&labelstack); \
    varmap_del(labels); \
}

static int run(Text *source, byte_t *buf, size_t *buf_len)
{
    assert(source);
    assert(buf);
    assert(buf_len);

    char_t *s = source->str;
    byte_t *b = buf;
    int cmd_code = 0;
    VarMap *labels = varmap_new();
    if (!labels)
        return ZCC_ERR_ALLOC;
    PtrStack addrstack = {};
    PtrStack labelstack = {};
    DO_OR_RET_ERR(ptrstack_new(&addrstack, MAX_LABEL_STACK), ZCC_ERR_ALLOC);
    DO_OR_RET_ERR(ptrstack_new(&labelstack, MAX_LABEL_STACK), ZCC_ERR_ALLOC);

    while (true)
    {
        SKIP_SPACES();
        SKIP_COMMENTS();
        SKIP_SPACES();

        if (read_cmd(&s, &cmd_code) != ZCC_OK)
        {
            if (*s == '\0') 
                break;

            FREE_DATA();
            return ZCC_ERR_WRONG_ASM;
        }

        SKIP_SPACES();
        SKIP_COMMENTS();
        SKIP_SPACES();

        write_byte(&b, cmd_code);

        if (cmd_code == CMD_PUSH)
        {
            double *arg = 0;
            if (get_double(&s, arg) != ZCC_OK)
            {
                FREE_DATA();
            }
        }
        else if (cmd_code == CMD_JMP)
        {

        }
        else if (cmd_code == CMD_JMPC)
        {

        }
    } 

    *buf_len = b - buf;
    FREE_DATA();
    return ZCC_OK;
}

int main(int argc, const char **argv)
{
    if (argc != 3 && argc != 2)
    {
        fprintf(stderr, "Wrong arguments!\n");
        return ZCC_ERR_WRONG_ARGS;
    }

    const char_t *input = argv[1];
    const char_t *output = argc == 2 ? DEFAULT_OUTPUT_FILENAME : argv[2];

    Text source = {};
    int result = ZCC_OK;
    if ((result = text_new(&source, input)) != ZCC_OK)
    {
        fprintf(stderr, "ASM error while reading %s: %d\n", input, result);
        return result;
    }

    byte_t *buffer = (byte_t *)calloc(source.size, sizeof(*buffer) * ZASM_CMD_SIZE);
    if (!buffer)
    {
        fprintf(stderr, "ZASM memory allocation error!\n");
        return ZCC_ERR_ALLOC;
    }

    size_t size = 0;
    result = run(&source, buffer, &size);
    if (result != ZCC_OK)
    {
        switch (result) 
        {
            case ZCC_ERR_WRONG_ASM:
                fprintf(stderr, "Wrong ASM code!\n");
                break;
            default:
                fprintf(stderr, "ASM error: %d\n", result);
                break;
        }
    
        text_del(&source);
        free(buffer);
        return result;
    }

    if ((result = write_bytes(output, buffer, size)) != ZCC_OK)
    {
        fprintf(stderr, "ASM failed to write binary code!\n");
        text_del(&source);
        free(buffer);
        return result;
    }

    text_del(&source);
    free(buffer);

    return ZCC_OK;
} 
