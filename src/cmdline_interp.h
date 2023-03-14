#ifndef CMD_INT_H
#define CMD_INT_H

#include "ast.h"
#include "cells.h"
#include "exec.h"
#include "lex.yy.h"
#include "parser.tab.h"
#include "stack.h"

extern void interactive_interp( CELLMATRIX * environment, S_STACK * stack, unsigned char mode);

#endif