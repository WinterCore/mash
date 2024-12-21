#ifndef MASH_H
#define MASH_H
#include <stdlib.h>
#include "./link.h"

typedef struct Mash {
    size_t elem_size;

    size_t buckets_count;
    size_t num_elements;

    Link *buckets[];
} Mash;

typedef struct MashElement {
    char *key;
    void *value;
} MashElement;

Mash *mash_create(size_t elem_size);

void mash_set(Mash *mash, char *key, void *value);
void *mash_get(Mash *mash, char *key);

#endif
