//
// Created by zeronsix on 12.11.16.
//
#ifndef LISTSANDHASHTABLES_TEST_H
#define LISTSANDHASHTABLES_TEST_H

/*!
 * Unit test macro.
 */
#define TEST_EQ(_expr_, _val_) { printf("   Testing { %s }... ", #_expr_); \
                                 printf(_expr_ == _val_ ? "correct!\n" : "incorrect!\n"); }

#endif //LISTSANDHASHTABLES_TEST_H
