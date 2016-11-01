/*!
 * \file
 * Program that translates source code to binary code.
 */
#include "zasm.h"
#include "io.h"
#include "int_stack.h"
#include "ptr_stack.h"
#include <assert.h>
#include <stdio.h>  
#include <stdlib.h>

/*!
 * Maximum string command buffer size.
 */
#define BUF_SIZE 1024
/*!
 * Maximum command byte size.
 */
#define CMD_SIZE 32

/*!
 * Where to put output if no filename is specified.
 */
static const char *DEFAULT_OUT_FILENAME = "out.bin";

/*!
 * Read a double from input.
 */
static int read_const_arg(char **str_ptr, double *arg)
{
    assert(arg);

    double tmp = 0;
    int len = 0;
    DO_OR_RET_ERR(sscanf(*str_ptr, "%lg%n[^:]", &tmp, &len) != 1, ZCPU_ERR_WRONG_INPUT);
    *str_ptr += len;

    *arg = tmp;
    return ZCPU_OK;
}

/*!
 * Writes a byte to output buffer.
 */
static void write_byte(Byte **buf_ptr, char code)
{
    assert(buf_ptr);
    assert(*buf_ptr);
    **buf_ptr = code;
    (*buf_ptr)++;
}

/*!
 * Writes a double to output buffer.
 */
static void write_double(Byte **buf_ptr, double arg)
{
    assert(buf_ptr);
    assert(*buf_ptr);
    *(double *)*buf_ptr = arg;
    (*buf_ptr) += sizeof(double) / sizeof(Byte);
}

/*!
 * Shifts pointer to skip comments
 */
