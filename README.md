32-bit stack machine with addressable memory.

```bash
make asm
cat examples/basic-loop.asm | ./asm
```

The instruction set is as follows:

Stack operations:

* **push** NUM - push the value onto the top of the stack
* **lvalue** IDENT - push the address of the given variable onto the stack
* **rvalue** IDENT - push the value of the given variable onto the stack
* **pop** - pop the top value of the stack
* **:=** - pop the top 2 values, store the top value into the bottom address
* **copy** - push a copy of the top stack value onto the stack
* **swap** - swap the positions of the top 2 values on the stack

Arithmetic operations, they all pop the top 2 values of the stack; top value is right
operand, bottom value is left operand. Result is placed back on the stack:

* **+**
* **-**
* **/**
* **\***

Control flow operations:

* **label** IDENT - target for jumps
* **goto** IDENT - jump to the given label
* **gofalse** IDENT - pop the top value of the stack, jump if non-zero
* **gotrue** IDENT - pop the top value of the stack, jump if zero
* **halt** - halt execution

Miscellaneous operations:

* **noop** - do nothing
* **print** - pop the top value and print it
