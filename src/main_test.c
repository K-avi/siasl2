#include <stdio.h>
#include "cells.h"
#include "cmdline_interp.h"
#include "exec.h"
#include "ast.h"
#include "parser.tab.h"
#include "stack.h"

extern instruction * prog;

int main(){
   
   CELLMATRIX* environment= init_mat(DEFAULT_ROWSIZE);
   S_STACK * stack= init_stack(STACK_SIZE);

   interactive_interp(environment, stack);

   free_stack(stack); 
   
   free_mat(environment);

}