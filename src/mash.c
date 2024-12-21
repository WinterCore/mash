#include "./mash.h"
#include "aids.h"
#include "link.h"
#include <stdio.h>
#include <string.h>

#define INITIAL_SIZE 1
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

MashElement *create_element(char *key, void *value, size_t value_size) {
    MashElement *elem = malloc(sizeof(MashElement));

    if (elem == NULL) {
        PANIC("Failed to allocate memory");
    }

    size_t key_len = strlen(key);

    elem->key = malloc(key_len);
    elem->value = malloc(sizeof(value_size));

    if (elem->key == NULL || elem->value == NULL) {
        PANIC("Failed to allocate memory")
    }

    memcpy(elem->key, key, key_len);
    memcpy(elem->value, value, value_size);

    return elem;
}

void mash_set(Mash *mash, char *key, void *value) {
    // TODO: Resize if needed

    MashElement *elem = create_element(key, value, mash->elem_size);
    
    size_t index = djb2_hash(key) % mash->buckets_count;

    Link *link = mash->buckets[index];

    if (link == NULL) {
        link = link_create(sizeof(MashElement));
        mash->buckets[index] = link;
    }

    // TODO: Check if key already exists
    link_append_node(link, elem);
}

void *mash_get(Mash *mash, char *key) {
    size_t index = djb2_hash(key) % mash->buckets_count;

    Link *bucket = mash->buckets[index];

    if (bucket == NULL) {
        return NULL;
    }
    
    LinkIterator iterator = link_iterator_create(bucket);

    MashElement *elem;
    
    while ((elem = link_iterator_next(&iterator)) != NULL) {
        if (strcmp(elem->key, key) == 0) {
            break;
        }
    }

    if (elem != NULL) {
        return elem->value;
    }

    return NULL;
}
