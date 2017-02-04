/*!
 * \file
 * Common code.
 */
#ifndef COMMON_H
#define COMMON_H 
#include <stdbool.h>

/*!
 * Free and return null.  */
#define FREE_AND_RET_NULL(_ptr_) { free(_ptr_); return NULL; }

/*!
 * Global error codes.
 */
enum
{
    DIFF_OK              = 0,
    DIFF_ERR_DIV_BY_ZERO = 256,
    DIFF_DOMAIN_ERROR    = 257
};

/*!
 * Double comparator.
 */
bool CompareDouble(double a, double b);
#endif /* ifndef COMMON_H */
