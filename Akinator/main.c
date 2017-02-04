/*!
 * \file
 * Akinator.
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "io.h"
#include "tree.h"

/*!
 * Database filename.
 */
static const char *DATABASE = "database.tree";

/*!
 * Error codes.
 */
enum 
{
    AKN_OK     = 0,
    AKN_ERR_IO = 1
};

/*! 
 * Node types.
 */
enum
{
    AKN_OBJECT   = 1,
    AKN_QUESTION = 2
};

/*!
 * Menu keys.
 */
enum
{
    AKN_MENU_GUESS  = '1',
    AKN_MENU_DUMP   = '2',
    AKN_MENU_DEFINE = '3',
    AKN_MENU_CMP    = '4',
    AKN_MENU_EXIT   = '5',
    AKN_MENU_YES    = 'y',
    AKN_MENU_NO     = 'n'
};

/*!
 * Application run cycle.
 */
static int Run(void);

/*!
 * Usage: ---
 */
int main(void)
{
    puts("Добро пожаловать в Акинатор!");
    printf("База данных находится в: %s\n", DATABASE);

    int result = Run();
    switch (result)
    {
        case AKN_OK:
            break;
        default:
            puts("Неизвестная ошибка!");
            break;
    }
    
    return result;
}

/*!
 * Deserializes tree.
 */
static TreeNode *LoadDatabase(void)
{
    Text *txt = TextReadFromFile(DATABASE);
    if (!txt)
        return NULL;

    TreeNode *root = NULL;
    TreeNodeDeserialize(&root, txt->string);
    TextDtor(txt);

    return root;
}

/*!
 * Serialize the tree.
 */
static int SaveDatabase(const TreeNode *dbRoot)
{
    FILE *f = fopen(DATABASE, "w");
    if (f)
    {
        TreeNodeSerialize(dbRoot, f);
        fclose(f);
        return AKN_OK;
    }
    else
        return AKN_ERR_IO;
}

/*!
 * Creates new TreeNode.
 */
#define GET_NAME(_name_) \
    char _name_[TREENODE_DATA_BUFFER_SIZE] = ""; \
    while (*_name_ == '\0' || strchr(_name_, '\'') != NULL) \
    { \
        fgets(_name_, TREENODE_DATA_BUFFER_SIZE, stdin); \
        char *end = strchr(_name_, '\n'); \
        if (end) \
            *end = '\0'; /* delete a newline at the end of the string */ \
    } \

static TreeNode *InsertNewNode(int type)
{
    GET_NAME(name);
    return TreeNodeCtor(type, name);
}

/*!
 * Display the menu.
 */
static void DisplayMenu(void)
{
    fflush(stdout);
    puts("Выберите действие: ");
    printf("Угадать объект [%c]\n", AKN_MENU_GUESS);
    printf("Дамп базы данных [%c]\n", AKN_MENU_DUMP);
    printf("Вывести определение [%c]\n", AKN_MENU_DEFINE);
    printf("Сравнить объекты [%c]\n", AKN_MENU_CMP);
    printf("Выход [%c]\n", AKN_MENU_EXIT);
}

/*!
 * Dumps the tree.
 */
static void DumpDatabase(const TreeNode *dbRoot)
{
    puts("Сохранение дампа базы данных...");
    TREENODE_DUMP(dbRoot, "AkinatorDump.gv", "AkinatorDump.png");
    system("firefox AkinatorDump.png &");
}

/*!
 * Guess function.
 */
