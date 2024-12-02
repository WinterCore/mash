#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "./link.h"

Link link_create(size_t item_size) {
    return (Link) {
        .item_size = item_size,
        .head = NULL,
        .tail = NULL,
    };
}

static LinkNode *link_node_create(Link *link, void *value) {
    LinkNode *node = malloc(sizeof(LinkNode) + link->item_size);
    if (node == NULL) {
        perror("Out of memory");
        exit(EXIT_FAILURE);
    }

    memcpy(node->value, value, link->item_size);
    node->next = NULL;

    return node;
}


void link_append_node(Link *link, void *value) {
    LinkNode *new_node = link_node_create(link, value);

    if (link->tail == NULL) {
        link->head = new_node;
        link->tail = new_node;

        return;
    }
    
    LinkNode *tail = link->tail;
    tail->next = new_node;
    link->tail = new_node;
}

void link_prepend_node(Link *link, void *value) {
    LinkNode *new_node = link_node_create(link, value);

    if (link->head == NULL) {
        link->head = new_node;
        link->tail = new_node;
    }

    new_node->next = link->head;
    link->head = new_node;
}

void link_pop_node(Link *link, size_t index, void *value) {
    assert(index >= 0 && "link_delete_node: index should be >= 0");

    if (link->head == NULL) {
        return;
    }

    LinkNode *prev = NULL;
    LinkNode *node = link->head;
    
    // Find
    while (index > 0 && node != NULL) {
        prev = node;
        node = node->next;

        index -= 1;
    }

    // Copy
    if (value != NULL) {
        memcpy(value, node->value, link->item_size);
    }

    // Delete
    if (prev != NULL) {
        prev->next = node->next;
    }

    if (node == link->head) {
        link->head = node->next;
    }

    if (node == link->tail) {
        link->tail = prev;
    }

    free(node);
}

void link_delete_node(Link *link, size_t index) {
    link_pop_node(link, index, NULL);
}


void link_destroy(Link *link) {
    // Iterate over items and free memory
}
