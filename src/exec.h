#ifndef EXEC_H 
#define EXEC_H

#include "ast.h"
#include "cells.h"
#include "stack.h"
extern int exec_prgm( program* progr, CELLMATRIX* environment, S_STACK* stack, macrotable * table, unsigned char *printcheck);

#endif