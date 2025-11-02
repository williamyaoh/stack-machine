#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "instruction.h"
#include "error.h"
#include "symbol.h"

#define LOOKUP_MAX 1024
#define INSTR_MAX 8192

typedef struct addr_lookup {
    char* identifier;
    int32_t addr;  // Negative if this is a dummy entry that will be filled in later
} addr_lookup_t;

addr_lookup_t label_lookup[LOOKUP_MAX];
addr_lookup_t ident_lookup[LOOKUP_MAX];
uint32_t num_labels = 0;
uint32_t num_idents = 0;

instruction_t instrs[INSTR_MAX];
uint32_t next_instr = 0;

int32_t lookahead;

/*
  Here's the grammar that we're working with here. Note that the lexer is case-insensitive.

  program -> instruction program | epsilon

  instruction -> 'noop' { output NOOP }
               | 'push' num { output PUSH with arg }
               | 'rvalue' id { fetch or register mem addr with id, output RVALUE with associated addr }
               | 'lvalue' id { fetch or register mem addr with id, output PUSH with associated addr }
               | 'pop' { output POP }
               | ':=' { output SETEQ }
               | 'copy' { output COPY }
               | 'swap' { output SWAP }
               | '+' { output PLUS }
               | '-' { output MINUS }
               | '/' { output DIV }
               | '*' { output MUL }
               | 'label' id { fetch or register instr index with id }
               | 'goto' id { fetch or create entry in the label lookup table for id, output GOTO with the index of the table entry, negated }
               | 'gofalse' id { fetch or create entry in the label lookup table for id, output GOFALSE with the index of the table entry, negated }
               | 'gotrue' id { fetch or create entry in the label lookup table for id, output GOTRUE with the index of the table entry, negated }
               | 'halt' { output HALT }
               | 'print' { output PRINT }

  The point of the negated indexes in the control flow statements is that sequentially, the
  label might appear after the control statements that point to it. For instance:

  > label L0
  > copy
  > gofalse EXIT
  > push 1
  > -
  > goto L0
  > label EXIT

  At the point where we see `gofalse', we don't yet have enough information to know what address
  to output as its argument. So instead we place a dummy entry in the label lookup table, to be
  filled in later, and place the index of the entry in that lookup as the opcode argument. After
  we've finished our first pass, we do a second pass and replace all control flow arguments with
  the actual label addresses.
 */

addr_lookup_t* register_lookup(addr_lookup_t* mapping, uint32_t* mappinglen, addr_lookup_t lookup);
void output(uint8_t opcode, int32_t arg);

void rewrite_gotos();

void program();
void instruction();
void match();

instructions_t assemble() {
    instructions_t result;
    init();
    lookahead = lexan();
    program(); match(EOF);
    rewrite_gotos();
    result.instrs = instrs;
    result.num_instrs = next_instr;
    return result;
}

addr_lookup_t* register_lookup(addr_lookup_t* mapping, uint32_t* mappinglen, addr_lookup_t lookup) {
    uint32_t i;
    for (i = 0; i < *mappinglen; i++)
        if (strcmp(mapping[i].identifier, lookup.identifier) == 0) return &mapping[i];
    if (i >= LOOKUP_MAX) error("max lookup length exceeded");
    mapping[i] = lookup;
    *mappinglen = *mappinglen + 1;
    return &mapping[i];
}

void output(uint8_t opcode, int32_t arg) {
    instruction_t instr;
    instr.opcode = opcode;
    instr.arg = arg;
    instrs[next_instr] = instr;
    next_instr = next_instr + 1;
}

void rewrite_gotos() {
    for (uint32_t i = 0; i < next_instr; i++) {
        instruction_t* instr = &instrs[i];
        switch(instr->opcode) {
        case GOTO:
        case GOFALSE:
        case GOTRUE:
            if (instr->arg < 0) {
                addr_lookup_t lookup = label_lookup[-(instr->arg)];
                if (lookup.addr < 0) error("goto pointing to unknown label");
                instr->arg = lookup.addr;
            }
            break;
        default:
        }
    }
}

