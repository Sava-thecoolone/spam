#pragma once
#include "token.h"

// helper function to trim spaces and comments from char*
char *trimleft(char *str, struct source_pos *err);

// lex char*, returns token da
struct tokens lex(char *str, char *file, Regex *rules);