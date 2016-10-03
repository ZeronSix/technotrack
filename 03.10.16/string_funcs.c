/*!
 * \file
 * \author Vyacheslav "ZeronSix" Zhdanovskiy <zeronsix@gmail.com>
 *
 * A solution for the excercise 3.
 */
#include <stdio.h>
#include <assert.h>

/*!
 * A simple macro for unit testing.
 * \param[in] x Expression to test.
 */
#define test(x) { printf("# Testing %s... ", #x); x ? printf("correct!\n") : printf("incorrect!\n"); }

/*!
 * An alternative to stdlib's strlen().
 */
size_t my_strlen(const char *str);
/*!
 * An alternative to stdlib's strncpy()
 */
char *my_strncpy(char *s, const char *ct, size_t num);
/*!
 * An alternative to stdlib's strncat()
 */
char *my_strncat(char *s, const char *ct, size_t num);
/*!
 * An alternative to stdlib's strcmp()
 */
int my_strcmp(const char *s1, const char *s2);
/*!
 * An alternative to stdlib's strstr()
 */
char *my_strstr(const char *cs, const char *ct);
/*!
 * An alternative to stdlib's strtok()
 */
char *my_strtok(char *s, const char *ct);

size_t my_strlen(const char *str)
{
    assert(str);
    int n;

    for (n = 0; *str != '\0'; str++)
        n++;

    return n;
}

char *my_strncpy(char *s, const char *ct, size_t num)
{
    assert(s);
    assert(ct);

    char *start = s;

    if (num == 0)
        return 0;

    size_t i = 0;
    while (i++ < num && (*s++ = *ct))
    {
        if (*ct)
            ct++;
    }
    *s = '\0';

    return start;
}

char *my_strncat(char *s, const char *ct, size_t num)
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

int my_strcmp(const char *s1, const char *s2)
{
    assert(s1);
    assert(s2);

    for ( ; *s1 == *s2; s1++, s2++)
        if (*s1 == '\0')
            return 0;
    return *s1 - *s2;
}

char *my_strstr(const char *cs, const char *ct)
{
    assert(cs);
    assert(ct);

    if (ct[0] == 0)
    {
        return (char *)cs;
    }

    while (*cs++)
    {
        const char *s1 = cs;
        const char *s2 = ct;
        for ( ; *s1 == *s2; s1++, s2++)
            ;
        
        if (*s2 == '\0')
        {
            return (char *)cs;
        }
    }

    return NULL;
}

#define TABLE_SIZE 256

char *my_strtok(char *s, const char *ct)
{
    static char *last_ptr = NULL;
    if (!last_ptr && !s)
    {
        return NULL;
    }

    char table[TABLE_SIZE] = {};
    table[(int)'\0'] = 1;
    while (*ct)
    {
        table[(int)*ct] = 1;
        ct++;
    }

    char *begin = s ? s : last_ptr;
    while (table[(int)*begin])
    {
        begin++;
    }

    char *end = begin;
    while (!table[(int)*end])
    {
        end++;
    }
    
    if (*end == '\0')
    {
        last_ptr = NULL;
    }
    else
    {
        last_ptr = end + 1;
    }

    *end = '\0';

    return begin;
}

/*
 * Default buffer size.
 */
#define TEST_BUFFER_SIZE 256

/*
 * Contains unit tests for my string functions.
 */
int main()
{
    char test_buf1[TEST_BUFFER_SIZE] = "";
    char test_buf2[TEST_BUFFER_SIZE] = "test";

    test(my_strlen("") == 0);
    test(my_strlen("Test") == 4);
    test(my_strcmp(my_strncpy(test_buf1, "abcdf", 4), "abcd") == 0);
    test(my_strcmp(my_strncpy(test_buf1, "abcdf", 6), "abcdf") == 0);
    test(my_strcmp(my_strncat(test_buf2, "abcdf", 4), "testabcd") == 0);
    test(my_strcmp(my_strstr("abctestabc", "test"), "testabc") == 0);
    test(my_strcmp(my_strstr("a", ""), "a") == 0);
    test(my_strstr("abc", "wxyz") == NULL);
    
    char s[] = "; , one,two; three";
    printf("\n# strtok test string:\n# %s\n", s);
    test(my_strcmp(my_strtok(s, ";, "), "one") == 0);
    test(my_strcmp(my_strtok(NULL, ";, "), "two") == 0);
    test(my_strcmp(my_strtok(NULL, ";, "), "three") == 0);
    test(my_strtok(NULL, ";, ") == NULL);

    return 0;
}
