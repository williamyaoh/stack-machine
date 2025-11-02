#include "assembler.h"
#include "interpreter.h"
#include <stdlib.h>

int main() {
    instructions_t program = assemble();
    interpret(program, false);
    return 0;
}
