#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "symbol.h"
#include "error.h"

#define IDMAX 99

int32_t tokennum = 0;
char* tokenid = NULL;

int32_t lexan() {
    char c;
    while(1) {
        c = getchar();
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            ;
        else if (c == EOF)
            return EOF;
        else if (c == ':') {
            c = getchar();
            if (c == '=') return TOK_SETEQ;
            else error("bad assignment token");
        } else if (isdigit(c)) {
            tokennum = c - '0';
            c = getchar();
            while(isdigit(c)) {
                tokennum = (tokennum * 10) + (c - '0');
                c = getchar();
            }
            ungetc(c, stdin);
            return TOK_NUM;
        } else if (c == '~') {
            tokennum = 0;
            c = getchar();
            while(isdigit(c)) {
                tokennum = (tokennum * 10) + (c - '0');
                c = getchar();
            }
            ungetc(c, stdin);
            tokennum = -tokennum;
            return TOK_NUM;
        } else if (isalpha(c)) {
            char strbuf[IDMAX] = {0};
            strbuf[0] = tolower(c);
            int32_t nextchar = 1;
            c = getchar();
            while(isalnum(c)) {
                if (nextchar >= IDMAX-1)
                    error("max identifier length exceeded");
                strbuf[nextchar] = tolower(c);
                nextchar = nextchar + 1;
                c = getchar();
            }
            ungetc(c, stdin);
            entry_t* entry = lookup(strbuf);
            if (entry == NULL) {
                entry = insert(strbuf, TOK_ID);
                tokenid = entry->lexptr;
                return TOK_ID;
            } else {
                tokenid = entry->lexptr;
                return entry->token;
            }
        } else {
            return (uint32_t) c;
        }
    }
    return EOF;
}
