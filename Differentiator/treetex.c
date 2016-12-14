#include "treetex.h"
#include "article.h"
#include <assert.h>

#define TEX_DUMP(_node_, _file_, _brackets_) \
{ \
    fprintf(_file_, "{"); \
    if (_brackets_) \
        fprintf(_file_, "\\left("); \
    DTreeNodeDumpTeX(_node_, _file_); \
    if (_brackets_) \
        fprintf(_file_, "\\right)"); \
    fprintf(_file_, "}"); \
}

int DTreeNodeDumpTeX(DTreeNode *this_, FILE *file)
{
    assert(this_);
    assert(file);
    DTreeNode *left = this_->left;
    DTreeNode *right = this_->right;
    bool placeBrackets = false;

    switch (this_->type)
    {
        case NODE_SUM:
            assert(left);
            assert(right);

            TEX_DUMP(left, file, false);
            fprintf(file, " + ");
            TEX_DUMP(right, file, false);
            break;
        case NODE_DIFF:
            assert(left);
            assert(right);
            TEX_DUMP(left, file, false);
            fprintf(file, " - ");
            TEX_DUMP(right, file, false);
            break;
        case NODE_MUL:
            assert(left);
            assert(right);
            placeBrackets = left->type == NODE_SUM || left->type == NODE_DIFF;
            TEX_DUMP(left, file, placeBrackets);
            fprintf(file, " * ");
            placeBrackets = right->type == NODE_SUM || right->type == NODE_DIFF;
            TEX_DUMP(right, file, placeBrackets);
            break;
        case NODE_POW:
            assert(left);
            assert(right);
            placeBrackets = left->type == NODE_SUM || left->type == NODE_DIFF ||
                            right->type == NODE_MUL || right->type == NODE_DIV;
            TEX_DUMP(left, file, placeBrackets);
            fprintf(file, "^");
            TEX_DUMP(right, file, false);
            break;
        case NODE_DIV:
            assert(left);
            assert(right);
            fprintf(file, "\\frac");
            placeBrackets = left->type == NODE_SUM || left->type == NODE_DIFF;
            TEX_DUMP(left, file, placeBrackets);
            TEX_DUMP(right, file, false);
            break;
        case NODE_VAR:
            fprintf(file, "x");
            break;
        case NODE_CONST:
            if (this_->parent && this_->data < 0)
                fprintf(file, "\\left(");
            fprintf(file, "%lg", this_->data);
            if (this_->parent && this_->data < 0)
                fprintf(file, "\\right)");
            break;
        case NODE_LN:
            assert(right);
            fprintf(file, "\\ln\\left(");
            TEX_DUMP(right, file, false);
            fprintf(file, "\\right)");
            break;
        case NODE_COS:
            assert(right);
            fprintf(file, "\\cos\\left(");
            TEX_DUMP(right, file, false);
            fprintf(file, "\\right)");
            break;
        case NODE_SIN:
            assert(right);
            fprintf(file, "\\sin\\left(");
            TEX_DUMP(right, file, false);
            fprintf(file, "\\right)");
            break;
        default:
            break;
    }

    return DIFF_OK;
}

/*!
 * Print beginning.
 */
void BeginTeX(FILE *file)
{
    assert(file);
    fprintf(file, "\\documentclass{article}\n");
    fprintf(file, "\\usepackage{amsmath}\n");
    fprintf(file, "\\usepackage{amssymb}\n");
    fprintf(file, "\\usepackage[utf8]{inputenc}\n");
    fprintf(file, "\\usepackage[russian]{babel}\n");
    //fprintf(article, "\\usepackage[ngerman]{babel}");
    fprintf(file, "\\begin{document} \n\n");
}

void EndTeX(FILE *file)
{
    assert(file);
    fprintf(file, "\\end{document}\n");
}

void PrintTeXEquation(FILE *file, DTreeNode *node, const char* prefix)
{
    assert(file);
    assert(node);
    assert(prefix);

    PrintTransition(file);
    fprintf(file, "\\begin{equation*}\n");
    fprintf(file, "%s", prefix);
    DTreeNodeDumpTeX(node, file);
    fprintf(file, "\n\\end{equation*}\n");
}
