/*!
 * \file
 * Input-output library.
 */
#ifndef IO_H
#define IO_H 
#include <stdio.h>

/*!
 * Input-output library.
 */

/*!
 * Read-only text-buffer.
 */
typedef struct
{
    char *string; 
    size_t size;
} Text;

/*!
 * Reads text from file.
 */
Text *TextReadFromFile(const char *filename);
/*!
 * Text destructor.
 */
void TextDtor(Text *text);

#endif /* ifndef IO_H */
