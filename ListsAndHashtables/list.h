/*!
 * \file
 * \author Vyacheslav "ZeronSix" Zhdanovskiy
 * Contains doubly-linked list.
 */
#ifndef LISTSANDHASHTABLES_LIST_H
#define LISTSANDHASHTABLES_LIST_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/*!
 * Canary.
 */
enum
{
    LST_CANARY = 0xFD
};
typedef int Canary;

/*!
 * List error codes.
 */
enum
{
    LST_OK = 0,
    LST_ERR_ALLOC_FAIL = 1,
    LST_ERR_BROKEN_PTR = 2,
    LST_ERR_BROKEN_LINK = 3,
    LST_ERR_CANARY = 4,
    LST_ERR_OUT_OF_BOUNDS = 5,
    LST_ERR_BROKEN_RC = 6
};

typedef char * T;

struct List;
typedef struct List List;

struct ListItem
{
    Canary left_canary;
    T value;
    struct List *list;
    struct ListItem *prev;
    struct ListItem *next;
    size_t rc;
    Canary right_canary;
};

/*!
 * A container that holds the value of a list element.
 * WARNING! Only to be created with dynamic memory allocation.
 */
typedef struct ListItem ListItem;

#ifndef NDEBUG
/*!
 * ListITEM assertion macro.
 */
#define ASSERT_LISTITEM_OK(_itmptr_) { \
    int _err_code_ = LST_ERR_BROKEN_PTR; \
    if (_itmptr_ == NULL || (_err_code_ = listitem_verify(_itmptr_)) != LST_OK) \
    { \
        fprintf(stderr, "[%s:%s:%d] ASSERT_LISTITEM_OK failed (error code %d).\n", \
                __FILE__, \
                __func__, \
                __LINE__, \
                _err_code_); \
        abort(); \
    } \
}
#else
#define ASSERT_LISTITEM_OK(_lst_) ;
#endif

/*!
 * List item constructor.
 * @param this_ pointer to ListItem.
 * @param value Value to hold.
 * @return Error code.
 */
int listitem_ctor(ListItem *this_, List *lst, T value);
/*!
 * List item destructor.
 * @param this_ pointer to ListItem.
 */
void listitem_dtor(ListItem *this_);
/*!
 * ListItem verifier.
 * @param this_
 */
int listitem_verify(const ListItem *this_);
/*!
 * List item dump method.
 * @param this_
 * @param dumpster
 */
void listitem_dump(const ListItem *this_, FILE *dumpster);
/*!
 * Returns pointer to the next element.
 * @param this_
 * @return
 */
ListItem *listitem_next(const ListItem *this_);
/*!
 * Return pointer to the previous element.
 * @param prev_
 * @return
 */
ListItem *listitem_prev(const ListItem *prev_);

/*!
 * Doubly-linked list.
 */
struct List
{
    Canary left_canary;
    ListItem *front;
    ListItem *back;
    size_t size;
    Canary right_canary;
};

#ifndef NDEBUG
/*!
 * List assertion macro.
 */
#define ASSERT_LIST_OK(_lstptr_) { \
    int _err_code_ = LST_ERR_BROKEN_PTR; \
    if (_lstptr_ == NULL || (_err_code_ = list_verify(_lstptr_)) != LST_OK) \
    { \
        fprintf(stderr, "[%s:%s:%d] ASSERT_LIST_OK failed (error code %d).\n", \
                __FILE__, \
                __func__, \
                __LINE__, \
                _err_code_); \
        abort(); \
    } \
}
#else
#define ASSERT_LIST_OK(_lst_) ;
#endif

/*!
 * List Constructor.
 * @param this_ a pointer to List.
 * @return  Error code.
 */
int list_ctor(List *this_);
/*!
 * List Destructor.
 * @param this_ a pointer to List.
 */
void list_dtor(List *this_);
/*!
 * List verifier.
 * @param this_ a pointer to List.
 * @return error code.
 */
int list_verify(const List *this_);
/*!
 * List dump method.
 * @param this_
 * @param dumpster
 */
void list_dump(const List *this_, FILE *dumpster);
/*!
 * Access the first element.
 */
ListItem * list_front(const List *this_);
/*!
 * Access the last element.
 */
ListItem * list_back(const List *this_);
/*!
 * Push front.
 * @param this_
 * @param value
 * @return
 */
int list_push_front(List *this_, T value);
/*!
 *
 * @param this_
 * @param value
 * @return
 */
int list_push_back(List *this_, T value);
/*!
 *
 * @param this_
 * @param value
 * @return
 */
int list_insert_before(ListItem *it, T value);
/*!
 *
 * @param this_
 * @param value
 * @return
 */
int list_insert_after(ListItem *it, T value);
/*!
 *
 * @param it
 * @param value
 * @return
 */
int list_erase(ListItem *it);
/*!
 *
 * @param this_
 * @param value
 * @return
 */
ListItem *list_index(const List *this_, T value);
/*!
 *
 * @param this_
 * @param index
 * @return
 */
ListItem *list_iterate(const List *this_, size_t index);
/*!
 *
 * @param this_
 * @return
 */
void list_clear(List *this_);

#endif //LISTSANDHASHTABLES_LIST_H
