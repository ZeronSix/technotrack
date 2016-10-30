/*!
 * Basic functions for file handling.
 */
#ifndef IO_H
#define IO_H 

#include <stdio.h>
#include "common.h"

/*!
 * Text string structure.
 */
typedef struct 
{
    char *buf;
    size_t size;
    size_t alloc_size;
    size_t line_count;
} TextBuffer;

/*!
 * Safe fopen alternative
 */
int fopen_safe(FILE **ptr, const char *filename, const char *mode);
/*!
 * Read file to TextBuffer.
 */
int fread_to_buffer(FILE *ptr, TextBuffer *textbuf);
/*!
 * TextBuffer destructor.
 */
void textbuf_del(TextBuffer *this_);
/*!
 * Reads source to a TextBuffer.
 */
int read_source(TextBuffer *text, const char *filename);
/*!
 * Write buffer to a file.
 */
int write_bytes(const char *filename, Byte *buffer, size_t size);
/*!
 * Read file to a binary buffer.
 */
int read_bytes(const char *filename, Byte *buffer, size_t max_size, size_t *size);

#endif /* ifndef IO_H */
