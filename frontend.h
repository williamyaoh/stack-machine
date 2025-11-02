#ifndef FRONTEND_H
#define FRONTEND_H
#include <stdint.h>

#define TOK_NOOP 256
#define TOK_PUSH 257
#define TOK_RVALUE 258
#define TOK_LVALUE 259
#define TOK_POP 260
#define TOK_SETEQ 261
#define TOK_COPY 262
#define TOK_SWAP 263
#define TOK_LABEL 268
#define TOK_GOTO 269
#define TOK_GOFALSE 270
#define TOK_GOTRUE 271
#define TOK_HALT 272
#define TOK_PRINT 273

#define TOK_NUM 274
#define TOK_ID 275

extern int32_t tokennum;
extern char* tokenid;

typedef struct entry {
    char* lexptr;
    int32_t token;
} entry_t;
#endif
