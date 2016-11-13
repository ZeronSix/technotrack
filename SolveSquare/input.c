/*!
 * \file
 * \author Vyacheslav "ZeronSix" Zhdanovskiy <zeronsix@gmail.com>
 *
 * A solution for the excercise 2.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*!
 * Default buffer size
 */
#define BUFFER_SIZE 1024

/*!
 * Reads a string from stdin to preallocated buffer.
 * \param[out] buffer The buffer to read to.
 * \param[in] max_len The maximum length of the buffer (including '\0')
 * \return The count of chars read (without '\0').
 */
int input_preallocated(char *buffer, size_t max_len);
/*!
 * Reads a string from stdin to statically buffer.
 * \return A pointer to the buffer.
 */
char *input_static(void);
/*!
 * Reads a string from stdin to dynamically allocated buffer.
 * If the reallocating process fails, the reading stops.
 * Don't forget to free() it!
 * \return A pointer to the buffer.
 */
char *input_dynamic(void);

int input_preallocated(char *buffer, size_t max_len)
{
    assert(buffer);

    int c = 0;
    size_t i = 0;

    for (i = 0; i < max_len - 1 && (c = getchar()) != EOF && c != '\n'; i++)
    {
        buffer[i] = c;
    }

    buffer[i] = '\0';
    return i;
}

char *input_static(void)
{
    static char buffer[BUFFER_SIZE] = "";

    int c = 0;
    size_t i = 0;

    for (i = 0; i < BUFFER_SIZE - 1 && (c = getchar()) != EOF && c != '\n'; i++)
    {
        buffer[i] = c;
    }

    buffer[i] = '\0';
    return buffer;
}

char *input_dynamic(void)
{
    char *buffer = (char *)calloc(BUFFER_SIZE, sizeof(*buffer));
    if (!buffer)
    {
        return NULL;
    }

    size_t current_alloc_size = BUFFER_SIZE;
    size_t i = 0;
    int c = 0;
    while ((c = getchar()) != EOF && c != '\n')
    {
        if (i == current_alloc_size - 1)
        {
            current_alloc_size *= 2;
            char *buf_new = (char *)realloc(buffer, current_alloc_size * sizeof(*buffer));
            if (!buf_new)
            {
                break;
            } 

            buffer = buf_new;
        }

        buffer[i] = c;
        i += 1;
    } 

    buffer[i] = '\0';
    return buffer;
}

/*
 * Contains tests for input_* functions.
 */
int main()
{
    char buffer[BUFFER_SIZE] = "";
    printf("# Using input with preallocated buffer...\n");
    input_preallocated(buffer, BUFFER_SIZE - 1);
    printf("%s\n", buffer);

    printf("# Using input with statically allocated buffer...\n");
    printf("%s\n", input_static());

    printf("# Using input with dynamically allocated buffer...\n");
    char *buffer_dyn = input_dynamic();
    if (buffer_dyn)
    {
        printf("%s\n", buffer_dyn);
    }
    else
    {
        fprintf(stderr, "Failed to allocate memory in input_dynamic!\n");
    }
    free(buffer_dyn);
    buffer_dyn = NULL;

    return 0;
}
