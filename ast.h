#pragma once
#include "log.h"
#include "libs/hashmap.h"

// parser node type
enum NodeType {
    NODE_BLOCK,
    NODE_OP,
    NODE_LITERAL,
    NODE_ASSIGN,
    NODE_FUNC, // todo: use this
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

// ast node dynamic array
struct astnode_da {
    struct astnode *items;
    int count;
    int capacity;
};

struct ast_block_data {
    struct astnode_da nodes;
};

struct ast_binop_data {
    char *op;
    struct astnode left;
    struct astnode right;
};

struct ast_assign_data {
    char *name;
    struct astnode expr;
};

// parser state
struct parser_st {
    struct token_da *toks;
    int pos;
    struct hashmap *macros;
};