//
// Created by zeronsix on 07.11.16.
//

#ifndef LISTSANDHASHTABLES_HASHTABLE_H
#define LISTSANDHASHTABLES_HASHTABLE_H
#include "list.h"

/*!
 * Canary.
 */
enum
{
    MAP_CANARY = 0xFD
};

/*!
 * Hash type.
 */
typedef unsigned Hash;

/*!
 * Map error codes.
 */
enum
{
    MAP_OK = 0,
    MAP_ERR_ALLOC_FAIL = 1,
    MAP_ERR_LIST_INIT_FAIL = 2,
    MAP_ERR_BROKEN_PTR = 3,
    MAP_ERR_BROKEN_BUCKETS = 4,
    MAP_ERR_BROKEN_HASHFUNC = 5,
    MAP_ERR_INSERT_FAIL = 6,
    MAP_ERR_CANARY = 7,
    MAP_ERR_ITEM_NOT_FOUND = 8
};

/*!
 * Hashmap.
 */
typedef struct
{
    Canary left_canary;
    List *buckets;
    size_t size;
    size_t bucket_count;
    Hash (*hash_func)(T);
    Canary right_canary;
} Map;

#ifndef NDEBUG
/*!
 * Map assertion macro.
 */
#define ASSERT_MAP_OK(_mapptr_) { \
    int _err_code_ = MAP_ERR_BROKEN_PTR; \
    if (_mapptr_ == NULL || (_err_code_ = map_verify(_mapptr_)) != MAP_OK) \
    { \
        fprintf(stderr, "[%s:%s:%d] ASSERT_MAP_OK failed (error code %d).", \
                __FILE__, \
                __func__, \
                __LINE__, \
                _err_code_); \
        abort(); \
    } \
}
#else
#define ASSERT_MAP_OK(_lst_) ;
#endif

/*!
 * Map dump macro.
 */
#define MAP_DUMP(_mapptr_, _out1_, _out2_) { \
    FILE *_fptr_ = fopen(_out1_, "w"); \
    if (_fptr_) \
    { \
        fprintf(_fptr_, "digraph G { \nrandkir=\"LR\";\n"); \
        map_dump(_mapptr_, _fptr_); \
        fprintf(_fptr_, "}\n"); \
        fclose(_fptr_); \
        system("dot -Tpng " _out1_ " -o " _out2_); \
    } \
}

/*!
 *
 * @param this_
 * @param bucket_count
 * @return
 */
int map_ctor(Map *this_, size_t bucket_count, Hash (*hash_func)(T));
/*!
 *
 * @param this_
 */
void map_dtor(Map *this_);
/*!
 *
 * @param this_
 * @return
 */
int map_verify(const Map *this_);
/*!
 *
 * @param this_
 * @param dumpster
 */
void map_dump(const Map *this_, FILE *dumpster);
/*!
 *
 * @param this_
 * @param value
 * @return
 */
int map_insert(Map *this_, T value);
/*!
 *
 */
bool map_find(const Map *this_, T value);
/*!
 *
 * @param this_
 * @param value
 * @return
 */
int map_erase(Map *this_, T value);
/*!
 *
 * @param this_
 */
void map_clear(Map *this_);

#endif //LISTSANDHASHTABLES_HASHTABLE_H
