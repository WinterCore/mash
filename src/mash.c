#include "./mash.h"
#include "aids.h"
#include "link.h"
#include <stdio.h>
#include <string.h>

#define INITIAL_SIZE 16
#define LOAD_GROW_THRESHOLD 0.7
#define LOAD_SHRINK_THRESHOLD 0.3
#define GROW_MULTIPLIER 2
#define SHRINK_MULTIPLIER 0.5


unsigned long djb2_hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = (unsigned char) *str++)) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int find_bucket_elem_index(Link *bucket, char *key) {
    int i = 0;

    LinkIterator iterator = link_iterator_create(bucket);
    MashElement *elem;
    
    while ((elem = link_iterator_next(&iterator)) != NULL) {
        if (strcmp(elem->key, key) == 0) {
            return i;
        }

        i += 1;
    }

    return -1;
}


Mash *mash_create(size_t elem_size) {
    Mash *map = malloc(sizeof(Mash));

    map->elem_size = elem_size;
    map->num_elements = 0;
    map->buckets_count = INITIAL_SIZE;
    map->buckets = calloc(INITIAL_SIZE, sizeof(Link *));

    return map;
}

MashElement create_element(char *key, void *value, size_t value_size) {
    MashElement elem = {0};

    size_t key_len = strlen(key);

    elem.key = malloc(key_len + 1);
    elem.value = malloc(value_size);

    if (elem.key == NULL || elem.value == NULL) {
        PANIC("Failed to allocate memory")
    }

    memcpy(elem.key, key, key_len);
    elem.key[key_len] = '\0';
    memcpy(elem.value, value, value_size);
    // printf("Created value %p = %p\n\n", value, *((void **) elem->value));

    return elem;
}

void destroy_element(MashElement *elem) {
    free(elem->key);
    free(elem->value);
}

typedef enum InsertResult {
    INSERT_SUCCESS,
    INSERT_OVERWRITE,
} InsertResult;

InsertResult insert_element(size_t buckets_len, Link *buckets[buckets_len], MashElement *elem) {
    size_t index = djb2_hash(elem->key) % buckets_len;
    
    Link *bucket = buckets[index];

    if (bucket == NULL) {
        bucket = link_create(sizeof(MashElement));
        buckets[index] = bucket;

        link_append_node(bucket, elem);

        return INSERT_SUCCESS;
    }

    int existing_elem_idx = find_bucket_elem_index(bucket, elem->key);

    if (existing_elem_idx > -1) {
        // Key already exists
        LinkNode *node = link_get_node(bucket, existing_elem_idx);
        destroy_element((void *) node->value);
        link_set_node_value(bucket, node, elem);

        return INSERT_OVERWRITE;
    }

    link_append_node(bucket, elem);

    return INSERT_SUCCESS;
}

void grow_if_needed(Mash *mash) {
    float load = (float) mash->num_elements / (float) mash->buckets_count;

    if (load < LOAD_GROW_THRESHOLD) {
        return;
    }

    size_t new_buckets_count = mash->buckets_count * GROW_MULTIPLIER;
    Link **new_buckets = calloc(new_buckets_count, sizeof(Link *));
    // printf("Resizing from %zu to %zu cuz of element_count = %zu\n", mash->buckets_count, new_buckets_count, mash->num_elements);

    for (size_t i = 0; i < mash->buckets_count; i += 1) {
        Link *bucket = mash->buckets[i];

        if (bucket == NULL) {
            continue;
        }

        LinkIterator iterator = link_iterator_create(bucket);

        MashElement *elem;
        
        while ((elem = link_iterator_next(&iterator)) != NULL) {
            insert_element(
                new_buckets_count,
                new_buckets,
                elem
            );
        }
        
        link_destroy(bucket);
    }

    free(mash->buckets);
    mash->buckets = new_buckets;
    mash->buckets_count = new_buckets_count;
}

void mash_set(Mash *mash, char *key, void *value) {
    grow_if_needed(mash);

    MashElement elem = create_element(key, value, mash->elem_size);
    
    switch (insert_element(mash->buckets_count, mash->buckets, &elem)) {
    case INSERT_SUCCESS:
        mash->num_elements += 1;
        break;
    case INSERT_OVERWRITE:
        break;
    }
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

void mash_delete(Mash *mash, char *key) {
    // TODO: Shrink if needed

    size_t index = djb2_hash(key) % mash->buckets_count;
    
    Link *bucket = mash->buckets[index];

    if (bucket == NULL) {
        return;
    }

    int idx = find_bucket_elem_index(bucket, key);

    if (idx == -1) {
        return;
    }

    LinkNode *node = link_get_node(bucket, idx);
    destroy_element((void *) node->value);
    link_delete_node(bucket, idx);
    mash->num_elements -= 1;

    // TODO: Maybe this isn't necessary?
    if (link_is_empty(bucket)) {
        link_destroy(bucket);
        mash->buckets[index] = NULL;
    }
}

void mash_debug(Mash *mash, void (*print_value)(void *)) {
    printf("Mash {\n");
    printf("\tbuckets_count = %zu\n", mash->buckets_count);
    printf("\telements_count = %zu\n", mash->num_elements);
    printf("}\n");

    printf("BUCKETS--------\n");
    for (size_t i = 0; i < mash->buckets_count; i += 1) {
        Link *bucket = mash->buckets[i];
        
        if (bucket == NULL || link_is_empty(bucket)) {
            printf("[%zu]: Empty\n", i);
            continue;
        }

        LinkIterator iterator = link_iterator_create(bucket);
        MashElement *elem;

        printf("[%zu]: \n", i);

        // TODO: Handle empty linked lists
        while ((elem = link_iterator_next(&iterator)) != NULL) {
            printf("  -key=\"%s\"", elem->key);
            if (print_value != NULL) {
                printf("   -value=");
                print_value(elem->value);
            }

            printf("\n");
        }

        printf("\n");
    }
}