void program() {
    switch(lookahead) {
    case TOK_NOOP:
    case TOK_PUSH:
    case TOK_RVALUE:
    case TOK_LVALUE:
    case TOK_POP:
    case TOK_SETEQ:
    case TOK_COPY:
    case TOK_SWAP:
    case '+':
    case '-':
    case '/':
    case '*':
    case TOK_LABEL:
    case TOK_GOTO:
    case TOK_GOFALSE:
    case TOK_GOTRUE:
    case TOK_HALT:
    case TOK_PRINT:
        instruction(); program();
    default:
    }
}

void instruction() {
    addr_lookup_t* lookup;
    char* ident;
    switch(lookahead) {
    case TOK_NOOP:
        match(TOK_NOOP);
        output(NOOP, NOARG);
        break;
    case TOK_PUSH:
        match(TOK_PUSH); match(TOK_NUM);
        output(PUSH, tokennum);
        break;
    case TOK_RVALUE:
        match(TOK_RVALUE); ident = tokenid; match(TOK_ID);
        lookup = register_lookup(ident_lookup, &num_idents, (addr_lookup_t) { ident, num_idents });
        output(RVALUE, lookup->addr);
        break;
    case TOK_LVALUE:
        match(TOK_LVALUE); ident = tokenid; match(TOK_ID);
        lookup = register_lookup(ident_lookup, &num_idents, (addr_lookup_t) { ident, num_idents });
        output(PUSH, lookup->addr);
        break;
    case TOK_POP:
        match(TOK_POP);
        output(POP, NOARG);
        break;
    case TOK_SETEQ:
        match(TOK_SETEQ);
        output(SETEQ, NOARG);
        break;
    case TOK_COPY:
        match(TOK_COPY);
        output(COPY, NOARG);
        break;
    case TOK_SWAP:
        match(TOK_SWAP);
        output(SWAP, NOARG);
        break;
    case '+':
        match('+');
        output(PLUS, NOARG);
        break;
    case '-':
        match('-');
        output(MINUS, NOARG);
        break;
    case '/':
        match('/');
        output(DIV, NOARG);
        break;
    case '*':
        match('*');
        output(MUL, NOARG);
        break;
    case TOK_LABEL:
        match(TOK_LABEL); ident = tokenid; match(TOK_ID);
        lookup = register_lookup(label_lookup, &num_labels, (addr_lookup_t) { ident, next_instr });
        if (lookup->addr < 0) lookup->addr = next_instr;
        break;
    case TOK_GOTO:
        match(TOK_GOTO); ident = tokenid; match(TOK_ID);
        lookup = register_lookup(label_lookup, &num_labels, (addr_lookup_t) { ident, -num_labels });
        output(GOTO, lookup->addr);
        break;
    case TOK_GOFALSE:
        match(TOK_GOFALSE); ident = tokenid; match(TOK_ID);
        lookup = register_lookup(label_lookup, &num_labels, (addr_lookup_t) { ident, -num_labels });
        output(GOFALSE, lookup->addr);
        break;
    case TOK_GOTRUE:
        match(TOK_GOTRUE); ident = tokenid; match(TOK_ID);
        lookup = register_lookup(label_lookup, &num_labels, (addr_lookup_t) { ident, -num_labels });
        output(GOTRUE, lookup->addr);
        break;
    case TOK_HALT:
        match(TOK_HALT);
        output(HALT, NOARG);
        break;
    case TOK_PRINT:
        match(TOK_PRINT);
        output(PRINT, NOARG);
        break;
    default:
        error("unknown operator");
    }
}

void match(int32_t t) {
    if (lookahead == t) {
        lookahead = lexan();
    } else error("syntax error");
}
