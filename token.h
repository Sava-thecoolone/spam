#pragma once
#include "log.h"
#include "libs/Regex.h"
#include <stdlib.h>
#define TYPE_NUM 12

// lexer token type
enum TokenType {
    TOK_OP,
    TOK_STR,
    TOK_INT,
    TOK_EQ,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_LSTRUCT,
    TOK_RSTRUCT,
    TOK_COMMA,
    TOK_PERIOD,
    TOK_MACRO,
    TOK_VAR,
    TOK_EOF
};

// literal lexer token type names
extern const char *typestrs[TYPE_NUM+1];

// regex rules for token lexing
extern const char *rulestrs[TYPE_NUM];

// lexer token
struct token {
    char *str;
    enum TokenType type;
    struct source_pos pos;
};

// lexer token da
struct tokens {
    struct token *toks;
    int count;
    int capacity;
};

Regex *comprules();
void free_tokens(struct tokens toks);
void da_append(struct tokens *toks, struct token tok);