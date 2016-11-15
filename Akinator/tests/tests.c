/*!
 * \file
 * Unit tests for trees.
 */
#include <tree.h>
#include <stdbool.h>

/*!
 * Unit test macro.
 */
#define TEST_EQ(_expr_, _val_) { printf("   Testing { %s }... ", #_expr_); \
                                 printf(_expr_ == _val_ ? "correct!\n" : "incorrect!\n"); }

static bool test_treenode_ctor(void)
{
    TreeNode *node = NULL;
    if (treenode_ctor(&node, NULL, TREENODE_TYPE_NONE, "test"))
        return false;

    treenode_dtor(node);

    return true;
}

static bool test_treenode_attach(void)
{
    TreeNode *root = NULL;
    if (treenode_ctor(&root, NULL, TREENODE_TYPE_NONE, "test"))
        return false;

    TreeNode *child1 = NULL;
    if (treenode_ctor(&child1, root, TREENODE_TYPE_NONE, "test"))
        return false;
    treenode_attach_left(root, child1);

    TreeNode *child2 = NULL;
    if (treenode_ctor(&child2, root, TREENODE_TYPE_NONE, "test"))
        return false;
    treenode_attach_right(child1, child2);

    if (root->left != child1 || root->left->right != child2)
        return false;

    treenode_dtor(root);
    return true;
}

static void test_dump(void)
{
    TreeNode *root = NULL;
    if (treenode_ctor(&root, NULL, TREENODE_TYPE_NONE, "test"))
        return;

    TreeNode *child1 = NULL;
    if (treenode_ctor(&child1, root, TREENODE_TYPE_NONE, "test1"))
        return;
    treenode_attach_left(root, child1);

    TreeNode *child2 = NULL;
    if (treenode_ctor(&child2, root, TREENODE_TYPE_NONE, "test2"))
        return;
    treenode_attach_right(child1, child2);

    TreeNode *child3 = NULL;
    if (treenode_ctor(&child3, root, TREENODE_TYPE_NONE, "test3"))
        return;
    TreeNode *child4 = NULL;
    if (treenode_ctor(&child4, root, TREENODE_TYPE_NONE, "test4"))
        return;
    treenode_attach_left(child2, child4);
    treenode_attach_right(child2, child3);
    TREENODE_DUMP(root, "dump/test_dump.gv", "dump/test_dump.png");

    treenode_dtor(root);
}

/*!
 * Unit tests.
 * @return
 */
int main(void)
{
    printf("# Running unit tests for TreeNode.\n");
    TEST_EQ(test_treenode_ctor(), true);
    TEST_EQ(test_treenode_attach(), true);
    printf("    Testing dump...\n");
    test_dump();
    printf("# All tests have been done.");

    return 0;
}
