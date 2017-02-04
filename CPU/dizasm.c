/*!
 * \file
 * \author Vyacheslav "ZeronSix" Zhdanovskiy <zeronsix@gmail.com>
 */ 
#include "dizasm.h"
#include "io.h"
#include <stdio.h>
#include <assert.h>

/*!
 * Default filename for output.
 */
static const char *DEFAULT_OUT_FILENAME = "out.zasm";

/*!
 * Maximum input buffer size.
 */
enum { MAX_BUF_SIZE = 1000000 };

/*! 
 * Reads an argument
 */
static int read_arg(Byte **buffer, double *arg)
{
    assert(buffer);
    assert(*buffer);
    assert(arg);

    *arg = *(double *)(*buffer);
    *buffer += sizeof(double) / sizeof(Byte);

    return ZCPU_OK;
}

/*
 * Dizasm goes here.
 */
int run(FILE *fout, Byte *buffer)
{
    assert(fout);
    assert(buffer);

    Byte *buf_ptr = buffer;
    while (*buf_ptr)
    {
        CMD_WRITER(fout, *buf_ptr)
        buf_ptr++;

        double arg = 0;
        switch (*(buf_ptr - 1))
        {
            case CMD_PUSH:
                if (read_arg(&buf_ptr, &arg) != ZCPU_OK)
                {
                    return ZCPU_ERR_WRONG_BINARY;
                }
                fprintf(fout, " %lg", arg);
                break; 
            default:
                break;
        }

        fputc('\n', fout);
    } 

    return ZCPU_OK;
}

/*!
 *
 */
int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        fprintf(stderr, "Wrong arguments!\n");
        return ZCPU_ERR_WRONG_ARGS;
    }        

    Byte buffer[MAX_BUF_SIZE + 1] = {};
    size_t buf_size = 0;
    const char *in_filename = argv[1];

    if (read_bytes(in_filename, buffer, MAX_BUF_SIZE, &buf_size) != ZCPU_OK)
    {
        fprintf(stderr, "Can't read %s\n", in_filename);
        return ZCPU_ERR_FILE_READ;
    }

    const char *out_filename = argc == 3 ? argv[2] : DEFAULT_OUT_FILENAME;
    FILE *fout = NULL;
    if (fopen_safe(&fout, out_filename, "w") != ZCPU_OK)
    {
        fprintf(stderr, "Can't write %s\n", out_filename); 
        return ZCPU_ERR_FILE_WRITE;
    }

    int result = run(fout, buffer);
    if (result != ZCPU_OK)
    {
        fprintf(stderr, "Error code: %d\n", result);
        return result;
    }

    fclose(fout);

    return ZCPU_OK;
}
