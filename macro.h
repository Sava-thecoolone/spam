#pragma once
#include "libs/hashmap.h"
#include "ast.h"

struct macro_argument {
    char *name;
    bool lit;
};

// marco argument dynamic array
struct macro_argument_da {
    struct macro_argument *items;
    int count;
    int capacity;
};

// parser macro
struct macro {
    struct macro_argument_da args;
    bool lit;
    struct astnode body;
};

// helper for hashmap
struct macro_entry {
    char *str;
    struct macro data;
};

// helper for hashmap
int macro_compare(const void *a, const void *b, void *udata);

// bool macro_iter(const void *item, void *udata);

// helper for hashmap
uint64_t macro_hash(const void *item, uint64_t seed0, uint64_t seed1);