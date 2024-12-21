#ifndef LINK_H
#define LINK_H
#include <stdio.h>
#include <inttypes.h>

typedef struct LinkNode {
    struct LinkNode *next;
    uint8_t value[];
} LinkNode;

typedef struct Link {
    LinkNode *head;
    LinkNode *tail;
    size_t item_size;
} Link;

Link *link_create(size_t item_size);

void link_append_node(Link *link, void *value);
void link_prepend_node(Link *link, void *value);

void link_pop_node(Link *link, size_t index, void *value);
void link_delete_node(Link *link, size_t index);
void link_destroy(Link *link);

typedef struct LinkIterator {
    LinkNode *current;
} LinkIterator;

LinkIterator link_iterator_create(Link *link);
void *link_iterator_next(LinkIterator *link_iterator);

#endif
