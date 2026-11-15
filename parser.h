#pragma once
#include "error.h"
#include "token.h"
#include "macro.h"
#include "ast.h"

void add_macro(struct parser_st *st, struct token tok);

// parser helper, returns current token
struct token peek(struct parser_st *st);

// parser helper, eats current token
struct token eat(struct parser_st *st, enum TokenType type);

struct astnode parse_atom(struct parser_st *st);

// parse binop expression with precedence
struct astnode parse_prec(struct parser_st *st, int level);

// parse binop expression
struct astnode parse_expr(struct parser_st *st);

// parse block
struct astnode parse(struct parser_st *st);

void print_ast(struct astnode node);

void free_ast(struct astnode node);