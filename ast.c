#include "ast.h"

char *precedence[PREC_CAP][PREC_LEVELS] = {
    {"==", "<=", ">=", NULL},
    {"+", "-", NULL},
    {"*", "/", NULL},
    {"^", NULL}
};

// returns bool if str is in precedence[level]
bool inprec(char *str, int level) {
    for (int i = 0; i < PREC_CAP && precedence[level][i] != NULL; i++) {
        if (strcmp(precedence[level][i], str) == 0) return true;
    }
    return false;
}