#ifndef AIDS_H
#define AIDS_H

#include <stdlib.h>

#define PANIC(...) \
    fprintf(stderr, "-----PANIC-----> %s:%d:%s(): ", __FILE__, __LINE__, __func__); \
    fprintf(stderr, __VA_ARGS__); \
    exit(EXIT_FAILURE);

#endif
