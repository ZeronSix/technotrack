/*!
 * Input-output module.
 */
#ifndef ZCC_IO_H
#define ZCC_IO_H 
#include "common.h"
#include <stddef.h>

/*!
 * Read-only text buffer.
 */
typedef struct
{
    size_t size;
    char_t *str;
} Text;

/*!
 * Load text from file
 */
int text_new(Text *this_, const char *filename);
/*!
 * Free memory allocated for text.
 */
int text_del(Text *this_);
/*!
 * Write buffer to a file.
 */
int write_bytes(const char *filename, byte_t *buffer, size_t size);
/*!
 * Read file to a binary buffer.
 */
int read_bytes(const char *filename, byte_t *buffer, size_t max_size, size_t *size);

#endif /* ifndef ZCC_IO_H */
