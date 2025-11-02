#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <stdint.h>

/*
  Instruction set:

  push VAL
  rvalue ADDR
  pop
  :=   // use top as r-value, value beneath as ADDR l-value
  copy
  swap
  // Within the textual assembly language, rather than addresses, we allow identifiers
  // as memory locations, rather than actual addresses. This also necessitates an `lvalue'
  // instruction within the assembly language. The assembler will assign addresses to
  // each identifier, and translate `rvalue' instructions accordingly. `lvalue' commands
  // get translated into equivalent `push' instructions with the address value.

  // For arithmetic instructions, we assume that the top is the right side, value beneath as left side
  +
  -
  /
  *

  // Note that `label' does not have a representation within the machine code; the assembler
  // will translate any references to a LABEL (via `goto', `gofalse' etc.) into the appropriate
  // address value
  label LABEL
  goto ADDR
  gofalse ADDR
  gotrue ADDR
  halt

  noop
  print  // pops the top value
 */

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
