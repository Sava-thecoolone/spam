#include "token.h"

const char *typestrs[TYPE_NUM+1] = {
    "op",
    "str",
    "int",
    "'='",
    "'('",
    "')'",
    "'{'",
    "'}'",
    "','",
    "'.'",
    "'#'",
    "id",
    "eof"
};

const char *rulestrs[TYPE_NUM] = {
    "^[+\\-*/<>\\^]",
    "^\".*?\"",
    "^\\d+",
    "^=",
    "^\\(",
    "^\\)",
    "^\\{",
    "^\\}",
    "^,",
    "^\\.",
    "^#",
    "^[a-zA-Z0-9_]+"
};

Regex *comprules() {
    Regex *r = malloc(TYPE_NUM*sizeof(Regex));
    for (int i = 0; i < TYPE_NUM; i++) {
        print_vinfo("compiling lex rule %d: %s\n", i, rulestrs[i]);
        regexCompile(&r[i], rulestrs[i]);
    }
    print_vinfo("done compiling\n");
    return r;
}

void free_tokens(struct tokens toks) {
    print_vinfo("freeing tokens...\n");
    for (int i = 0; i < toks.count; i++) free(toks.toks[i].str);
    free(toks.toks);
}

void da_append(struct tokens *toks, struct token tok) {
    if (toks->capacity <= toks->count) {
        toks->capacity = toks->capacity == 0 ? 256 : toks->capacity*2;
        toks->toks = realloc(toks->toks, toks->capacity*sizeof(struct token));
    }
    toks->toks[toks->count++] = tok;
}