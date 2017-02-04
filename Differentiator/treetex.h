/*!
 * \file
 * TeX dumper.
 */
#ifndef TREETEX_H
#define TREETEX_H 
#include "treemath.h"

/*!
 * Dump to TeX.
 */
int DTreeNodeDumpTeX(DTreeNode *node, FILE *file);
/*!
 * Print node in equation mode.
 */
void PrintTeXEquation(FILE *file, DTreeNode *node, const char* prefix);
/*!
 * Print beginning.
 */
void BeginTeX(FILE *file);
/*!
 * Print ending.
 */
void EndTeX(FILE *file);
#endif /* ifndef SYMBOL */
