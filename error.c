#include <stdio.h>
#include <stdlib.h>

void error(char* msg) {
    fprintf(stderr, "error: %s\n", msg);
    exit(1);
}
