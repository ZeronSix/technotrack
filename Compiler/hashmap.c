#include "hashmap.h"
#include "common.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum 
{
    HASHMAP_SIZE = 1001
};

typedef unsigned Hash;

struct VarListItem
{
    struct VarListItem *next;
    const char      *name;
    size_t          offset; 
};

typedef struct VarListItem VarListItem;

static Hash hash(const char *s)
{
    assert(s);
    
    unsigned hashval = 0;
    for (hashval = 0; *s != '\0'; s++)
        hashval = (Hash)*s + 31 * hashval;
    return hashval % HASHMAP_SIZE;
}

struct VarMap
{
    VarListItem *buckets[HASHMAP_SIZE];
    size_t   size;
};

VarMap *varmap_new(void)
{
    return (VarMap *)calloc(1, sizeof(VarMap));
}

int varmap_del(VarMap *this_)
{
    if (!this_)
        return ZCC_OK;

    for (size_t i = 0; i < HASHMAP_SIZE; i++)
    {
        VarListItem *next = this_->buckets[i];
        for (VarListItem *np = this_->buckets[i]; next != NULL;)
        {
            next = np->next;
            free(np);
        }    
    }

    free(this_);
    return ZCC_OK;
}

size_t *varmap_search(VarMap *this_, const char *name)
{
    assert(this_);
    assert(name);

    for (VarListItem *np = this_->buckets[hash(name)]; np != NULL; np = np->next)
    {
        if (strcmp(name, np->name) == 0)
            return &np->offset;
    }
    return NULL;
}

int varmap_insert(VarMap *this_, const char *name, size_t offset)
{
    assert(this_);
    assert(name);

    Hash hashval = 0;
    size_t *np = varmap_search(this_, name);
    if (np == NULL)
    {
        VarListItem *itm = (VarListItem *)calloc(1, sizeof(*itm));
        if (!itm)
            return ZCC_ERR_ALLOC;
        hashval = hash(name);
        itm->next = this_->buckets[hashval];
        itm->offset = offset;
        itm->name = name;
        this_->buckets[hashval] = itm;
        this_->size++;
    }
    else
        *np = offset;

    return ZCC_OK;
}

struct FuncListItem
{
    struct FuncListItem *next;
    const char          *name;
    Func                func;
};

typedef struct FuncListItem FuncListItem;

struct FuncMap
{
    FuncListItem *buckets[HASHMAP_SIZE];
    size_t   size;
};

FuncMap *funcmap_new(void)
{
    return (FuncMap *)calloc(1, sizeof(FuncMap));
}

int funcmap_del(FuncMap *this_)
{
    if (!this_)
        return ZCC_OK;

    for (size_t i = 0; i < HASHMAP_SIZE; i++)
    {
        FuncListItem *next = this_->buckets[i];
        for (FuncListItem *np = this_->buckets[i]; next != NULL;)
        {
            next = np->next;
            free(np);
        }    
    }

    free(this_);
    return ZCC_OK;
}

Func *funcmap_search(FuncMap *this_, const char *name)
{
    assert(this_);
    assert(name);

    for (FuncListItem *np = this_->buckets[hash(name)]; np != NULL; np = np->next)
    {
        if (strcmp(name, np->name) == 0)
            return &np->func;
    }
    return 0;
}

int funcmap_insert(FuncMap *this_, const char *name, size_t args, size_t locals)
{
    assert(this_);
    assert(name);

    Hash hashval = 0;
    Func *np = funcmap_search(this_, name);
    if (!np)
    {
        FuncListItem *itm = (FuncListItem *)calloc(1, sizeof(*itm));
        if (!itm)
            return ZCC_ERR_ALLOC;
        hashval = hash(name);
        itm->next = this_->buckets[hashval];
        itm->func.args = args;
        itm->func.locals = locals;
        itm->name = name;
        this_->buckets[hashval] = itm;
        this_->size++;
    }

    return ZCC_OK;
}

size_t varmap_size(const VarMap *this_)
{
    return this_->size;
}

struct LabelListItem
{
    struct LabelListItem *next;
    char      *name;
    size_t          offset; 
};

typedef struct LabelListItem LabelListItem;

struct LabelMap
{
    LabelListItem *buckets[HASHMAP_SIZE];
    size_t   size;
};

LabelMap *labelmap_new(void)
{
    return (LabelMap *)calloc(1, sizeof(LabelMap));
}

int labelmap_del(LabelMap *this_)
{
    if (!this_)
        return ZCC_OK;

    for (size_t i = 0; i < HASHMAP_SIZE; i++)
    {
        LabelListItem *next = this_->buckets[i];
        for (LabelListItem *np = this_->buckets[i]; next != NULL;)
        {
            next = np->next;
            free(np->name);
            free(np);
        }    
    }

    free(this_);
    return ZCC_OK;
}

size_t *labelmap_search(LabelMap *this_, const char *name)
{
    assert(this_);
    assert(name);

    for (LabelListItem *np = this_->buckets[hash(name)]; np != NULL; np = np->next)
    {
        if (strcmp(name, np->name) == 0)
            return &np->offset;
    }
    return NULL;
}

int labelmap_insert(LabelMap *this_, const char *name, size_t offset)
{
    assert(this_);
    assert(name);

    Hash hashval = 0;
    size_t *np = labelmap_search(this_, name);
    if (np == NULL)
    {
        LabelListItem *itm = (LabelListItem *)calloc(1, sizeof(*itm));
        if (!itm)
            return ZCC_ERR_ALLOC;
        hashval = hash(name);
        itm->next = this_->buckets[hashval];
        itm->offset = offset;
        char *newname = (char *)calloc(strlen(name) + 1, sizeof(*newname));
        strcpy(newname, name);
        assert(newname);
        itm->name = newname;
        this_->buckets[hashval] = itm;
        this_->size++;
    }
    else
        *np = offset;

    return ZCC_OK;
}
