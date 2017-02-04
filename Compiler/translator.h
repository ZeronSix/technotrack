/*!
 * \file
 * ASM translator.
 */
#ifndef ZCC_TRANSLATOR_H
#define ZCC_TRANSLATOR_H 
#include "tree.h"
#include "hashmap.h"

/*! 
 * Translator.
 */
typedef struct
{
    VarMap *var_scope;
    FuncMap *funcmap;
    size_t while_count;
    size_t if_count;
} Translator;

/*!
 * Translate to asm.
 */
int translate(Translator *tr, TreeNode *root);

#endif /* ifndef ZCC_TRANSLATOR_H */
