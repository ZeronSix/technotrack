#include "test.h"
#include "../tree.h"
#include "../io.h"
#include <stdbool.h>
#include <string.h>
#include <assert.h>

static bool TestTreeNodeCtor(void)
{
    TreeNode *node = TreeNodeCtor(1, "test");
    if (!node)
        return false;
    
    bool result = true;
    ASSERT_TREENODE_OK(node);
    if (node->type != 1)
        result = false;
    if (strcmp("test", node->data) != 0)
        result = false;

    TreeNodeDtor(node);
    return result;
}

static bool TestTreeNodeAttach(void)
{
    TreeNode *root = NULL;
    if (!(root = TreeNodeCtor(1, "root")))
        return false;

    TreeNode *child1 = NULL;
    if (!(child1 = TreeNodeCtor(1, "child1")))
        return false;

    TreeNode *child2 = NULL;
    if (!(child2 = TreeNodeCtor(1, "child2")))
        return false;

    TreeNodeAttachLeft(root, child1);
    TreeNodeAttachRight(child1, child2);
    if (root->left != child1 || root->left->right != child2)
        return false;

    TreeNodeDtor(root);
    return true;
}

static bool TestTreeNodeDetach(void)
{
    TreeNode *root = NULL;
    if (!(root = TreeNodeCtor(1, "root")))
        return false;
    TreeNode *child1 = NULL;
    if (!(child1 = TreeNodeCtor(1, "child1")))
        return false;

    TreeNodeAttachLeft(root, child1);
    TreeNodeDetach(child1);
    bool result = root->left == NULL && child1->parent == NULL;

    TreeNodeDtor(root);
    TreeNodeDtor(child1);
    return result;
}

static void TestTreeNodeDump(void)
{
    TreeNode *root = TreeNodeCtor(1, "root");
    TreeNode *child1 = TreeNodeCtor(2, "child1");
    TreeNode *child2 = TreeNodeCtor(3, "child3");
    TreeNode *child3 = TreeNodeCtor(4, "child4");
    TreeNode *child4 = TreeNodeCtor(5, "child5");

    TreeNodeAttachLeft(root, child1);
    TreeNodeAttachRight(child1, child2);
    TreeNodeAttachLeft(child2, child3);
    TreeNodeAttachRight(child2, child4);

    TREENODE_DUMP(root, "./Tests/TestDump.gv", "./Tests/TestDump.png");
}

static void TestTreeNodeSerialize(void)
{
    TreeNode *root = TreeNodeCtor(1, "root");
    TreeNode *child1 = TreeNodeCtor(2, "child1");
    TreeNode *child2 = TreeNodeCtor(3, "child3");
    TreeNode *child3 = TreeNodeCtor(4, "child4");
    TreeNode *child4 = TreeNodeCtor(5, "child5");

    TreeNodeAttachLeft(root, child1);
    TreeNodeAttachRight(child1, child2);
    TreeNodeAttachLeft(child2, child3);
    TreeNodeAttachRight(child2, child4);

    FILE *f = fopen("./Tests/TestSerialize.txt", "w");
    assert(f);
    TreeNodeSerialize(root, f);
    fclose(f);
}

bool TestTreeNodeDeserialize(void)
{
    Text *txt = TextReadFromFile("./Tests/TestSerialize.txt");
    assert(txt);

    TreeNode *node = NULL;
    assert(TreeNodeDeserialize(&node, txt->string) == TREE_OK);
    TextDtor(txt);
    
    FILE *f = fopen("./Tests/TestDeserialize.txt", "w");
    assert(f);
    TreeNodeSerialize(node, f);
    fclose(f);
    TreeNodeDtor(node);
    return system("diff ./Tests/TestSerialize.txt ./Tests/TestDeserialize.txt") == 0;
}

/*!
 * Run unit tests.
 */
int main(void)
{
    printf("# Running unit tests for TreeNode.\n");
    TEST_EQ(bool, "%d", TestTreeNodeCtor(), true);
    TEST_EQ(bool, "%d", TestTreeNodeAttach(), true);
    TEST_EQ(bool, "%d", TestTreeNodeDetach(), true);
    printf("# Testing TreeNodeDump - check manually\n");
    TestTreeNodeDump(); 
    printf("# Testing TreeNodeSerialize - check manually\n");
    TestTreeNodeSerialize();
    TEST_EQ(bool, "%d", TestTreeNodeDeserialize(), true);
    printf("# All tests have been done.\n"); }
