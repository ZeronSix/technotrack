/*!
 * \file
 * \author Vyacheslav "ZeronSix" Zhdanovskiy <zeronsix@gmail.com>
 *
 * A solution for the excercises 4, 5, 6.
 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum
{
    ONG_OK = 0,
    ONG_ERR_WRONG_ARGS,
    ONG_ERR_FILE_OPEN,
    ONG_ERR_FTELL_ERR,
    ONG_ERR_ALLOC
};

typedef struct 
{
    char *str;
    size_t len;
} String;

typedef struct
{
    size_t size;
    size_t allocated_size;
    size_t str_count;

    char *buffer;
    String *strings;
} Text;

int read_text(Text *text, const char *filename);
void close_text(Text *text);
int strcmp_skip_punctuation(const void *p1, const void *p2);
int strcmp_skip_punctuation_reversed(const void *p1, const void *p2);
int write_text(Text *text, const char *filename);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Wrong arguments!\n");
        return ONG_ERR_WRONG_ARGS;
    }

    Text text = {};
    int file_err = read_text(&text, argv[1]);
    if (file_err)
    {
        fprintf(stderr, "Text reading error!\n");
        return file_err;
    }
    write_text(&text, "original.txt");

    qsort(text.strings, text.str_count, sizeof(*text.strings), strcmp_skip_punctuation);
    write_text(&text, "sorted.txt");

    qsort(text.strings, text.str_count, sizeof(*text.strings), strcmp_skip_punctuation_reversed);
    write_text(&text, "sorted_rhymes.txt");

    close_text(&text);
    return 0;
}

int read_text(Text *text, const char *filename)
{
    assert(text);
    assert(filename);

    FILE *fi = fopen(filename, "r");
    if (!fi)
    {
        return ONG_ERR_FILE_OPEN;
    }

    fseek(fi, 0, SEEK_END);
    long fsize = ftell(fi);
    if (fsize < 0)
    {
        return ONG_ERR_FTELL_ERR; 
    }
    fseek(fi, 0, SEEK_SET);

    char *buf = (char *)calloc(fsize + 1, sizeof(*buf));
    if (!buf)
    {
        return ONG_ERR_ALLOC;
    }
    size_t size_read = fread(buf, 1, fsize, fi);
    text->buffer = buf;
    text->allocated_size = fsize;
    text->size = size_read;
    
    fclose(fi);

    char *start = buf;
    while ((buf = strchr(buf, '\n')))
    {
        text->str_count++;
        buf++;
    }

    buf = start;
    
    String *str_ptrs = (String *)calloc(text->str_count, sizeof(*str_ptrs));
    if (!str_ptrs)
    {
        return ONG_ERR_ALLOC;
    }
    text->strings = str_ptrs;

    size_t i = 0;
    while ((buf = strchr(buf, '\n')))
    {
        text->strings[i].str = start;
        text->strings[i].len = buf - start - 1;
        *buf = '\0';
        start = ++buf;
        i++;
    }

    return ONG_OK;
}

void close_text(Text *text)
{
    assert(text);

    free(text->strings);
    free(text->buffer);
    text->str_count = 0;
    text->allocated_size = 0;
    text->size = 0;
}

static bool is_punctuation(unsigned char c)
{
    return ispunct(c) || isspace(c);
}

int strcmp_skip_punctuation(const void *p1, const void *p2)
{
    assert(p1);
    assert(p2);

    const unsigned char *s1 = (const unsigned char *)((String *)p1)->str;
    const unsigned char *s2 = (const unsigned char *)((String *)p2)->str;

    while (true)
    {
        while (is_punctuation(*s1))
            s1++;
        while (is_punctuation(*s2))
            s2++;

        if (*s1 == *s2)
        {
            if (*s1 == '\0')
            {
                return 0;
            }
        }
        else
        {
            return *s1 - *s2;
        }

        s1++;
        s2++;
    }
}

int strcmp_skip_punctuation_reversed(const void *p1, const void *p2)
{
    assert(p1);
    assert(p2);

    String* struct1 = (String *)p1;
    String* struct2 = (String *)p2;

    const unsigned char *s1 = (const unsigned char *)struct1->str;
    const unsigned char *s2 = (const unsigned char *)struct2->str;
    
    const unsigned char *t1 = s1 + struct1->len;
    const unsigned char *t2 = s2 + struct2->len;
    while (t1 - s1 >= 0 && t2 - s2 >= 0)
    {
        while (is_punctuation(*t1))
            t1--;
        while (is_punctuation(*t2))
            t2--;

        if (*t1 != *t2)
        {
            break;
        }    
        t1--;
        t2--;
    }
    if (t1 - s1 == -1 && t2 - s2 == -1)
        return 0;
    if (t1 - s1 == -1)
        t1 = s1 + struct1->len;
    if (t2 - s2 == -1)
        t2 = s2 + struct2->len;

    return *t1 - *t2;
}

int write_text(Text *text, const char *filename)
{
    assert(text);
    assert(text->buffer);

    FILE *fo = fopen(filename, "w");
    if (!fo)
    {
        return ONG_ERR_FILE_OPEN;
    }
    
    for (size_t i = 0; i < text->str_count; i++)
    {
        fputs(text->strings[i].str, fo);
        fputc('\n', fo);
    }
    fclose(fo);

    return ONG_OK;
}
