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
    
    unsigned short instruction = curr->symbol.token1 | ((curr->symbol.token2)<<4);
  //printf("%u %u %b\n",curr->symbol.token1 , curr->symbol.token2 ,instruction );
    switch (instruction) {
        case (INT_NEUT<<4) | INT_NEUT:  break;

        case (INT_LEFT<<4)| INT_NEUT: idx=OP_LEFT(idx, matsize);  break;

        case (INT_RIGHT<<4)| INT_NEUT: idx=OP_RIGHT(idx , matsize); break;

        case (INT_UP<<4)| INT_NEUT: idx=OP_UP(idx, matsize); break;

        case (INT_DOWN<<4) | INT_NEUT : idx= OP_DOWN(idx, matsize); break;

        case (INT_PLUS<<4) | INT_NEUT: environment->mat[idx]++; break;

        case(INT_MINUS <<4) | INT_NEUT: environment->mat[idx]--; break;

        case (INT_MULT <<4) | INT_NEUT :environment->mat[idx]*=2; break;

        case (INT_DIV <<4) | INT_NEUT: environment->mat[idx]/=2; break;

        case (INT_READ<<4 )| INT_NEUT : environment->mat[idx]=getchar(); break;

        case (INT_PRINT<<4) | INT_NEUT: printf("%c", environment->mat[idx]); break;

        case (INT_LBRACKET <<4 ) | INT_NEUT :
            if (cells[idx] == 0) {
              curr = curr->other;
            } else {
              if (stack_ptr >= STACK_SIZE) {
                  return -1; //stack overflow
              }
              stack->stack[stack_ptr++] = curr;
            }
            break;
        case (INT_RBRACKET <<4) | INT_NEUT :
            curr = stack->stack[--stack_ptr];
            continue;
            
        default: 
         break;
    }
    curr = curr->next;
  }
  environment->curindex=idx;
  return 0;
}
