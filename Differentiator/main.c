#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "rdp.h"
#include "diff.h"
#include "treemath.h"
#include "treetex.h"
#include "article.h"

/*!
 * Max input buffer size.
 */
enum { MAX_INPUT_BUF_SIZE = 1024 };
/*!
 * Error codes.
 */
enum 
{ 
    ERR_WRONG_SYNTAX = -1, 
    ERR_CALC_ERROR   = -2,
    ERR_IO           = -3
};

int main(void)
{
    srand(time(NULL));

    char buffer[MAX_INPUT_BUF_SIZE] = "";
    fgets(buffer, MAX_INPUT_BUF_SIZE, stdin);
    char *term = strchr(buffer, '\n');
    if (term)
        *term = '\0';
    
    DTreeNode *result = RDP(buffer);
    if (RDPGetError() == RDP_ERR_OK)
    {
        TREENODE_DUMP(result, "dump/tree.gv", "dump/tree.png");
    }
    else
    {
        for (size_t i = 0; i < RDPGetErrorPos(); i++)
            fputc(' ', stderr);
        fputs("^\nParse error!\n", stderr);
        DTreeNodeDtor(result);
        return ERR_WRONG_SYNTAX;
    }

    FILE *article = fopen("dump/article.tex", "w");
    if (!article)
    {
        DTreeNodeDtor(result);
        fprintf(stderr, "Cannot access file!\n");
        return ERR_IO;
    }
    BeginTeX(article);
    PrintIntro(article);
    PrintTeXEquation(article, result, "f(x) = ");

    if (Simplify(result, result, article, "f(x) = ") != DIFF_OK)
    {
        fputs("Calculation error!\n", stderr);
        DTreeNodeDtor(result);
        return ERR_CALC_ERROR;
    }
    //PrintTeXEquation(article, result, "f(x) = ");
    TREENODE_DUMP(result, "dump/simplified.gv", "dump/simplified.png");

    DTreeNode *diff = Diff(result);
    if (diff)
    {
        TREENODE_DUMP(diff, "dump/diff.gv", "dump/diff.png");
        PrintTeXEquation(article, diff, "f'(x) = ");
        if (Simplify(diff, diff, article, "f'(x) = ") != DIFF_OK)
        {
            fputs("Calculation error!\n", stderr);
            return ERR_CALC_ERROR;
        }
        TREENODE_DUMP(diff, "dump/diff_simplified.gv", "dump/diff_simplified.png");
        // PrintTeXEquation(article, diff, "f'(x) = ");
        DTreeNodeDtor(diff);
    }
    else
    {
        fprintf(stderr, "Differentiation error!\n");
        DTreeNodeDtor(result);
        fclose(article);
        return ERR_CALC_ERROR;
    }
    
    DTreeNodeDtor(result);

    PrintOutro(article);
    EndTeX(article);
    fclose(article);
    system("pdflatex -output-directory=./dump dump/article.tex -o dump/article.pdf");
    return 0;
}
