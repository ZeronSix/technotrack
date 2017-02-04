#include "stack.h"
#include <assert.h>

int doublestack_new(DoubleStack *this_, size_t alloc_size)
{
    assert(this_);
    assert(alloc_size > 0);

    double *data = (double *)calloc(alloc_size, sizeof(*data));
    if (!data)
    {
        return ZCC_ERR_ALLOC;
    }
    this_->index = 0;
    this_->alloc_size = alloc_size;
    this_->data = data;

    return ZCC_OK;
}

void doublestack_del(DoubleStack *this_)
{
    if (!this_)
        return;

    free(this_->data);
    this_->data = NULL;
    this_->index = 0;
    this_->alloc_size = 0;
}

int doublestack_push(DoubleStack *this_, double item)
{
    assert(this_);
    if (this_->index == this_->alloc_size)
    {
        return ZCC_ERR_STACK_OVERFLOW;
    }
    this_->data[this_->index++] = item;

    return ZCC_OK;
}

int doublestack_pop(DoubleStack *this_, double *item)
{
    assert(this_);

    if (this_->index == 0)
    {
        return ZCC_ERR_STACK_UNDERFLOW;
    }
    this_->index--;
    if (item)
    {
        *item = this_->data[this_->index];
    }

    return ZCC_OK;
}

int intstack_new(IntStack *this_, size_t alloc_size)
{
    assert(this_);
    assert(alloc_size > 0);

    int *data = (int *)calloc(alloc_size, sizeof(*data));
    if (!data)
    {
        return ZCC_ERR_ALLOC;
    }
    this_->index = 0;
    this_->alloc_size = alloc_size;
    this_->data = data;

    return ZCC_OK;
}

void intstack_del(IntStack *this_)
{
    if (!this_)
        return;

    free(this_->data);
    this_->data = NULL;
    this_->index = 0;
    this_->alloc_size = 0;
}

int intstack_push(IntStack *this_, int item)
{
    assert(this_);
    if (this_->index == this_->alloc_size)
    {
        return ZCC_ERR_STACK_OVERFLOW;
    }
    this_->data[this_->index++] = item;

    return ZCC_OK;
}

int intstack_pop(IntStack *this_, int *item)
{
    assert(this_);

    if (this_->index == 0)
    {
        return ZCC_ERR_STACK_UNDERFLOW;
    }
    this_->index--;
    if (item)
    {
        *item = this_->data[this_->index];
    }

    return ZCC_OK;
}

int ptrstack_new(PtrStack *this_, size_t alloc_size)
{
    assert(this_);
    assert(alloc_size > 0);

    void* *data = (void* *)calloc(alloc_size, sizeof(*data));
    if (!data)
    {
        return ZCC_ERR_ALLOC;
    }
    this_->index = 0;
    this_->alloc_size = alloc_size;
    this_->data = data;

    return ZCC_OK;
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
        return ZCC_ERR_STACK_OVERFLOW;
    }
    this_->data[this_->index++] = item;

    return ZCC_OK;
}

int ptrstack_pop(PtrStack *this_, void* *item)
{
    assert(this_);

    if (this_->index == 0)
    {
        return ZCC_ERR_STACK_UNDERFLOW;
    }
    this_->index--;
    if (item)
    {
        *item = this_->data[this_->index];
    }

    return ZCC_OK;
}

void *ptrstack_get(PtrStack *this_, size_t index)
{
    assert(this_);
    if (this_->index <= index)
        return NULL;
    return this_->data[index];
}

int sizestack_new(SizeStack *this_, size_t alloc_size)
{
    assert(this_);
    assert(alloc_size > 0);

    size_t *data = (size_t *)calloc(alloc_size, sizeof(*data));
    if (!data)
    {
        return ZCC_ERR_ALLOC;
    }
    this_->index = 0;
    this_->alloc_size = alloc_size;
    this_->data = data;

    return ZCC_OK;
}

void sizestack_del(SizeStack *this_)
{
    if (!this_)
        return;

    free(this_->data);
    this_->data = NULL;
    this_->index = 0;
    this_->alloc_size = 0;
}

int sizestack_push(SizeStack *this_, size_t item)
{
    assert(this_);
    if (this_->index == this_->alloc_size)
    {
        return ZCC_ERR_STACK_OVERFLOW;
    }
    this_->data[this_->index++] = item;

    return ZCC_OK;
}

int sizestack_pop(SizeStack *this_, size_t *item)
{
    assert(this_);

    if (this_->index == 0)
    {
        return ZCC_ERR_STACK_UNDERFLOW;
    }
    this_->index--;
    if (item)
    {
        *item = this_->data[this_->index];
    }

    return ZCC_OK;
}

size_t sizestack_get(SizeStack *this_, size_t index)
{
    assert(this_);
    if (this_->index <= index)
        return 0;
    return this_->data[index];
}
