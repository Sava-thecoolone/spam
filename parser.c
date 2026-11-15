#include "token.h"
#include "libs/hashmap.h"
#include "parser.h"
#include "macro.h"
#include "ast.h"


void add_macro(struct parser_st *st, struct token tok) {
    print_vpinfo("add_macro called\n", tok.pos);
    (void)st;
    (void)tok;
    // if (toks->capacity <= toks->count) {
    //     toks->capacity = toks->capacity == 0 ? 256 : toks->capacity*2;
    //     toks->toks = realloc(toks->toks, toks->capacity*sizeof(struct token));
    // }
    // toks->toks[toks->count++] = tok;
}

// parser helper, returns current token
struct token peek(struct parser_st *st) {
    return st->toks->toks[st->pos];
}

// parser helper, eats current token
struct token eat(struct parser_st *st, enum TokenType type) {
    struct token t = peek(st);
    if (type != TOK_EOF && t.type != type) print_perr("invalid token: expected %s, got %s\n", peek(st).pos, typestrs[type], typestrs[t.type]);
    st->pos++;
    return t;
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
        case TOK_MACRO: {
            eat(st, TOK_MACRO);
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
        while (inprec(peek(st).str, level)) {
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
// struct astnode parse(struct parser_st *st) {
//     // ... block parsing ...
// }

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
        case NODE_SET: {
            printf("(set %s ", ((struct ast_set_data*)node.data)->name);
            print_ast(((struct ast_set_data*)node.data)->expr);
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
        case NODE_SET: {
            free_ast(((struct ast_set_data*)node.data)->expr);
            break;
        }
        default: break;
    }
    free(node.data);
}