#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include "mash.h"

char random_char() {
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t index = rand() % strlen(charset);

    return charset[index];
}

void rand_string(size_t len, char *str) {
    for (size_t i = 0; i < len; i++) {
       str[i] = random_char();
    }

    str[len] = '\0';
}

#define ITEMS_COUNT 1000000
#define KEY_LEN 32

void print_value(void *value) {
    printf("\"%s\"", *(char **) value);
}

int main() {
    srand(time(NULL));

    Mash *hash = mash_create(sizeof(char *));
    char *strings[] = {
        "Eins",
        "Zwei",
        "Drei",
        "Vier",
        "Funf",
        "Sechs",
        "Sieben",
        "Acht",
        "Neun",
        "Zehn",
    };
    size_t len = sizeof(strings) / sizeof(strings[0]);

    for (size_t i = 0; i < 1000000; i += 1) {
        char *key = strings[rand() % len];
        mash_set(hash, key, &strings[rand() % len]);
        mash_delete(hash, key);
    }
    

    mash_debug(hash, print_value);

    return 0;
}
