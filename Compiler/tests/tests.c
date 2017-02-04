/*!
 * \file
 * Contains unit tests.
 */
#include "testlib.h"
#include "../common.h"
#include "../io.h"
#include <string.h>

static int test_text_io(void)
{
#define FREE_TXT_AND_RET() { text_del(&txt); return result; }
    Text txt = {};
    int result = text_new(&txt, "./tests/test_text.txt");
    if (result != ZCC_OK)
        FREE_TXT_AND_RET();

    if (strcmp(txt.str, "TEST 123\n") != 0)
    {
        printf(" !%s! ", txt.str);
        result = ZCC_ERR_FAILED_TEST;
        FREE_TXT_AND_RET();
    }

    text_del(&txt);
    return ZCC_OK;
#undef FREE_TXT_AND_RET
}

int main()
{
    printf("# Running unit tests for IO module.\n");
    TEST_EQ(int, "%d", test_text_io(), ZCC_OK);
    printf("# All tests have been done.\n");
    return ZCC_OK;
}
