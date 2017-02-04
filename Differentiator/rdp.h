/*!
 * Recursive-descent parser.
 */
#include "tree.h"

/*!
 * RDP error codes.
 */
enum 
{
    RDP_ERR_OK             = 0,
    RDP_ERR_EMPTY_NUMBER   = 1,
    RDP_ERR_UNEXPECTED_END = 2,
    RDP_ERR_EXPECTED_VARIABLE = 3,
    RDP_ERR_EXPECTED_BRACKETS = 4
};

/*!
 * Parse the string.
 */
DTreeNode *RDP(const char *str);
/*!
 * Get RDP error.
 */
int RDPGetError(void);
/*!
 * Get char that broke RDP.
 */
size_t RDPGetErrorPos(void);
