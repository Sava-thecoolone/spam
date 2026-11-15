// SPAM
// SPAM Produces Assembly for Mindustry
 
// ...is a programming language designed to target mindustry processors
// it compiles to mindustry's high-level assembly

// run `comp` or `./comp.sh` to compile

#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "token.h"

int main(int argc, char **argv) {
#ifdef _WIN32
    print_vinfo("changing code page...\n");
    if (system("chcp 65001 > nul") != 0) {
        fprintf(stderr, "counldn't enable ansi coloring");
        exit(1);
    }
#endif
    if (argc <= 1) print_err("incorrect argument count: %d\n", argc);
    Regex *rules = comprules();

    print_vinfo("opening %s...\n", argv[1]);
    FILE *pinput = fopen(argv[1], "r");
    if (pinput == NULL) {
        print_err("file not found: %s\n", argv[1]);
    }
    fseek(pinput, 0L, SEEK_END);
    long inplen = ftell(pinput);
    rewind(pinput);
    char *input = malloc(inplen+1);
    fread(input, sizeof(char), inplen, pinput);
    input[inplen] = '\0';
    fclose(pinput);
    struct tokens toks = lex(input, argv[1], rules);
    for (int i = 0; i < toks.count; i++) {
        printf("[%d %s] ", toks.toks[i].type, toks.toks[i].str);
    }
    printf("\n");
    print_info("parsing...\n");
    struct parser_st state = {&toks, 0, hashmap_new(sizeof(struct macro), 0, 0, 0, macro_hash, macro_compare, NULL, NULL)};
    struct astnode ast = parse_expr(&state);
    free_tokens(toks);
    print_ast(ast);
    printf("\n");
    free_ast(ast);
    free(input);
    free(rules);

    return 0;
}