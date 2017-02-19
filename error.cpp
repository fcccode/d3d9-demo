#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void fatal(const char *file, int line, const char *message) {
    fprintf(stderr, "%s:%d %s\n", file, line, message);
    abort();
}
