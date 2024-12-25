#include <stdio.h>
#include "mash.h"


int main() {

    Mash *hash = mash_create(sizeof(int));
    mash_set(hash, "hello", (int []) {5});
    mash_set(hash, "world", (int []) {12});
    mash_set(hash, "world", (int []) {13});

    int *value = mash_get(hash, "world");

    printf("Value = %d", *value);

    return 0;
}
