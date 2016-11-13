//
// Created by zeronsix on 06.11.16.
//
#include "list.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>

int listitem_ctor(ListItem *this_, List *lst, T value)
{
    assert(this_);
    assert(lst);

    this_->value = value;
    this_->left_canary = LST_CANARY;
    this_->list = lst;
    this_->next = NULL;
    this_->prev = NULL;
    this_->rc = 1;
    this_->right_canary = LST_CANARY;

    return LST_OK;
}

void listitem_dtor(ListItem *this_)
{
    if (this_ == NULL)
        return;

    this_->rc--;
    if (this_->rc == 0)
        free(this_);
}

void listitem_dump(const ListItem *this_, FILE *dumpster)
{
    assert(this_);
    assert(dumpster);

    fprintf(dumpster, "litem_%p [label=\"ListItem[0x%p]\n"
                      "{\n"
                      "    left_canary = 0x%x;\n"
                      "    value = %p;\n"
                      "    rc = %lu;\n"
                      "    right_canary = 0x%x;\n"
                      "}\n\"]\n", this_, this_, this_->left_canary, &this_->value,
                      this_->rc, this_->right_canary);
}

int listitem_verify(const ListItem *this_)
{
    assert(this_);

    if (this_->rc == 0)
        return LST_ERR_BROKEN_RC;
    if (this_->left_canary != LST_CANARY || this_->right_canary != LST_CANARY)
        return LST_ERR_CANARY;
    if (this_->list == NULL)
        return LST_ERR_BROKEN_PTR;
    return LST_OK;
}

int list_ctor(List *this_)
{
    assert(this_);
    this_->front = NULL;
    this_->back = NULL;
    this_->left_canary = LST_CANARY;
    this_->right_canary = LST_CANARY;
    this_->size = 0;

    ASSERT_LIST_OK(this_);
    return LST_OK;
}

void list_dtor(List *this_)
{
    if (this_ == NULL)
        return;

    list_clear(this_);
}

int list_verify(const List *this_)
{
    assert(this_);

    if (this_->left_canary != LST_CANARY || this_->right_canary != LST_CANARY)
        return LST_ERR_CANARY;
    if (this_->front == NULL && this_->back != NULL)
        return LST_ERR_BROKEN_LINK;
    if (this_->front != NULL && this_->back == NULL)
        return LST_ERR_BROKEN_LINK;
    if (this_->front == NULL && this_->size != 0)
        return LST_ERR_BROKEN_LINK;

    if (this_->front != NULL)
    {
        size_t size = 0;
        ListItem *it = this_->front;
        ListItem *prev = NULL;
        do
        {
            int it_ok = listitem_verify(it);
            if (it_ok != LST_OK)
                return it_ok;

            if (it->prev != prev)
                return LST_ERR_BROKEN_LINK;
            prev = it;
            size++;
        } while ((it = it->next) != NULL);
        if (prev != this_->back)
            return LST_ERR_BROKEN_LINK;
        if (size != this_->size)
            return LST_ERR_BROKEN_LINK;
    }

    return LST_OK;
}

void list_dump(const List *this_, FILE *dumpster)
{
    assert(this_);
    assert(dumpster);

    fprintf(dumpster, "list_%p [label=\"List[0x%p]\n"
                    "{\n"
                    "    left_canary = 0x%x;\n"
                    "    size = %lu;\n"
                    "    right_canary = 0x%x;\n"
                    "}\n\"]\n", this_, this_, this_->left_canary,
                    this_->size, this_->right_canary);

    if (this_->front != NULL)
    {
        fprintf(dumpster, "list_%p -> litem_%p\n", this_, this_->front);
        ListItem *it = this_->front;
        while (it != NULL)
        {
            if (it->prev != NULL)
                fprintf(dumpster, "litem_%p -> litem_%p\n", it, it->prev);
            if (it->next != NULL)
                fprintf(dumpster, "litem_%p -> litem_%p\n", it, it->next);

            listitem_dump(it, dumpster);
            it = it->next;
        }
    }
    if (this_->back != NULL && this_->front != this_->back)
    {
        fprintf(dumpster, "list_%p -> litem_%p\n", this_, this_->back);
    }
}

ListItem * list_front(const List *this_)
{
    ASSERT_LIST_OK(this_);

    if (this_->front == NULL)
        return NULL;

    this_->front->rc++;
    ASSERT_LIST_OK(this_);
    return this_->front;
}