static void Guess(TreeNode **root)
{
    assert(root);
    assert(*root);
    
    TreeNode *node = *root;

    while (node->type == AKN_QUESTION)
    {
        printf("Верно, что - %s?[%c%c]\n", node->data, AKN_MENU_YES, AKN_MENU_NO);
        while (true)
        {
            char c = tolower(getchar());
            if (c == AKN_MENU_YES)
                node = node->left;
            else if (c == AKN_MENU_NO)
                node = node->right;
            else
                continue;
            break;
        }
    }

    printf("Ваш объект - %s?[%c%c]\n", node->data, AKN_MENU_YES, AKN_MENU_NO);
    while (true)
    {
        char c = tolower(getchar());
        if (c == AKN_MENU_YES)
            puts("Ура!\n");
        else if(c == AKN_MENU_NO)
        {
            puts("Но что же это?");
            TreeNode *newNode = InsertNewNode(AKN_OBJECT);
            assert(newNode);
            printf("Чем это от отличается от %s?\n", node->data);
            TreeNode *question = InsertNewNode(AKN_QUESTION);
            assert(question);
            if (node->parent == NULL)
                *root = question;
            else
            {
                TreeNode *parent = node->parent;
                bool isLeft = parent->left == node;
                TreeNodeDetach(node);
                if (isLeft)
                    TreeNodeAttachLeft(parent, question);
                else
                    TreeNodeAttachRight(parent, question);
            }
            TreeNodeAttachLeft(question, newNode);
            TreeNodeAttachRight(question, node);
        }
        else
            continue;
        break;
    }
}

static void Define(TreeNode *root)
{
    assert(root);
    GET_NAME(name);
    
    TreeNode *obj = TreeNodeSearch(root, AKN_OBJECT, name);
    if (!obj)
    {
        puts("Объект не найден!\n");
        return; 
    }

    TreeNode *node = obj->parent;
    TreeNode *prev = obj;
    if (!node)
        return;

    printf("Объект %s можно охарактеризовать как:", obj->data);
    while (node != NULL)
    {
        if (node->right == prev)
            printf(" не");
        printf(" %s", node->data);
        prev = node;
        node = node->parent;
    }
    puts("\n");
}   

static void Compare(TreeNode *root)
{
    assert(root);
    GET_NAME(name1);
    GET_NAME(name2);

    TreeNode *obj1 = TreeNodeSearch(root, AKN_OBJECT, name1);
    TreeNode *obj2 = TreeNodeSearch(root, AKN_OBJECT, name2);
    if (!obj1 || !obj2)
    {
        puts("Объекты не найдены!\n");
        return; 
    }

    TreeNode *node = obj1->parent;
    TreeNode *prev = NULL;
    bool metCommon = false;
    bool isRightSide = false;
    while (node != NULL)
    {
        if (metCommon)
        {
            if (node->right == prev)
                isRightSide = true;
            else
                isRightSide = false;
            if (isRightSide)
                printf(" не");
            printf(" %s", node->data);
        }
        else if (TreeNodeIsParent(node, obj2)) 
        {
            printf("Различия: %s\n", node->data);
            metCommon = true;
            printf("Сходства:");
            if (node->parent == NULL)
            {
                printf(" нет");
                break;
            }
        }
        prev = node;
        node = node->parent;
    }

    puts("\n");
}

int Run(void)
{
    TreeNode *dbRoot = LoadDatabase();
    if (!dbRoot)
    {
        puts("База данных пуста, добавьте новый объект:");
        dbRoot = InsertNewNode(AKN_OBJECT);
    }

    bool shouldExit = false;
    while (!shouldExit)
    {
        DisplayMenu();
        char c = getchar();
        switch (c)
        {
            case AKN_MENU_GUESS:
                Guess(&dbRoot);
                break;
            case AKN_MENU_DEFINE:
                Define(dbRoot);
                break;
            case AKN_MENU_DUMP:
                DumpDatabase(dbRoot);
                break;
            case AKN_MENU_CMP:
                Compare(dbRoot);
                break;
            case AKN_MENU_EXIT:
                shouldExit = true;
                break;     
            default:
                break;
        }
    }
        
    if (SaveDatabase(dbRoot) != AKN_OK)
        return AKN_ERR_IO;

    TreeNodeDtor(dbRoot);
    return AKN_OK;
}
