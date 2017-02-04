/*!
 * \file
 * Hashmap.
 */
#ifndef ZCC_HASHMAP_H
#define ZCC_HASHMAP_H
#include <stddef.h>

/*!
 * Hashmap.
 */
typedef struct VarMap VarMap;

VarMap  *varmap_new(void); 
int     varmap_del(VarMap *this_);
size_t  *varmap_search(VarMap *this_, const char *name);
int     varmap_insert(VarMap *this_, const char *name, size_t offset);
size_t  varmap_size(const VarMap *this_);

/*!
 * Hashmap for functions.
 */
typedef struct FuncMap FuncMap;
typedef struct 
{
    size_t args;
    size_t locals;
} Func;

FuncMap *funcmap_new(void); 
int     funcmap_del(FuncMap *this_);
Func    *funcmap_search(FuncMap *this_, const char *name);
int     funcmap_insert(FuncMap *this_, const char *name, size_t args, size_t locals);

/*!
 * Hashmap for labels.
 */
typedef struct LabelMap LabelMap;

LabelMap *labelmap_new(void); 
int      labelmap_del(LabelMap *this_);
size_t   *labelmap_search(LabelMap *this_, const char *name);
int      labelmap_insert(LabelMap *this_, const char *name, size_t offset);

#endif
