#ifndef EZ_STACK_H
#define EZ_STACK_H 

#include "ez_common.h"

enum
{
    EZ_CANARY = 0xCE,
    EZ_POISON = -666
};
typedef int Canary;

const char EZ_STACK_DUMP_FORMAT_START[] = "%s \"%s\" [%p]\n" \
                                          "{\n" \
                                          "   data[%lu] = %p\n";
const char EZ_STACK_DUMP_FORMAT_MID[]   = "      %c[%lu] = ";
const char EZ_STACK_DUMP_FORMAT_END[]   = "   count = [%lu]\n" \
                                          "}\n";

#define STACK_DATA_LEFT_CANARY(__THIS__) *((char *)__THIS__->data)
#define STACK_DATA_RIGHT_CANARY(__THIS__) *((char *)(__THIS__->data \
                                          + __THIS__->alloc_size))

#define DEF_STACK(__TYPE__, __NAME__, __PREFIX__) typedef struct \
{ \
    Canary canary_left; \
    size_t count; \
    size_t alloc_size; \
    __TYPE__ *data; \
    Canary canary_right; \
} __NAME__; \
\
void __PREFIX__ ## _dump(__NAME__ *this_) \
{ \
    fprintf(stdout, EZ_STACK_DUMP_FORMAT_START, #__NAME__, #__TYPE__, \
            (void *)this_, this_->alloc_size, (void *)this_->data); \
} \
\
bool __PREFIX__ ## _verify(__NAME__ *this_) \
{ \
    assert(this_); \
    bool result = this_->count > 0 && \
                  this_->data && \
                  this_->alloc_size > 0 && \
                  this_->canary_left == EZ_CANARY && \
                  this_->canary_right == EZ_CANARY && \
                  STACK_DATA_LEFT_CANARY(this_) == EZ_CANARY && \
                  STACK_DATA_RIGHT_CANARY(this_) == EZ_CANARY; \
    if (!result) \
    { \
        __PREFIX__ ## _dump(this_); \
    } \
    return result; \
} \
int __PREFIX__ ## _new(__NAME__ *this_, size_t alloc_size) \
{\
    assert(this_); \
    assert(alloc_size > 0); \
    \
    __TYPE__ *data = (__TYPE__ *)calloc(alloc_size, sizeof(__TYPE__)); \
    if (!data) \
    { \
        return EZ_ERR_MEMORY_ALLOC; \
    } \
    this_->canary_left = EZ_CANARY; \
    this_->count = 0; \
    this_->alloc_size = alloc_size; \
    this_->canary_right = EZ_CANARY; \
    this_->data = data; \
    STACK_DATA_LEFT_CANARY(this_) = EZ_CANARY; \
    STACK_DATA_RIGHT_CANARY(this_) = EZ_CANARY; \
    \
    return __PREFIX__ ## _verify(this_) ? EZ_OK : EZ_ERR_VERIFY; \
}\
\
void __PREFIX__ ## _del(__NAME__ *this_) \
{ \
    if (!this_) \
        return; \
    \
    __PREFIX__ ## _verify(this_); \
    this_->canary_left = EZ_POISON; \
    this_->count = 0; \
    this_->canary_right = 0; \
    this_->alloc_size = 0; \
    free(this_->data); \
}\
int __PREFIX__ ## _push(__NAME__ *this_, __TYPE__ item) \
{ \
    assert(this_); \
    if (!__PREFIX__ ## _verify(this_)) \
        return EZ_ERR_STACK_INCORRECT_PUSH;  \
    if (this_->count == this_->alloc_size) \
    { \
        __PREFIX__ ## _dump(this_); \
        return EZ_ERR_STACK_OVERFLOW; \
    } \
    this_->data[this_->count++] = item; \
    if (!__PREFIX__ ## _verify(this_)) \
        return EZ_ERR_STACK_INCORRECT_PUSH;  \
    return EZ_OK; \
} \
int __PREFIX__ ## _pop(__NAME__ *this_, __TYPE__ *item) \
{ \
    assert(this_); \
    if (!__PREFIX__ ## _verify(this_)) \
        return EZ_ERR_STACK_INCORRECT_POP;  \
    if (this_->count == 0) \
    { \
        __PREFIX__ ## _dump(this_); \
        return EZ_ERR_STACK_UNDERFLOW; \
    } \
    __TYPE__ popped_item = this_->data[this_->count]; \
    *(int *)(this_->data + this_->count--) = EZ_POISON; \
    if (!__PREFIX__ ## _verify(this_)) \
        return EZ_ERR_STACK_INCORRECT_POP;  \
    *item = popped_item; \
    return EZ_OK; \
} 

#endif /* ifndef EZ_STACK_H */
