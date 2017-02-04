/*!
 * \file
 * Virtual machine architecture.
 */
#ifndef VMARCH_H
#define VMARCH_H

enum {
    ZASM_MAX_CMD_LEN = 128,
    ZASM_MAX_LABEL_LEN = 256
};

enum
{
    ADDR_MODE_CONST = 0,
    ADDR_MODE_STACK = 1
};

/*!
 * Command codes.
 */
enum
{
    CMD_UNKNOWN = 0,
    CMD_PUSH = 128,
    CMD_POP  = 129,
    CMD_ADD  = 130,
    CMD_SUB  = 131,
    CMD_MUL  = 132,
    CMD_DIV  = 133,
    CMD_AND  = 134,
    CMD_OR   = 135,
    CMD_ABV  = 136,
    CMD_BLW  = 137,
    CMD_AOE  = 138,
    CMD_BOE  = 139,
    CMD_EQ   = 140,
    CMD_RET  = 141,
    CMD_CALL = 142,
    CMD_JMP  = 143,
    CMD_JMPC = 144,
    CMD_SET  = 145,
    CMD_GET  = 146,
    CMD_STACKFRAME = 147,
    CMD_IN   = 148,
    CMD_OUT  = 149
};

#endif
