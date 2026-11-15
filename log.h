#pragma once
#include <stdlib.h>
#include <string.h>
#include "ansi.h"

// string slice by pointers to original string
struct strsl {
    char *from;
    char *to;
};

// get null terminated string from slice
char *fromslice(struct strsl sl);

// cursor position for error handling
struct source_pos {
    struct strsl at;
    int offset;
    int line;
    char *file;
};

#define CURSOR_FORMAT "%s\n%*s\nat %s:%i:%i\n"
#define CURSOR_ARGS(name) fromslice(name.at), name.offset+1, "^", name.file, name.line+1, name.offset+1

// prints an error printf-style with position
#define print_perr(format, pos, ...) do {fprintf(stderr, ANSI_ERROR "spam error: " ANSI_RESET format CURSOR_FORMAT, ##__VA_ARGS__, CURSOR_ARGS(pos)); exit(1);} while(0)
// prints an error printf-style
#define print_err(format, ...) do {fprintf(stderr, ANSI_ERROR "spam error: " ANSI_RESET format, ##__VA_ARGS__); exit(1);} while(0)
 
#ifdef LOG_INFO
// prints into printf-style with position
#define print_pinfo(format, pos, ...) printf(ANSI_INFO "spam info: " ANSI_RESET format CURSOR_FORMAT, ##__VA_ARGS__, CURSOR_ARGS(pos))
// prints into printf-style
#define print_info(format, ...) printf(ANSI_INFO "spam info: " ANSI_RESET format, ##__VA_ARGS__)
#else
#ifdef LOG_VERBOSEINFO
// prints into printf-style with position
#define print_pinfo(format, pos, ...) printf(ANSI_INFO "spam info: " ANSI_RESET format CURSOR_FORMAT, ##__VA_ARGS__, CURSOR_ARGS(pos))
// prints into printf-style
#define print_info(format, ...) printf(ANSI_INFO "spam info: " ANSI_RESET format, ##__VA_ARGS__)
#else
// prints info printf-style with position (if LOG_INFO or LOG_VERBOSEINFO is defined)
#define print_pinfo(format, pos, ...)
// prints info printf-style (if LOG_INFO or LOG_VERBOSEINFO is defined)
#define print_info(format, ...)
#endif
#endif

#ifdef LOG_VERBOSEINFO
// prints verbose info printf-style with position
#define print_vpinfo(format, pos, ...) printf(ANSI_VINFO "spam verbose info: " format CURSOR_FORMAT ANSI_RESET, ##__VA_ARGS__, CURSOR_ARGS(pos))
// prints verbose info printf-style
#define print_vinfo(format, ...) printf(ANSI_VINFO "spam verbose info: " format ANSI_RESET, ##__VA_ARGS__)
#else
// prints verbose info printf-style with position (if LOG_VERBOSEINFO is defined)
#define print_vpinfo(format, pos, ...)
// prints verbose info printf-style (if LOG_VERBOSEINFO is defined)
#define print_vinfo(format, ...)
#endif