#include <stdlib.h>
#include <string.h>
#include "log.h"

// get null terminated string from slice
char *fromslice(struct strsl sl) {
    char *str = malloc(sl.to-sl.from+1);
    memcpy(str, sl.from, sl.to-sl.from);
    str[sl.to-sl.from] = '\0';
    return str;
}