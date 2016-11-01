#ifndef COMMON_H
#define COMMON_H 

#include "stdbool.h"

#define DO_OR_RET_ERR(_expr_, _err_) { if ((_expr_) != ZCPU_OK) return _err_; }

/*!
 * Typedef for byte
 */
typedef unsigned char Byte;

/*!
 * Error codes.
 */
enum 
{
    ZCPU_OK = 0,
    ZCPU_ERR_STACK_OVERFLOW = 1,
    ZCPU_ERR_STACK_UNDERFLOW = 2,
    ZCPU_ERR_MEM_ALLOC = 3,
    ZCPU_ERR_INIT = 4,
    ZCPU_ERR_WRONG_INPUT = 5,
    ZCPU_ERR_WRONG_CMD = 6,
    ZCPU_ERR_WRONG_ARGS = 7,
    ZCPU_ERR_FILE_READ = 8,
    ZCPU_ERR_CMD_NOT_IMPLEMENTED = 9,
    ZCPU_ERR_FILE_WRITE = 10,
    ZCPU_ERR_WRONG_BINARY = 11,
    ZCPU_ERR_UNKNOWN_ADDR_MODE = 12,
    ZCPU_ERR_UNKNOWN_REGISTER = 13,
    ZCPU_ERR_OUT_OF_BOUNDS = 14,
    ZCPU_ERR_NOT_A_LABEL = 15,
    ZCPU_ERR_LABEL_REDEFINITION = 16
};

#define ZCPU_ASM_COMMENT ';'

#endif /* ifndef COMMOM_H */
