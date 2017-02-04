//
// Created by zeronsix on 07.11.16.
//

#include "hashmap.h"
#include <assert.h>
#include <stdlib.h>

int map_ctor(Map *this_, size_t bucket_count, Hash (*hash_func)(T))
{
    assert(this_);
    assert(bucket_count);
    assert(hash_func);
    List *buckets = (List *)calloc(bucket_count, sizeof(List));
    for (size_t i = 0; i < bucket_count; i++)
    {
        if (list_ctor(buckets + i) != LST_OK)
            return MAP_ERR_LIST_INIT_FAIL;
    }

    if (buckets == NULL)
        return MAP_ERR_ALLOC_FAIL;
    this_->left_canary = MAP_CANARY;
    this_->right_canary = MAP_CANARY;
    this_->buckets = buckets;
    this_->bucket_count = bucket_count;
    this_->hash_func = hash_func;
    this_->size = 0;

    ASSERT_MAP_OK(this_);

    return MAP_OK;
}

void map_dtor(Map *this_)
{
    if (!this_)
        return;

    if (this_->buckets)
    {
        for (size_t i = 0; i < this_->bucket_count; i++)
            list_dtor(this_->buckets + i);
        free(this_->buckets);
        this_->buckets = NULL;
        this_->bucket_count = 0;
    }
    this_->size = 0;
    this_->hash_func = NULL;
}

int map_verify(const Map *this_)
{
    assert(this_);
    if (!this_->hash_func)
        return MAP_ERR_BROKEN_HASHFUNC;
    if (this_->bucket_count == 0)
        return MAP_ERR_BROKEN_BUCKETS;
    if (!this_->buckets)
        return MAP_ERR_BROKEN_BUCKETS;

    for (size_t i = 0; i < this_->bucket_count; i++)
    {
        if (list_verify(this_->buckets + i) != LST_OK)
            return MAP_ERR_BROKEN_BUCKETS;
    }

    return MAP_OK;
}

int map_insert(Map *this_, T value)
{
    ASSERT_MAP_OK(this_);
    size_t hash = this_->hash_func(value) % this_->bucket_count;

    List *lst = this_->buckets + hash;
    if (lst->size == 0 || list_index(lst, value) == NULL)
        list_push_front(lst, value);
    this_->size += 1;

    ASSERT_MAP_OK(this_);
    return MAP_OK;
}

bool map_find(const Map *this_, T value)
{
    ASSERT_MAP_OK(this_);

    size_t hash = this_->hash_func(value) % this_->bucket_count;
    List *lst = this_->buckets + hash;
    ListItem *itm = list_index(lst, value);
    if (itm)
    {
        listitem_dtor(itm);
        return true;
    }
    else
        return false;
}

int map_erase(Map *this_, T value)
{
    ASSERT_MAP_OK(this_);

    size_t hash = this_->hash_func(value) % this_->bucket_count;
    List *lst = this_->buckets + hash;
    ListItem *it = list_index(lst, value);
    if (!it)
        return MAP_ERR_ITEM_NOT_FOUND;
    list_erase(it);
    this_->size--;

    return MAP_OK;
}

void map_clear(Map *this_)
{
    ASSERT_MAP_OK(this_);

    for (size_t i = 0; i < this_->bucket_count; i++)
        list_clear(this_->buckets + i);
    this_->size--;
}

void map_dump(const Map *this_, FILE *dumpster)
{
    assert(this_);
    assert(dumpster);

    fprintf(dumpster, "map_%p [label=\"Map[%p]\n"
                    "{\n"
                    "    left_canary = 0x%x;\n"
                    "    size = %lu;\n"
                    "    bucket_count = %lu;\n"
                    "    func = %p\n"
                    "    right_canary = 0x%x;\n"
                    "}\n\"]\n", this_, this_, this_->left_canary,
                    this_->size, this_->bucket_count, this_->hash_func, this_->right_canary);
    if (!this_->buckets)
        return;

    // fprintf(dumpster, "map_%p -> list_%p\n", this_, this_->buckets);
    list_dump(this_->buckets + 0, dumpster);
    for (size_t i = 1; i < this_->bucket_count; i++)
    {
        list_dump(this_->buckets + i, dumpster);
        //fprintf(dumpster, "list_%p -> list_%p\n",  this_->buckets + i - 1, this_->buckets + i);
    }
}

