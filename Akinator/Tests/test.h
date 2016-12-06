#ifndef TEST_H
#define TEST_H 
/*!
 * Basic unit test library.
 */

#include <stdio.h>

/*!
 * Unit test macro.
 */
#define TEST_EQ(_type_, _fmt_, _expr_, _val_) { \
    printf("Testing " #_expr_ " - "); \
    _type_ _result_ = _expr_; \
    _result_ == _val_ ? printf("correct!\n") : \
                printf("INCORRECT: expected " _fmt_ ", but got " _fmt_ ".\n", _val_, _result_); \
}

#endif /* ifndef TEST_H */
