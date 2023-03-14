#include "exec.h"
#include "stack.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



int exec_prgm( instruction* program, CELLMATRIX* environment, S_STACK* stack) {

  if(! (program && environment && stack)) return -1;
  instruction* curr = program;

  unsigned short stack_ptr = 0;
  
 // int length= environment->size;
  int idx= environment->curindex; 
 
  CELL *cells= environment->mat; 

 
  uint32_t matsize= environment->size* environment->size;
  while (curr) {
    int instruction=curr->token;
    
    switch (instruction) {
        case INT_NEUT:  break;

        case INT_LEFT: idx=OP_LEFT(idx, matsize);  break;

        case INT_RIGHT: idx=OP_RIGHT(idx , matsize); break;

        case INT_UP: idx=OP_UP(idx, matsize); break;

        case INT_DOWN : idx= OP_DOWN(idx, matsize); break;

        case INT_PLUS: environment->mat[idx]++; break;

        case INT_MINUS: environment->mat[idx]--; break;

        case INT_MULT: environment->mat[idx]*=2; break;

        case INT_DIV: environment->mat[idx]/=2; break;

        case INT_READ: environment->mat[idx]=getchar(); break;

        case INT_PRINT: printf("%c", environment->mat[idx]); break;

        case INT_LBRACKET:
            if (cells[idx] == 0) {
              curr = curr->other;
            } else {
              if (stack_ptr >= STACK_SIZE) {
                  return -1; //stack overflow
              }
              stack->stack[stack_ptr++] = curr;
            }
            break;
        case INT_RBRACKET:
            curr = stack->stack[--stack_ptr];
            continue;
            
            default: return 0;
    }
    curr = curr->next;
  }
  environment->curindex=idx;
  return 0;
}
