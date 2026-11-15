#pragma once
#include "libs/hashmap.h"
#include "ast.h"

struct macro_data {
    char **argnames;
    struct astnode body;
};

// parser macros
struct macro {
    char *str;
    struct macro_data data;
};

// helper for hashmap
int macro_compare(const void *a, const void *b, void *udata);

// bool macro_iter(const void *item, void *udata);

// helper for hashmap
uint64_t macro_hash(const void *item, uint64_t seed0, uint64_t seed1);