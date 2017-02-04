/*!
 * \file
 * Test library.
 */
#ifndef ZCC_TESTLIB_H
#define ZCC_TESTLIB_H 

#include <stdio.h>

/*!
 * Unit test macro.
 */
#define TEST_EQ(_type_, _fmt_, _expr_, _val_) \
{ \
    printf("Testing " #_expr_ " - "); \
    _type_ _result_ = _expr_; \
    _result_ == _val_ ? printf("correct!\n") : \
                printf("INCORRECT: expected " _fmt_ ", but got " _fmt_ ".\n", _val_, _result_); \
}

#endif /* ifndef ZCC_TESTLIB_H */
