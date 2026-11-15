#pragma once
#include "log.h"
#include "libs/Regex.h"
#include <stdlib.h>
#define TYPE_NUM 15

// lexer token type
enum TokenType {
    TOK_OP,
    TOK_STR,
    TOK_INT,
    TOK_EQ,
    TOK_SC,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_LSTRUCT,
    TOK_RSTRUCT,
    TOK_LTEXT,
    TOK_RTEXT,
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

// lexer token dynamic array
struct token_da {
    struct token *items;
    int count;
    int capacity;
};

#define da_append(da, item) do {\
    if (da.capacity <= da.count) {\
        da.capacity = da.capacity == 0 ? 256 : da.capacity*2;\
        da.items = realloc(da.items, da.capacity*sizeof(item));\
    }\
    da.items[da.count++] = item;\
} while(0)

Regex *comprules();
void free_tokens(struct token_da da);