#include <stdlib.h>
#include <string.h>

#include "./link.h"
#include "aids.h"

Link *link_create(size_t item_size) {
    Link *link = malloc(sizeof(Link));

    if (link == NULL) {
        PANIC("Out of memory");
    }

    link->head = NULL;
    link->tail = NULL;
    link->item_size = item_size;

    return link;
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

LinkNode *link_get_node(Link *link, size_t index) {
    LinkNode *head = link->head;

    while (head != NULL && index > 0) {
        index -= 1;
        head = head->next;
    }
    
    return head;
}

void link_pop_node(Link *link, size_t index, void *value) {
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

bool link_is_empty(Link *link) {
    if (link->head == NULL || link->tail == NULL) {
        return true;
    }
    
    return false;
}

void link_destroy(Link *link) {
    LinkNode *head = link->head;

    while (head) {
        LinkNode *next = head->next;
        free(head);

        head = next;
    }

    free(link);
}

void link_set_node_value(Link *link, LinkNode *node, void *value) {
    memcpy(node->value, value, link->item_size);
}

LinkIterator link_iterator_create(Link *link) {
    return (LinkIterator) { .current = link->head };
}

void *link_iterator_next(LinkIterator *link_iterator) {
    LinkNode *node = link_iterator->current;

    if (node == NULL) {
        return NULL;
    }

    link_iterator->current = link_iterator->current->next;

    return node->value;
}