static void skip_comments(char **str_ptr)
{
    assert(str_ptr);
    assert(*str_ptr);

    char c = '\0';
    while (true)
    {
        if (sscanf(*str_ptr, " %c", &c) == 1)
        {
            if (c == ZCPU_ASM_COMMENT)
            {
                *str_ptr = strchr(*str_ptr, '\n') + 1;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
}

/*!
 * Check if the next symbol is comma.
 */
static int check_for_comma(char **str_ptr)
{
    assert(str_ptr);
    assert(*str_ptr);

    char c = '\0';
    int len = 0;
    if (sscanf(*str_ptr, " %c%n", &c, &len) == 1)
    {
        if (c == ',')
        {
            *str_ptr += len + 1;
            return ZCPU_OK;
        } 
    }

    return ZCPU_ERR_WRONG_INPUT;
}

/*!
 * Reads a command and shifts the pointer to the input buffer.
 */
static int read_cmd(char **str_ptr, int *code)
{
    assert(str_ptr);
    assert(*str_ptr);
    assert(code);

    char str[BUF_SIZE] = {};
    int cmd_len = 0;
    *code = CMD_UNKNOWN;
    if (sscanf(*str_ptr, " %s%n", str, &cmd_len) == 1)
    {
        CMD_READER(str_ptr, str, cmd_len, code, ZCPU_ERR_WRONG_INPUT);
    }  
    
    return ZCPU_OK;
}

/*!
 * Reads a register and shifts the pointer to the input buffer.
 */
static int read_reg(char **str_ptr, Byte *code)
{
    assert(str_ptr);
    assert(*str_ptr);
    assert(code);

    char str[BUF_SIZE] = {};
    int cmd_len = 0;
    *code = REG_UNKNOWN;
    if (sscanf(*str_ptr, " %[^, \t\n]%n", str, &cmd_len) == 1)
    {
        REG_READER(str_ptr, str, cmd_len, code, ZCPU_ERR_WRONG_INPUT);
    }  

    return ZCPU_OK;
}

/*!
 * Reads a label and shifts the pointer.
 */
static int read_label(char **str_ptr, unsigned *label)
{
    assert(str_ptr);
    assert(*str_ptr);
    assert(label);

    int lab_len = 0;
    unsigned tmp = 0;
    if (sscanf(*str_ptr, " %d:%n", &tmp, &lab_len) == 1)
    {
        *label = tmp; 
        *str_ptr += lab_len + 1;
    }
    else 
    {
        return ZCPU_ERR_NOT_A_LABEL;
    }

    return ZCPU_OK;
}

/*!
 * Assemblying cycle.
 */
static int run(TextBuffer *text, Byte *buffer, size_t *buf_len)
{
    assert(text);
    assert(buffer);
    assert(buf_len);
    
    char *str_ptr = text->buf;
    Byte *cur_ptr = buffer;
    int cmd_code = CMD_UNKNOWN;
    Byte* labels[MAX_LABEL] = {};
    PtrStack addrstack = {};
    IntStack labelstack = {};
    DO_OR_RET_ERR(ptrstack_new(&addrstack, MAX_LABEL_STACK), ZCPU_ERR_INIT);
    DO_OR_RET_ERR(intstack_new(&labelstack, MAX_LABEL_STACK), ZCPU_ERR_INIT);

    do
    {
        Byte addr_mode = ADDR_MODE_CONST;
        double arg = 0;
        Byte reg = 0;
        unsigned label = 0;

        skip_comments(&str_ptr);
        if (read_label(&str_ptr, &label) == ZCPU_OK)
        {
            if (labels[label] != NULL)
            {
                return ZCPU_ERR_LABEL_REDEFINITION;
            }
            labels[label] = cur_ptr;
        }
        skip_comments(&str_ptr);

        if (read_cmd(&str_ptr, &cmd_code) != ZCPU_OK)
        {
            return ZCPU_ERR_WRONG_INPUT;
        }
        if (cmd_code == CMD_UNKNOWN)
        {
            break;
        }

        write_byte(&cur_ptr, cmd_code);
        ARG_ASM_HANDLER(cmd_code);
    } while (cmd_code != CMD_UNKNOWN);

    // write label adresses
    size_t len = addrstack.index;
    for (size_t i = 0; i < len; i++)
    {
        void *addr = NULL;
        int label = 0;

        DO_OR_RET_ERR(ptrstack_pop(&addrstack, &addr), ZCPU_ERR_STACK_UNDERFLOW);
        DO_OR_RET_ERR(intstack_pop(&labelstack, &label), ZCPU_ERR_STACK_UNDERFLOW);
        *(double *)addr = labels[label] - buffer;
    }

    *buf_len = cur_ptr - buffer;
    ptrstack_del(&addrstack);
    intstack_del(&labelstack);
    return ZCPU_OK;
}

int main(int argc, const char **argv)
{
    if (argc != 3 && argc != 2)
    {
        fprintf(stderr, "Wrong arguments!\n");
        return ZCPU_ERR_WRONG_ARGS;
    }

    const char* in_filename = argv[1];
    const char* out_filename = argc == 2 ? DEFAULT_OUT_FILENAME : argv[2];
    
    TextBuffer text = {};
    if (read_source(&text, in_filename) != ZCPU_OK)
    {
        fprintf(stderr, "Failed to read %s!\n", in_filename);
        return ZCPU_ERR_FILE_READ;
    }

    Byte *buffer = (Byte *)calloc(text.line_count, sizeof(*buffer) * CMD_SIZE);
    if (!buffer)
    {
        fprintf(stderr, "Failed to allocate memory!\n");
        return ZCPU_ERR_MEM_ALLOC;
    }

    size_t size = 0;
    int result = run(&text, buffer, &size);
    if (result != ZCPU_OK)
    {
        switch (result)
        {
            case ZCPU_ERR_WRONG_INPUT:
                fprintf(stderr, "Wrong ASM code!\n");
                break;
            case ZCPU_ERR_CMD_NOT_IMPLEMENTED:
                fprintf(stderr, "ASM code has a command \
                                 that hasn't been implemented yet!\n");
                break;
            default:
                fprintf(stderr, "Unknown error\n");
        }
        return result;
    } 

    if (write_bytes(out_filename, buffer, size))
    {
        fprintf(stderr, "Failed to write binary!\n");
        return ZCPU_ERR_FILE_WRITE;
    }

    textbuf_del(&text);
    free(buffer);

    return ZCPU_OK;
}
