/*!
 * \file
 * Recursive-descent parser.
 */
#ifndef ZCC_RDP_H
#define ZCC_RDP_H
#include "tree.h"
#include "lexer.h"
#include "hashmap.h"

/*!
 * Parse an expression
 */
TreeNode *rdp(Token **tokens, FuncMap *funcmap);
/*!
 * Get RDP error.
 */
int rdp_get_err(void);
Token *rdp_get_err_token(void);

#endif /* ifndef ZCC_RDP_H */
