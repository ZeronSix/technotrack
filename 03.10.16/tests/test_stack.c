#include "../ez_common.h"
#include "../ez_stack.h"

DEF_STACK(int, IntStack, intstack)

#define TEST_BUFFER_SIZE 1024

int main(void)
{
    printf("# TESTING STACK\n");

    IntStack stack = {};
    intstack_new(&stack, TEST_BUFFER_SIZE);
    intstack_del(&stack);

    printf("# END TESTING STACK\n\n");
    return 0;
}
