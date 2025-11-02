#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <stdint.h>

#define NOOP 0
#define PUSH 1
#define RVALUE 2
#define SWAP 3
#define POP 4
#define SETEQ 5
#define COPY 6
#define PLUS 7
#define MINUS 8
#define DIV 9
#define MUL 10
#define GOTO 12
#define GOFALSE 13
#define GOTRUE 14
#define HALT 15
#define PRINT 16

#define NOARG 0

typedef struct instruction {
    uint8_t opcode;
    int32_t arg;  // Whether this is an address or immediate value is determined by the instruction.
} instruction_t;

typedef struct instructions {
    instruction_t* instrs;
    uint32_t num_instrs;
} instructions_t;
#endif
