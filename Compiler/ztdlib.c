#include "ztdlib.h"
#include "common.h"
#include <stdio.h>
#include <assert.h>

const char_t *ZTDLIB_CODE = "call main:\n"
"jmp exit:\n"
"\n"
"input:\n"
"stackframe 0\n"
"in\n"
"ret\n"
"\n"
"print:\n"
"stackframe 1\n"
"get 0\n"
"out\n"
"push 0\n"
"ret\n"
"\n";

int inject_ztdlib_code(void)
{
    fputs(ZTDLIB_CODE, stdout);
    return ZCC_OK;
}

int inject_program_end(void)
{
    fprintf(stdout, "exit:\n");
    return ZCC_OK;
}

int install_ztdlib_funcmap(FuncMap *funcmap)
{
    assert(funcmap);
    funcmap_insert(funcmap, "input", 0, 0);
    funcmap_insert(funcmap, "print", 1, 0);

    return ZCC_OK;
}
