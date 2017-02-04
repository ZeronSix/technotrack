/*!
 * \file
 * Compiler.
 */
#include <stdio.h>
#include "common.h"
#include "lexer.h"
#include "io.h"
#include "simplifier.h"
#include "translator.h"
#include "rdp.h"
#include "ztdlib.h"

#define MALLOC_ERROR() \
{ \
    fprintf(stderr, "Memory allocation error!\n"); \
    return ZCC_ERR_ALLOC; \
}

/*!
 * TODO: usage
 */
int main(int argc, char **argv)
{
    int result = ZCC_OK;

    if (argc != 2) 
    {
        fprintf(stderr, "Wrong arguments! Usage: zcc {source}");
        return ZCC_ERR_WRONG_ARGS;
    }
   
    // read input 
    Text source = {};
    if ((result = text_new(&source, argv[1])) != ZCC_OK) 
    {
        fprintf(stderr, "IO error %d!", result);
        text_del(&source);
        return result;
    }

    // lexical analysis
    Token **tokens = analyze(&source);
    text_del(&source);

    // parsing
    FuncMap *funcmap = funcmap_new();
    if (!funcmap)
        MALLOC_ERROR();
    install_ztdlib_funcmap(funcmap);
    TreeNode *root = rdp(tokens, funcmap);
    TREENODE_DUMP(root, "./dump/tree.gv", "./dump/tree.png");
    if (!root)
    {
        result = rdp_get_err();
        fprintf(stderr, "RDP Error: %d at [%lu:%lu]\n", result, rdp_get_err_token()->line, 
                                                                rdp_get_err_token()->column);
        treenode_del(root);
        del_tokens(tokens);
        return result;
    }

    // simplification
    result = simplify(root);
    if (result != ZCC_OK)
    {
        fprintf(stderr, "Simplification error: %d\n", result);

        treenode_del(root);
        del_tokens(tokens);
        return result;
    }
    TREENODE_DUMP(root, "./dump/tree_simplified.gv", "./dump/tree_simplified.png");
    
    inject_ztdlib_code();
    Translator tr = {};
    tr.funcmap = funcmap;
    if ((result = translate(&tr, root)) != ZCC_OK)
    {
        fprintf(stderr, "Translation error!\n");
        treenode_del(root);
        del_tokens(tokens);
        return result;
    } 
    inject_program_end();

    funcmap_del(funcmap);
    treenode_del(root);
    del_tokens(tokens);
    
    return ZCC_OK;
}
