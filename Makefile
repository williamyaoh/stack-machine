.PHONY: clean all

clean:
	rm -f *.o asm

all: asm

asm: error.c lexer.c symbol.c assembler.c interpreter.c main.c
	gcc -O2 -Wall -Werror error.c lexer.c symbol.c assembler.c interpreter.c main.c -o asm
