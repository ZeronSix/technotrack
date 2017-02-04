/*!
 * \file
 * Contains unit tests.
 */
#include <stdio.h>
#include <stdbool.h>
#include <list.h>
#include <hashmap.h>
#include <string.h>
#include "test.h"

// List tests.
static bool list_eq(List *lst, T arr[])
{
    ListItem *it = lst->front;
    size_t i = 0;

    while (it != NULL)
    {
        if (strcmp(it->value, arr[i]) != 0)
            return false;

        i++;
        it = it->next;
    }

    return true;
}

bool test_list_insert(void)
{
    T result[] = { "1", "2", "3", "4" };

    List lst = {};
    list_ctor(&lst);
    list_push_front(&lst, "1");
    ListItem *front = list_front(&lst);
    list_insert_after(front, "3");
    list_insert_before(front->next, "2");
    list_push_back(&lst, "4");

    if (lst.size != 4)
        return false;
    if (!list_eq(&lst, result))
        return false;
    listitem_dtor(front);
    list_dtor(&lst);

    return true;
}

bool test_list_erase(void)
{
    T result[] = {"3"};
    List lst = {};
    list_ctor(&lst);

    list_push_back(&lst, "1");
    list_push_back(&lst, "2");
    list_push_back(&lst, "3");
    list_erase(lst.front->next);
    list_erase(lst.front);

    if (!list_eq(&lst, result))
        return false;
    list_dtor(&lst);

    return true;
}

bool test_list_clear(void)
{
    T result[1] = {};
    List lst = {};
    list_ctor(&lst);

    list_push_back(&lst, "1");
    list_push_back(&lst, "2");
    list_push_back(&lst, "3");
    list_clear(&lst);

    if (!list_eq(&lst, result))
        return false;
    list_dtor(&lst);

    return true;
}

bool test_list_index(void)
{
    List lst = {};
    list_ctor(&lst);

    list_push_back(&lst, "1");
    list_push_back(&lst, "2");
    list_push_back(&lst, "3");

    ListItem *itm = NULL;
    if ((itm = list_index(&lst, "2")) == NULL || strcmp(itm->value, "2") != 0)
        return false;
    if ((itm = list_index(&lst, "4")) != NULL)
        return false;

    listitem_dtor(itm);
    list_dtor(&lst);

    return true;
}

// Hashmap tests
enum { TEST_MAP_BUCKET_SIZE = 10 };

Hash test_hash1(T v)
{
    return 2;
}

bool test_map_insert(void)
{
    Map map = {};
    map_ctor(&map, TEST_MAP_BUCKET_SIZE, test_hash1);
    map_insert(&map, "1");
    map_insert(&map, "2");
    if (map.size != 2)
        return false;

    ListItem *itm = NULL;
    if ((itm = list_index(&map.buckets[2], "2")) == NULL || strcmp(itm->value, "2") != 0)
        return false;
    if ((itm = list_index(&map.buckets[2], "1")) == NULL || strcmp(itm->value, "1") != 0)
        return false;
    listitem_dtor(itm);
    map_dtor(&map);

    return true;
}

bool test_map_search(void)
{
    Map map = {};
    map_ctor(&map, TEST_MAP_BUCKET_SIZE, test_hash1);
    map_insert(&map, "1");
    map_insert(&map, "2");

    if (!map_find(&map, "1") || !map_find(&map, "2") || map_find(&map, "3"))
        return false;

    map_dtor(&map);

    return true;
}

static Hash hash_djb2(char *s)
{
    Hash hash = 5381;
    int c;

    while ((c = *s))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        s++;
    }

    return hash;
}

void test_dump(void)
{
    Map map = {};
    map_ctor(&map, TEST_MAP_BUCKET_SIZE, hash_djb2);
    map_insert(&map, "1");
    map_insert(&map, "2");
    map_insert(&map, "3");
    map_insert(&map, "4");
    map_insert(&map, "5");
    map_insert(&map, "6");
    map_insert(&map, "7");
    MAP_DUMP(&map, "./dump/map.gv", "./dump/map.png");
    map_dtor(&map);
}

/*!
 * Unit test running function.
 * @return
 */
int main(void)
{
    printf("# Running unit tests for lists.\n");
    TEST_EQ(test_list_insert(), true);
    TEST_EQ(test_list_erase(), true);
    TEST_EQ(test_list_clear(), true);
    TEST_EQ(test_list_index(), true);

    printf("# Running unit tests for hashmaps.\n");
    TEST_EQ(test_map_insert(), true);
    TEST_EQ(test_map_search(), true);
    printf("    Testing dump...\n");
    test_dump();
    printf("# All tests have been finished.");

    return 0;
}