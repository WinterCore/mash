#include "./mash.h"
#include <string.h>

#define INITIAL_SIZE 11
#define LOAD_THRESHOLD 0.7


unsigned long djb2_hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = (unsigned char) *str++)) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

Mash *mash_create(size_t elem_size) {
    Mash *map = malloc(
        sizeof(Mash) +
        (INITIAL_SIZE * sizeof(Link *))
    );

    map->elem_size = elem_size;
    map->num_elements = 0;
    map->buckets_count = INITIAL_SIZE;
    
    memset(map->buckets, 0, INITIAL_SIZE);

    return map;
}


Mash *mash_set(Mash *mash, char *key, void *value) {
    // TODO: Resize if needed



}
