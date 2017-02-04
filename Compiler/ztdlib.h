/*!
 * Standard library.
 */
#ifndef ZTDLIB_H
#define ZTDLIB_H 
#include "hashmap.h"

/*!
 * Inject standard library code.
 */
int inject_ztdlib_code(void);
/*!
 * Inject end.
 */
int inject_program_end(void);
/*!
 * Instal ztdlib.
 */
int install_ztdlib_funcmap(FuncMap *funcmap);

#endif /* ifndef ZTDLIB_H */
