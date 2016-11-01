#include <assert.h>
#include "ptr_stack.h"

int ptrstack_new(PtrStack *this_, size_t alloc_size)
{
    assert(this_);
    assert(alloc_size > 0);

    void* *data = (void* *)calloc(alloc_size, sizeof(*data));
    if (!data)
    {
        return ZCPU_ERR_MEM_ALLOC;
    }
    this_->index = 0;
    this_->alloc_size = alloc_size;
    this_->data = data;

    return ZCPU_OK;
}

void ptrstack_del(PtrStack *this_)
{
    if (!this_)
        return;

    free(this_->data);
    this_->data = NULL;
    this_->index = 0;
    this_->alloc_size = 0;
}

int ptrstack_push(PtrStack *this_, void* item)
{
    assert(this_);
    if (this_->index == this_->alloc_size)
    {
        return ZCPU_ERR_STACK_OVERFLOW;
    }
    this_->data[this_->index++] = item;

    return ZCPU_OK;
}

int ptrstack_pop(PtrStack *this_, void* *item)
{
    assert(this_);

    if (this_->index == 0)
    {
        return ZCPU_ERR_STACK_UNDERFLOW;
    }
    this_->index--;
    if (item)
    {
        *item = this_->data[this_->index];
    }

    return ZCPU_OK;
}
