#include <stdio.h>
#include <stdlib.h>

#include "fatal.h"

void fatal(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}
