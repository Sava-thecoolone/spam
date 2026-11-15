#include "token.h"
#include "libs/hashmap.h"
#include "parser.h"
#include "macro.h"
#include "ast.h"

// parser helper, returns current token
struct token peek(struct parser_st *st) {
    return st->toks->items[st->pos];
}

// parser helper, eats current token
struct token eat(struct parser_st *st, enum TokenType type) {
    struct token t = peek(st);
    if (t.type != type) print_perr("invalid token: expected %s, got %s\n", t.pos, typestrs[type], typestrs[t.type]);
    st->pos++;
    return t;
}

void add_macro(struct parser_st *st, struct macro_entry *mac) {
    print_vpinfo("add_macro called\n", peek(st).pos);
    hashmap_set(st->macros, mac);
}


void parse_macro(struct parser_st *st) {
    print_vpinfo("parsing macro...\n", peek(st).pos);
    eat(st, TOK_MACRO);
    struct token t = eat(st, TOK_VAR);
    eat(st, TOK_EQ);
    bool lit;
    if (peek(st).type == TOK_LBRACE) {
        eat(st, TOK_LBRACE);
        lit = false;
    } else {
        eat(st, TOK_LTEXT);
        lit = true;
    }
    struct macro_argument_da args = {0};
    while (peek(st).type != TOK_RBRACE) {
        if (peek(st).type == TOK_STR) {
            char *str = strdup(eat(st, TOK_STR).str);
            struct macro_argument arg = {str, true};
            da_append(args, arg);
        } else {
            char *str = strdup(eat(st, TOK_VAR).str);
            struct macro_argument arg = {str, false};
            da_append(args, arg);
        }
        if (peek(st).type == TOK_COMMA) eat(st, TOK_COMMA);
    }
    eat(st, TOK_RBRACE);
    struct astnode body = parse(st);
    struct macro mac = {args, lit, body};
    struct macro_entry *entry = malloc(sizeof(struct macro_entry));
    *entry = (struct macro_entry){strdup(t.str), mac};
    add_macro(st, entry);
}

struct astnode parse_atom(struct parser_st *st) {
    print_vpinfo("parsing atom...\n", peek(st).pos);
    switch (peek(st).type) {
        case TOK_STR: {
            struct token t = eat(st, TOK_STR);
            return (struct astnode){NODE_LITERAL, t.pos, strdup(t.str)};
        }
        case TOK_INT: {
            struct token t = eat(st, TOK_INT);
            return (struct astnode){NODE_LITERAL, t.pos, strdup(t.str)};
        }
        case TOK_VAR: {
            struct token t = eat(st, TOK_VAR);
            return (struct astnode){NODE_LITERAL, t.pos, strdup(t.str)};
        }
        default: print_perr("invalid atom %s\n", peek(st).pos, typestrs[peek(st).type]);
    }
}

// parse binop expression with precedence
struct astnode parse_prec(struct parser_st *st, int level) {
    if (level >= PREC_LEVELS) return parse_atom(st);
    else {
        struct astnode left = parse_prec(st, level+1);
        print_vpinfo("parsing op...\n", peek(st).pos);
        while (peek(st).type == TOK_OP && inprec(peek(st).str, level)) {
            struct token op = eat(st, TOK_OP);
            struct astnode right;
            if (peek(st).type == TOK_EOF) print_perr("trailing operator in expression\n", peek(st).pos);
            if (strcmp(peek(st).str, "^")) right = parse_prec(st, level);
            else right = parse_prec(st, level+1);
            struct ast_binop_data *data = malloc(sizeof(struct ast_binop_data));
            *data = (struct ast_binop_data){strdup(op.str), left, right};
            left = (struct astnode){NODE_OP, op.pos, data};
        }
        return left;
    }
}

// parse binop expression
struct astnode parse_expr(struct parser_st *st) {
    print_vpinfo("parsing expression...\n", peek(st).pos);
    return parse_prec(st, 0);
}

// parse block
struct astnode parse(struct parser_st *st) {
    struct astnode_da stmts;
    while (peek(st).type != TOK_SC && peek(st).type != TOK_EOF) {
        if (peek(st).type == TOK_MACRO) parse_macro(st);
        else {
            struct astnode node = parse_expr(st);
            da_append(stmts, node);
        }
    }
    if (peek(st).type == TOK_SC) eat(st, TOK_SC);
    if (stmts.count == 1) return stmts.items[0];
    else {
        struct ast_block_data *data = malloc(sizeof(struct ast_block_data));
        *data = (struct ast_block_data){stmts};
        return (struct astnode){NODE_BLOCK, peek(st).pos, data};
    }
}

void print_ast(struct astnode node) {
    print_vinfo("printing ast...\n");
    switch (node.type) {
        case NODE_OP: {
            printf("(op ");
            print_ast(((struct ast_binop_data*)node.data)->left);
            printf(" %s ", ((struct ast_binop_data*)node.data)->op);
            print_ast(((struct ast_binop_data*)node.data)->right);
            printf(")");
            break;
        }
        case NODE_LITERAL: printf("(lit %s)", (char*)node.data); break;
        case NODE_ASSIGN: {
            printf("(set %s ", ((struct ast_assign_data*)node.data)->name);
            print_ast(((struct ast_assign_data*)node.data)->expr);
            printf(")");
            break;
        }
        default: break;
    }
}

void free_ast(struct astnode node) {
    print_vinfo("freeing ast...\n");
    switch (node.type) {
        case NODE_OP: {
            free_ast(((struct ast_binop_data*)node.data)->left);
            free_ast(((struct ast_binop_data*)node.data)->right);
            free(((struct ast_binop_data*)node.data)->op);
            break;
        }
        case NODE_ASSIGN: {
            free_ast(((struct ast_assign_data*)node.data)->expr);
            break;
        }
        default: break;
    }
    free(node.data);
}