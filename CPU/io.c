#include "io.h"
#include <assert.h>
#include <stdlib.h>

int fopen_safe(FILE **ptr, const char *filename, const char *mode)
{
    assert(ptr);
    assert(filename);
    assert(mode);    

    FILE *f = fopen(filename, mode);
    if (!f)
    {
        return ZCPU_ERR_FILE_READ;
    }

    *ptr = f;
    return ZCPU_OK;
}

int fread_to_buffer(FILE *ptr, TextBuffer *textbuf)
{
    assert(ptr);
    assert(textbuf);

    if (fseek(ptr, 0, SEEK_END) != 0)
    {
        return ZCPU_ERR_FILE_READ;
    }
    long long fsize = ftell(ptr);
    if (fsize == -1)
    {
        return ZCPU_ERR_FILE_READ;
    }
    fseek(ptr, 0, SEEK_SET);

    char *buf = (char *)calloc((size_t)fsize + 1, sizeof(*buf));
    if (!buf)
    {
        return ZCPU_ERR_MEM_ALLOC;
    }
    textbuf->buf = buf;
    textbuf->alloc_size = (size_t)fsize + 1;

    size_t read_size = fread(buf, sizeof(*buf), (size_t)fsize, ptr);
    if (buf[read_size - 1] != '\n')
    {
        buf[read_size] = '\n';
        textbuf->size = read_size + 1;
    }
    else
    {
        textbuf->size = read_size;
    }
    for (size_t i = 0; i < read_size; i++)
    {
        if (buf[i] == '\n')
        {
            textbuf->line_count++;
        }
    }

    return ZCPU_OK;
}

void textbuf_del(TextBuffer *this_)
{
    if (this_ == NULL)
    {
        return;
    }

    free(this_->buf);
    this_->buf = 0;
    this_->size = 0;
    this_->alloc_size = 0;
}

int read_source(TextBuffer *text, const char *filename)
{
    assert(text);
    assert(filename);

    FILE *fin = NULL;
    if (fopen_safe(&fin, filename, "r") != ZCPU_OK)
    {
        return ZCPU_ERR_FILE_READ;
    }

    if (fread_to_buffer(fin, text) != ZCPU_OK)
    {
        return ZCPU_ERR_FILE_READ;
    }

    fclose(fin);
    return ZCPU_OK;
}

int write_bytes(const char *filename, Byte *buffer, size_t size)
{
    assert(filename);
    assert(buffer);

    FILE *fout = NULL;
    if (fopen_safe(&fout, filename, "wb") != ZCPU_OK)
    {
        return ZCPU_ERR_FILE_WRITE;
    }

    if (fwrite(buffer, size, 1, fout) != 1)
    {
        return ZCPU_ERR_FILE_WRITE;
    }

    fclose(fout);
    return ZCPU_OK;
}

int read_bytes(const char *filename, Byte *buffer, size_t max_size, size_t *size)
{
    assert(filename);
    assert(buffer);
    assert(max_size);
    assert(size);

    FILE *fin = NULL;
    if (fopen_safe(&fin, filename, "rb") != ZCPU_OK)
    {
        return ZCPU_ERR_FILE_WRITE;
    }

    size_t size_read = 0;
    if ((size_read = fread(buffer, max_size, 1, fin)) != 0)
    {
        return ZCPU_ERR_FILE_WRITE;
    }
    
    fclose(fin);
    *size = size_read;
    return ZCPU_OK;
    
}
