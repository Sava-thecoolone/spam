#include "libs/hashmap.h"
#include "macro.h"
#include <string.h>
#include "parser.h"

// helper for hashmap
int macro_compare(const void *a, const void *b, void *udata) {
    (void)udata;
    const struct macro *ua = a;
    const struct macro *ub = b;
    return strcmp(ua->str, ub->str);
}

// bool macro_iter(const void *item, void *udata) {
//     const struct macro *macro = item;
//     printf("%s (age=%d)\n", macro->name, macro->age);
//     return true;
// }

// helper for hashmap
uint64_t macro_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct macro *macro = item;
    return hashmap_sip(macro->str, strlen(macro->str), seed0, seed1);
}