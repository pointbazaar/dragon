#ifndef INVOKE_H
#define INVOKE_H

#include <stdbool.h>
#include "util/flags.h"

bool invoke_lexer_parser(char* filename, struct Flags* flags);

#endif
