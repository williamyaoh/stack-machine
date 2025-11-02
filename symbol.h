#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdint.h>
#include "frontend.h"

entry_t* lookup(char* s);

entry_t* insert(char* s, int32_t tok);

void init();
#endif
