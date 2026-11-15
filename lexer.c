#include "token.h"
#include <ctype.h>
#include "lexer.h"

// helper function to trim spaces and comments from char*
char *trimleft(char *str, struct source_pos *err) {
    while (1) {
        for (; isspace(str[0]); str++) {
            err->offset++;
            if (str[0] == '\n') {
                err->line++;
                err->at.from++;
                char *newline = strchr(str, '\n');
                if (newline != NULL) err->at.to = newline;
                else err->at.to = str+strlen(str);
                err->offset = 0;
            }
        }
        if (str[0] != '\0' && str[1] != '\0' && str[0] == '/' && str[1] == '/') {
            str += 2;
            for (; str[0] != '\n' && str[0] != '\0'; str++);
            err->line++;
            err->at.from++;
            char *newline = strchr(str, '\n');
            if (newline != NULL) err->at.to = newline;
            else err->at.to = str+strlen(str);
            err->offset = 0;
            continue;
        } else break;
    }
    return str;
}

// lex char*, returns token da
struct tokens lex(char *str, char *file, Regex *rules) {
    print_info("lexing...\n");
    struct tokens toks = {0};
    struct source_pos err = {.at.from=str, .file=file};
    char *newline = strchr(str, '\n');
    if (newline != NULL) err.at.to = newline;
    else err.at.to = str+strlen(str);

    char *end = str+strlen(str);
    while (str < end) {
        str = trimleft(str, &err);
        if (str >= end) break;
        bool found_match = false;
        for (int i = 0; i < TYPE_NUM; i++) {
            Matcher m = regexMatch(&rules[i], str);
            if (m.isFound) {
                char *new = malloc((m.matchLength+1)*sizeof(char));
                memcpy(new, str, m.matchLength);
                new[m.matchLength] = '\0';
                da_append(&toks, (struct token){new, i, err});
                err.offset += m.matchLength;
                str += m.matchLength;
                found_match = true;
                break;
            }
        }
        if (!found_match) print_perr("invalid token\n", err);
    }
    da_append(&toks, (struct token){strdup(""), TOK_EOF, err});
    return toks;
}