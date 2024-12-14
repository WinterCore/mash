#include <stdio.h>
#include "./link.h"


int main() {
    Link *link = link_create(sizeof(int));

    link_append_node(link, &(int[]){5});
    link_append_node(link, &(int[]){4});
    link_append_node(link, &(int[]){3});
    link_append_node(link, &(int[]){2});

    int output;
    link_pop_node(link, 0, &output);
    link_pop_node(link, 0, &output);
    link_pop_node(link, 0, &output);
    link_pop_node(link, 0, &output);
    link_pop_node(link, 0, &output);
    link_pop_node(link, 0, &output);
    
    printf("%p", link->tail);

    return 0;
}
