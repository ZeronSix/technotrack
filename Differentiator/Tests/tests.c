#include "test.h"
#include "../tree.h"
#include "../io.h"
#include <stdbool.h>
#include <string.h>
#include <assert.h>

static bool TestDTreeNodeCtor(void)
{
    DTreeNode *node = DTreeNodeCtor(1, 0.0);
    if (!node)
        return false;
    
    bool result = true;
    ASSERT_TREENODE_OK(node);
    if (node->type != 1)
        result = false;
    if (!CMP_DOUBLE(0.0, node->data))
        result = false;

    DTreeNodeDtor(node);
    return result;
}

static bool TestDTreeNodeAttach(void)
{
    DTreeNode *root = NULL;
    if (!(root = DTreeNodeCtor(1, 0.0)))
        return false;

    DTreeNode *child1 = NULL;
    if (!(child1 = DTreeNodeCtor(1, 1.0)))
        return false;

    DTreeNode *child2 = NULL;
    if (!(child2 = DTreeNodeCtor(1, 2.0)))
        return false;

    DTreeNodeAttachLeft(root, child1);
    DTreeNodeAttachRight(child1, child2);
    if (root->left != child1 || root->left->right != child2)
        return false;

    DTreeNodeDtor(root);
    return true;
}

static bool TestDTreeNodeDetach(void)
{
    DTreeNode *root = NULL;
    if (!(root = DTreeNodeCtor(1, 1.0)))
        return false;
    DTreeNode *child1 = NULL;
    if (!(child1 = DTreeNodeCtor(1, 2.0)))
        return false;

    DTreeNodeAttachLeft(root, child1);
    DTreeNodeDetach(child1);
    bool result = root->left == NULL && child1->parent == NULL;

    DTreeNodeDtor(root);
    DTreeNodeDtor(child1);
    return result;
}

static void TestDTreeNodeDump(void)
{
    DTreeNode *root = DTreeNodeCtor(1, 0.0);
    DTreeNode *child1 = DTreeNodeCtor(2, 1.0);
    DTreeNode *child2 = DTreeNodeCtor(3, 2.0);
    DTreeNode *child3 = DTreeNodeCtor(4, 3.0);
    DTreeNode *child4 = DTreeNodeCtor(5, 4.0);

    DTreeNodeAttachLeft(root, child1);
    DTreeNodeAttachRight(child1, child2);
    DTreeNodeAttachLeft(child2, child3);
    DTreeNodeAttachRight(child2, child4);

    TREENODE_DUMP(root, "./Tests/TestDump.gv", "./Tests/TestDump.png");
    DTreeNodeDtor(root);
}

static void TestDTreeNodeSerialize(void)
{
    DTreeNode *root = DTreeNodeCtor(1, 0.0);
    DTreeNode *child1 = DTreeNodeCtor(2, 1.0);
    DTreeNode *child2 = DTreeNodeCtor(3, 2.0);
    DTreeNode *child3 = DTreeNodeCtor(4, 3.0);
    DTreeNode *child4 = DTreeNodeCtor(5, 4.0);

    DTreeNodeAttachLeft(root, child1);
    DTreeNodeAttachRight(child1, child2);
    DTreeNodeAttachLeft(child2, child3);
    DTreeNodeAttachRight(child2, child4);

    FILE *f = fopen("./Tests/TestSerialize.txt", "w");
    assert(f);
    DTreeNodeSerialize(root, f);
    fclose(f);
}

static bool TestDTreeNodeDeserialize(void)
{
    Text *txt = TextReadFromFile("./Tests/TestSerialize.txt");
    assert(txt);

    DTreeNode *node = NULL;
    assert(DTreeNodeDeserialize(&node, txt->string) == TREE_OK);
    TextDtor(txt);
    
    FILE *f = fopen("./Tests/TestDeserialize.txt", "w");
    assert(f);
    DTreeNodeSerialize(node, f);
    fclose(f);
    DTreeNodeDtor(node);
    return system("diff ./Tests/TestSerialize.txt ./Tests/TestDeserialize.txt") == 0;
}

/*!
 * Run unit tests.
 */
int main(void)
{
    printf("# Running unit tests for DTreeNode.\n");
    TEST_EQ(bool, "%d", TestDTreeNodeCtor(), true);
    TEST_EQ(bool, "%d", TestDTreeNodeAttach(), true);
    TEST_EQ(bool, "%d", TestDTreeNodeDetach(), true);
    printf("# Testing DTreeNodeDump - check manually\n");
    TestDTreeNodeDump(); 
    printf("# Testing DTreeNodeSerialize - check manually\n");
    TestDTreeNodeSerialize();
    TEST_EQ(bool, "%d", TestDTreeNodeDeserialize(), true);
    printf("# All tests have been done.\n"); 
}
