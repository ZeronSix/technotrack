#include "io.h"
#include "common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int text_new(Text *this_, const char *filename)
{
    assert(filename);
    assert(this_);

    FILE *f = fopen(filename, "r");
    if (!f)
        return ZCC_ERR_OPEN_FILE;

    struct stat file_stat = {};
    if (fstat(fileno(f), &file_stat) != 0 || file_stat.st_size < 0)
    {
        fclose(f);
        return ZCC_ERR_FSTAT;
    }

    size_t size = (size_t)file_stat.st_size;

    char_t *buf = (char_t *)calloc(size + 1, sizeof(*buf));
    if (!buf)
    {
        fclose(f);
        return ZCC_ERR_ALLOC;
    }

    if (fread(buf, sizeof(*buf), size, f) != size)
    {
        fclose(f);
        return ZCC_ERR_READ_FILE;
    }

    this_->size = size + 1;
    this_->str  = buf;

    return ZCC_OK;
}

int text_del(Text *this_)
{
    if (!this_)
        return ZCC_OK;

    free(this_->str);
    this_->size = 0;
    this_->str  = NULL;

    return ZCC_OK;
}

int write_bytes(const char *filename, byte_t *buffer, size_t size)
{
    assert(filename);
    assert(buffer);

    FILE *fout = NULL;
    if (!(fout = fopen(filename, "wb")))
    {
        return ZCC_ERR_WRITE_FILE;
    }

    if (fwrite(buffer, size, 1, fout) != 1)
    {
        assert(0);
        return ZCC_ERR_WRITE_FILE;
    }

    fclose(fout);
    return ZCC_OK;
}

int read_bytes(const char *filename, byte_t *buffer, size_t max_size, size_t *size)
{
    assert(filename);
    assert(buffer);
    assert(max_size);
    assert(size);

    FILE *fin = NULL;
    if (!(fin = fopen(filename, "rb")))
    {
        return ZCC_ERR_READ_FILE;
    }

    size_t size_read = 0;
    if ((size_read = fread(buffer, 1, max_size, fin)) == 0)
    {
        return ZCC_ERR_WRITE_FILE;
    }
    
    fclose(fin);
    *size = size_read;
    return ZCC_OK;
}
