/*!
 * \file
 * \author Vyacheslav "ZeronSix" Zhdanovskiy <zeronsix@gmail.com>
 *
 * A solution for the excercise 3.
 */
#include <stdio.h>
#include <assert.h>

#define test(x) { printf("Testing %s... ", #x); x ? printf("correct!\n") : printf("incorrect!\n"); }

size_t strlen(const char *str);
char *strncpy(char *s, const char *ct, size_t num);
char *strncat(char *s, const char *ct, size_t num);
int strcmp(const char *s1, const char *s2);

size_t strlen(const char *str)
{
    assert(str);
    int n;

    for (n = 0; *str != '\0'; str++)
        n++;

    return n;
}

char *strncpy(char *s, const char *ct, size_t num)
{
    assert(s);
    assert(ct);

    if (num == 0)
        return 0;

    size_t i = 0;
    while (i++ < num && (*s++ = *ct))
    {
        if (*ct)
            ct++;
    }
    s[i] = '\0';

    return s;
}

char *strncat(char *s, const char *ct, size_t num)
{
    assert(s);
    assert(ct);

    size_t i = 0, j = 0;
    
    if (num == 0)
        return s;

    while (s[i])
        i++;

    while (j < num && (s[i++] = ct[j++]))
        ;

    s[i] = '\0';
    return s;
}

int strcmp(const char *s1, const char *s2)
{
    for ( ; *s1 == *s2; s1++, s2++)
        if (*s1 == '\0')
            return 0;
    return *s1 - *s2;
}

#define TEST_BUFFER_SIZE 256

int main()
{
    char test_buf1[TEST_BUFFER_SIZE] = "";
    char test_buf2[TEST_BUFFER_SIZE] = "test";

    test(strlen("") == 0);
    test(strlen("Test") == 4);
    test(strcmp(strncpy(test_buf1, "abcdf", 4), "abcd") == 0);
    test(strcmp(strncpy(test_buf1, "abcdf", 6), "abcdf") == 0);
    test(strcmp(strncat(test_buf2, "abcdf", 4), "testabcd") == 0);
    
    return 0;
}
