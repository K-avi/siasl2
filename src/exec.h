#ifndef EXEC_H 
#define EXEC_H

#include "ast.h"
#include "cells.h"
#include "stack.h"
extern int exec_prgm( program* progr, CELLMATRIX* environment, S_STACK* stack, macrotable * table, unsigned char *printcheck);

extern int mod(int a , int b);

#define OP_LEFT(index, matsize) ( mod(( (index) -1), (matsize)))
#define OP_RIGHT(index, matsize) ( mod(( (index) + 1) , (matsize)))
#define OP_UP(index, matsize) ( mod(( (index) - DEFAULT_ROWSIZE ) , (matsize)))
#define OP_DOWN(index, matsize) ( mod(( (index) + DEFAULT_ROWSIZE ) , (matsize)))

#endif