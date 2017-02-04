#ifndef ZCC_COMMON_H
#define ZCC_COMMON_H 

/*!
 * Error codes.
 */
enum
{
    ZCC_OK                   = 0,
    ZCC_ERR_WRONG_ARGS       = 1,
    ZCC_ERR_OPEN_FILE        = 2,
    ZCC_ERR_FSTAT            = 3,
    ZCC_ERR_ALLOC            = 4,
    ZCC_ERR_READ_FILE        = 5,
    ZCC_ERR_FAILED_TEST      = 6,
    ZCC_ERR_UNEXPECTED_END   = 7,
    ZCC_ERR_EXPECTED_BRACKET = 8,
    ZCC_ERR_UNKNOWN_ID       = 9,
    ZCC_ERR_FUNC_REDEF       = 10,
    ZCC_ERR_EXPECTED_ID      = 11,
    ZCC_ERR_EXPECTED_EXPR    = 12,
    ZCC_ERR_EXPECTED_ASSIGN  = 13,
    ZCC_ERR_VAR_REDEF        = 14,
    ZCC_ERR_EXPECTED_FUNC    = 15,
    ZCC_ERR_EXPECTED_SEMICOLON = 16,
    ZCC_ERR_EXPECTED_OP      = 17,
    ZCC_ERR_DIVISION_BY_ZERO = 18,
    ZCC_ERR_WRONG_ASM        = 19,
    ZCC_ERR_EXPECTED_CONST   = 20,
    ZCC_ERR_STACK_OVERFLOW   = 21,
    ZCC_ERR_STACK_UNDERFLOW  = 22,
    ZCC_ERR_UNKNOWN_CMD      = 23,
    ZCC_ERR_WRITE_FILE       = 24,
    ZCC_ERR_LABEL_REDEF      = 25,
    ZCC_ERR_EXPECTED_LABEL   = 26,
    ZCC_ERR_LABEL_NOT_FOUND  = 27,
    ZCC_ERR_OUT_OF_BOUNDS    = 28
};

/*!
 * Typedef for easy switching between char types.
 */
typedef char char_t;

/*!
 * Typedef for byte.
 */
typedef unsigned char byte_t;

/*!
 * Do or return error.
 */
#define DO_OR_RET_ERR(_expr_, _err_) { if ((_expr_) != ZCC_OK) return _err_; }

#endif /* ifndef ZCC_COMMON_H */
