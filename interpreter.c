#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "instruction.h"
#include "error.h"

#define STACK_LIMIT 1024
#define MEM_LIMIT 1024

int32_t stack[STACK_LIMIT];
int32_t memory[MEM_LIMIT];

int32_t stack_pop(int32_t* sc);
void stack_push(int32_t* sc, int32_t value);

void interpret(instructions_t instrs, bool debug) {
    uint32_t pc = 0;
    int32_t sc = -1;

    while (true) {
        if (pc >= instrs.num_instrs) return;

        instruction_t next = instrs.instrs[pc];
        int32_t operand;
        int32_t operand2;
        bool advance = true;

        if (debug) {
            // TODO logic to do the debug stuff here; print out which instruction
            // we're about to run, current state of the stack, (maybe) all of memory
            printf("%d: opcode: %d arg: %d\n", pc, next.opcode, next.arg);
        }

        switch(next.opcode) {
        case NOOP:
            break;
        case PUSH:
            stack_push(&sc, next.arg);
            break;
        case RVALUE:
            if (next.arg < 0 || next.arg >= MEM_LIMIT) error("out of bounds memory access");
            stack_push(&sc, memory[next.arg]);
            break;
        case POP:
            stack_pop(&sc);
            break;
        case SETEQ:
            operand2 = stack_pop(&sc);
            operand = stack_pop(&sc);
            if (operand < 0 || operand >= MEM_LIMIT) error("out of bounds memory access");
            memory[operand] = operand2;
            break;
        case COPY:
            operand = stack_pop(&sc);
            stack_push(&sc, operand);
            stack_push(&sc, operand);
            break;
        case SWAP:
            operand2 = stack_pop(&sc);
            operand = stack_pop(&sc);
            stack_push(&sc, operand2);
            stack_push(&sc, operand);
            break;
        case PLUS:
            operand2 = stack_pop(&sc);
            operand = stack_pop(&sc);
            stack_push(&sc, operand + operand2);
            break;
        case MINUS:
            operand2 = stack_pop(&sc);
            operand = stack_pop(&sc);
            stack_push(&sc, operand - operand2);
            break;
        case DIV:
            operand2 = stack_pop(&sc);
            operand = stack_pop(&sc);
            stack_push(&sc, operand / operand2);
            break;
        case MUL:
            operand2 = stack_pop(&sc);
            operand = stack_pop(&sc);
            stack_push(&sc, operand * operand2);
            break;
        case GOTO:
            advance = false;
            pc = (uint32_t) next.arg;
            break;
        case GOFALSE:
            operand = stack_pop(&sc);
            if (operand == 0) {
                advance = false;
                pc = (uint32_t) next.arg;
            }
            break;
        case GOTRUE:
            operand = stack_pop(&sc);
            if (operand != 0) {
                advance = false;
                pc = (uint32_t) next.arg;
            }
            break;
        case HALT:
            return;
        case PRINT:
            operand = stack_pop(&sc);
            printf("%d\n", operand);
            break;
        default:
            error("unknown opcode");
        }

        if (advance) pc += 1;
    }
}

int32_t stack_pop(int32_t* sc) {
    if (*sc < 0) error("stack is empty");
    int32_t value = stack[*sc];
    *sc = *sc - 1;
    return value;
}

void stack_push(int32_t* sc, int32_t value) {
    *sc = *sc + 1;
    if (*sc >= STACK_LIMIT) error("stack overflow");
    stack[*sc] = value;
}
