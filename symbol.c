#include <string.h>
#include "frontend.h"
#include "error.h"

#define SYMMAX 1024
#define STRMAX 8196

entry_t symtable[SYMMAX] = {0};
char lexemes[STRMAX] = {0};
int32_t lastchar = -1;
int32_t lastentry = 0;

entry_t keywords[] = {
    { "noop", TOK_NOOP },
    { "push", TOK_PUSH },
    { "rvalue", TOK_RVALUE },
    { "lvalue", TOK_LVALUE },
    { "pop", TOK_POP },
    { "copy", TOK_COPY },
    { "swap", TOK_SWAP },
    { "label", TOK_LABEL },
    { "goto", TOK_GOTO },
    { "gofalse", TOK_GOFALSE },
    { "gotrue", TOK_GOTRUE },
    { "halt", TOK_HALT },
    { "print", TOK_PRINT },
    { NULL, 0 }
};

entry_t* lookup(char* s) {
    for (int p = lastentry; p > 0; p = p - 1) {
        if (strcmp(symtable[p].lexptr, s) == 0)
            return &symtable[p];
    }
    return NULL;
}

entry_t* insert(char* s, int32_t tok) {
    int len = strlen(s);
    if (lastentry + 1 >= SYMMAX)
        error("symbol table full");
    if (lastchar + len + 1 >= STRMAX)
        error("lexemes array full");
    lastentry = lastentry + 1;
    symtable[lastentry].token = tok;
    symtable[lastentry].lexptr = &lexemes[lastchar + 1];
    lastchar = lastchar + len + 1;
    strcpy(symtable[lastentry].lexptr, s);
    return &symtable[lastentry];
}

void init() {
    entry_t* p;
    for (p = keywords; p->lexptr; p++) {
        insert(p->lexptr, p->token);
    }
}