ListItem * list_back(const List *this_)
{
    ASSERT_LIST_OK(this_);

    if (this_->back == NULL)
        return NULL;

    this_->back->rc++;
    ASSERT_LIST_OK(this_);
    return this_->back;
}

ListItem *listitem_next(const ListItem *this_) {
    ASSERT_LISTITEM_OK(this_);
    return this_->next;
}

ListItem *listitem_prev(const ListItem *this_) {
    ASSERT_LISTITEM_OK(this_);
    return this_->prev;
}

static ListItem *create_item(List *lst, T value)
{
    ListItem *it = (ListItem *)calloc(1, sizeof(ListItem));
    if (!it)
        return NULL;
    listitem_ctor(it, lst, value);
    return it;
}

static void insert_to_empty(List *lst, ListItem *it)
{
    lst->front = it;
    lst->back = it;
    lst->size++;
}

int list_push_front(List *this_, T value)
{
    ASSERT_LIST_OK(this_);

    if (this_->front == NULL)
    { ListItem *it = create_item(this_, value);
        if (it == NULL)
            return LST_ERR_ALLOC_FAIL;

        insert_to_empty(this_, it);
    }
    else
        return list_insert_before(this_->front, value);

    return LST_OK;
}

int list_insert_before(ListItem *it, T value)
{
    ASSERT_LISTITEM_OK(it);
    ASSERT_LIST_OK(it->list);
    ListItem *new_it = create_item(it->list, value);
    if (new_it == NULL)
        return LST_ERR_ALLOC_FAIL;

    if (it->prev)
        it->prev->next = new_it;
    else
        it->list->front = new_it;
    new_it->prev = it->prev;
    it->prev = new_it;
    it->list->size++;
    new_it->next = it;

    ASSERT_LIST_OK(it->list);
    return LST_OK;
}

int list_insert_after(ListItem *it, T value)
{
    ASSERT_LISTITEM_OK(it);
    ASSERT_LIST_OK(it->list);
    ListItem *new_it = create_item(it->list, value);
    if (new_it == NULL)
        return LST_ERR_ALLOC_FAIL;

    if (it->next)
        it->next->prev = new_it;
    else
        it->list->back = new_it;
    new_it->next = it->next;
    it->next = new_it;
    new_it->prev = it;
    it->list->size++;

    ASSERT_LIST_OK(it->list);
    return LST_OK;
}

int list_push_back(List *this_, T value)
{
    ASSERT_LIST_OK(this_);

    if (this_->back == NULL)
    {
        ListItem *it = create_item(this_, value);
        if (it == NULL)
            return LST_ERR_ALLOC_FAIL;

        insert_to_empty(this_, it);
    }
    else
        return list_insert_after(this_->back, value);

    ASSERT_LIST_OK(this_);
    return LST_OK;
}

int list_erase(ListItem *it)
{
    ASSERT_LISTITEM_OK(it);
    ASSERT_LIST_OK(it->list);
    assert(it->list->size);

    it->list->size--;
    if (it->prev == NULL)
        it->list->front = it->next;
    else
        it->prev->next = it->next;

    if (it->next == NULL)
        it->list->back = it->prev;
    else
        it->next->prev = it->prev;
    listitem_dtor(it);

    ASSERT_LIST_OK(it->list);
    return LST_OK;
}

ListItem *list_index(const List *this_, T value)
{
    ASSERT_LIST_OK(this_);

    ListItem *it = this_->front;
    while (it != NULL)
    {
        if (it->value == value || strcmp(it->value, value) == 0)
        {
            it->rc++;
            return it;
        }
        it = it->next;
    }

    return NULL;
}

ListItem *list_iterate(const List *this_, size_t index)
{
    ASSERT_LIST_OK(this_);

    if (index >= this_->size)
        return NULL;

    ListItem *it = this_->front;
    size_t i = 0;
    while (it != NULL)
    {
        if (i == index)
        {
            it->rc++;
            return it;
        }
        i++;
        it = it->next;
    }

    return NULL;
}

void list_clear(List *this_)
{
    ASSERT_LIST_OK(this_);
    ListItem *it = this_->back;
    while (it != NULL)
    {
        ListItem *cur = it;
        it = it->prev;
        listitem_dtor(cur);
    }
    this_->size = 0;
    this_->front = NULL;
    this_->back = NULL;
}

