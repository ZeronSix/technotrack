/*!
 * \file
 * Input-output library sources.
 */
#include "io.h"
#include <stdlib.h>
#include <assert.h>
#include "common.h"

Text *TextReadFromFile(const char *filename)
{
    assert(filename);
    
    Text *ptr = (Text *)calloc(1, sizeof(*ptr));
    if (!ptr)
        return NULL;

    FILE *f = fopen(filename, "r");
    if (!f)
        FREE_AND_RET_NULL(ptr);

    if (fseek(f, 0, SEEK_END) != 0) 
        FREE_AND_RET_NULL(ptr);

    long fsize = ftell(f);
    if (fsize == -1)
        FREE_AND_RET_NULL(ptr);
    char *buf = (char *)calloc(fsize + 1, sizeof(*buf));
    if (!buf)
    {
        free(buf);
        FREE_AND_RET_NULL(ptr);
    }
    fseek(f, 0, SEEK_SET);
    fread(buf, sizeof(*buf), fsize, f);

    ptr->size = fsize;
    ptr->string = buf;

    return ptr;
}

void TextDtor(Text *text)
{
    if (!text)
        return;

    free(text->string);
    text->size = 0;
    free(text);
}

