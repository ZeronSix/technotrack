/*!
 * \file
 * Assembler.
 */
#include "common.h"
#include "vmarch.h"
#include "io.h"
#include "hashmap.h"
#include "stack.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define IF_CHECK_CMD(_cmd_, _len_) \
if (strncasecmp(s, #_cmd_, _len_) == 0 && isspace(*(s + _len_))) \
{ \
    s += _len_; \
    cmd_code = CMD_ ## _cmd_; \
} 
#define SKIP_SPACES() { while (isspace(*s)) s++; }
#define GET_CONST(_t_, _fmt_) \
{ \
    _t_ arg = 0; \
    if (sscanf(s, _fmt_ "%n", &arg, &chars_read) == 1) \
    { \
        s += chars_read; \
        *(_t_ *)b = arg; \
        b += sizeof(_t_); \
    } \
    else \
    { \
        ptrstack_del(&patch_addr); \
        ptrstack_del(&label_stack); \
        return ZCC_ERR_EXPECTED_CONST; \
    } \
}

enum { MAX_LABEL_STACK = 1024 };

/*!
 * Assemble.
 */
static int assemble(LabelMap *labels, char_t *s, byte_t *output, size_t *buf_len)
{
    assert(s);
    assert(labels);
    assert(output);
    assert(buf_len);

    byte_t *b = output;
    int chars_read  = 0;
    PtrStack patch_addr = {};
    PtrStack label_stack = {};
    ptrstack_new(&patch_addr, MAX_LABEL_STACK);
    ptrstack_new(&label_stack, MAX_LABEL_STACK);

    while (*s != '\0')
    {
        int cmd_code = CMD_UNKNOWN;
        chars_read = 0;
        SKIP_SPACES();

        char_t label_buf[ZASM_MAX_LABEL_LEN] = "";
        if (sscanf(s, "%255s%n", label_buf, &chars_read) == 1 && chars_read > 1 && s[chars_read - 1] == ':')
        { 
            if (labelmap_search(labels, label_buf))
            {
                ptrstack_del(&patch_addr); 
                ptrstack_del(&label_stack); 
                return ZCC_ERR_LABEL_REDEF;
            } 
            labelmap_insert(labels, label_buf, (size_t)(b - output));
            s += chars_read + 1;
        }

        SKIP_SPACES();

        if (!*s)
            break;

        IF_CHECK_CMD(PUSH, 4)
        else IF_CHECK_CMD(POP, 3)
        else IF_CHECK_CMD(ADD, 3)
        else IF_CHECK_CMD(SUB, 3)
        else IF_CHECK_CMD(MUL, 3)
        else IF_CHECK_CMD(DIV, 3)
        else IF_CHECK_CMD(AND, 3)
        else IF_CHECK_CMD(OR, 2)
        else IF_CHECK_CMD(IN, 2)
        else IF_CHECK_CMD(OUT, 3)
        else IF_CHECK_CMD(ABV, 3)
        else IF_CHECK_CMD(BLW, 3)
        else IF_CHECK_CMD(AOE, 3)
        else IF_CHECK_CMD(BOE, 3)
        else IF_CHECK_CMD(EQ, 2)
        else IF_CHECK_CMD(RET, 3)
        else IF_CHECK_CMD(CALL, 4)
        else IF_CHECK_CMD(JMPC, 4)
        else IF_CHECK_CMD(JMP, 3)
        else IF_CHECK_CMD(SET, 3)
        else IF_CHECK_CMD(GET, 3)
        else IF_CHECK_CMD(STACKFRAME, 10)
        else
        {
            ptrstack_del(&patch_addr); 
            ptrstack_del(&label_stack); 
            return ZCC_ERR_UNKNOWN_CMD;
        }

        *b = (byte_t)cmd_code;
        b += sizeof(byte_t);

        SKIP_SPACES();

        if (cmd_code == CMD_PUSH)
        {
            GET_CONST(double, "%lg");
        }
        else if (cmd_code == CMD_SET || cmd_code == CMD_GET || cmd_code == CMD_STACKFRAME)
        {
            GET_CONST(size_t, "%lu");
        }
        else if (cmd_code == CMD_JMP || cmd_code == CMD_JMPC || cmd_code == CMD_CALL)
        {
            char_t labelarg_buf[ZASM_MAX_LABEL_LEN] = "";
            if (sscanf(s, "%255s%n", labelarg_buf, &chars_read) == 1 && chars_read > 1 && s[chars_read - 1] == ':')
            { 
                // TODO: check for overflow
                ptrstack_push(&patch_addr, b);
                ptrstack_push(&label_stack, (void *)s);
                s += chars_read + 1;
                b += sizeof(size_t);
            }
            else
            {
                ptrstack_del(&patch_addr); 
                ptrstack_del(&label_stack); 
                return ZCC_ERR_EXPECTED_LABEL;
            }
        }
        
        SKIP_SPACES();
    }

    assert(patch_addr.index == label_stack.index);
    while (patch_addr.index > 0)
    {
        void *sl = NULL;
        void *patch = NULL;
        ptrstack_pop(&patch_addr, &patch);
        ptrstack_pop(&label_stack, &sl);

        char_t label_buf[ZASM_MAX_LABEL_LEN] = "";
        sscanf((char_t *)sl, "%255s", label_buf);
        size_t *offset = labelmap_search(labels, label_buf);
        if (!offset)
        {
            ptrstack_del(&patch_addr); 
            ptrstack_del(&label_stack); 
            return ZCC_ERR_LABEL_NOT_FOUND;
        }
        *(size_t *)patch = *offset;
    }
    ptrstack_del(&patch_addr); 
    ptrstack_del(&label_stack); 
    *buf_len = (size_t)(b - output) + 1;
    return ZCC_OK;
}

enum { CMD_SIZE = 24 };

/*!
 * TODO: usage
 */
int main(int argc, char_t **argv)
{
    int result = ZCC_OK;

    if (argc != 3)
    {
        fprintf(stderr, "Wrong args!\n");
        return ZCC_ERR_WRONG_ARGS;
    }

    Text source = {};
    if ((result = text_new(&source, argv[1])) != ZCC_OK) 
    {
        fprintf(stderr, "IO error %d!", result);
        text_del(&source);
        return result;
    }

    LabelMap *labels = labelmap_new();
    if (!labels)
    {
        fprintf(stderr, "Memory allocation error!\n");
        text_del(&source);
        return ZCC_ERR_ALLOC;
    }

    byte_t *buf = (byte_t *)calloc(source.size + 1, CMD_SIZE);
    if (!buf)
    {
        fprintf(stderr, "Memory allocation error!\n");
        labelmap_del(labels);
        text_del(&source);
        return ZCC_ERR_ALLOC;
    }

    size_t buf_len = 0;
    if ((result = assemble(labels, source.str, buf, &buf_len)))
    {
        fprintf(stderr, "Assembling error: %d\n", result);
        free(buf);
        labelmap_del(labels);
        text_del(&source);
        return ZCC_ERR_ALLOC;
    }

    labelmap_del(labels);
    text_del(&source);

    if ((result = write_bytes(argv[2], buf, buf_len)) != ZCC_OK)
    {
        free(buf);
        fprintf(stderr, "IO error %d!\n", result);
        return result;
    }
    free(buf);

    return ZCC_OK;
}
