#pragma once
#include "log.h"
#include "libs/hashmap.h"

// parser node type
enum NodeType {
    NODE_OP,
    NODE_LITERAL,
    NODE_SET,
    NODE_FUNC,
    NODE_MACRO,
};

// parser precedence level count
#define PREC_LEVELS 4
// parser precedence per-level max op count
#define PREC_CAP 4

extern char *precedence[PREC_CAP][PREC_LEVELS];

// returns bool if str is in precedence[level]
bool inprec(char *str, int level);

// parser ast node
struct astnode {
    enum NodeType type;
    struct source_pos pos;
    void *data;
};

struct ast_binop_data {
    char *op;
    struct astnode left;
    struct astnode right;
};

struct ast_set_data {
    char *name;
    struct astnode expr;
};

// parser state
struct parser_st {
    struct tokens *toks;
    int pos;
    struct hashmap *macros;
};