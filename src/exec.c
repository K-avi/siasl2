#include "exec.h"
#include "ast.h"
#include "cells.h"
#include "macros.h"
#include "stack.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

int default_mult_div=2;

int checkDefaultSymbol(Symbol sym){
  /*
  returns 1 if sym is a default sym 0 otherwise
  */
  if( (sym & INT_NEUT)==INT_NEUT ){
    switch (sym) {

          case (INT_LEFT<<4)| INT_NEUT: return 1; break;
          case (INT_RIGHT<<4)| INT_NEUT: return 1; break;
          case (INT_UP<<4)| INT_NEUT: return 1; break;
          case (INT_DOWN<<4) | INT_NEUT : return 1; break;
          case (INT_PLUS<<4) | INT_NEUT: return 1; break;
          case(INT_MINUS <<4) | INT_NEUT: return 1; break;
          case (INT_MULT <<4) | INT_NEUT : return 1; break;
          case (INT_DIV <<4) | INT_NEUT: return 1; break;
          case (INT_READ<<4 )| INT_NEUT : return 1; break;
          case (INT_PRINT<<4) | INT_NEUT: return 1; break;
          case (INT_LBRACKET <<4 ) | INT_NEUT : return 1; break;
          case (INT_RBRACKET <<4) | INT_NEUT : return 1; break;
          case (INT_RPAR<<4) | INT_NEUT: return 1; break;

          default: return 0;  break;
    }
  }else if((sym & INT_PLUS)==INT_PLUS){
            
      switch (sym) {

      
      case (INT_PLUS <<4) | INT_PLUS : return 1; break;
      case (INT_PRINT<<4) | INT_PLUS: return 1; break;
      case (INT_LBRACKET <<4 ) | INT_PLUS : return 1; break;
      case (INT_RBRACKET <<4) | INT_PLUS : return 1; break;
      default: return 0;  break;
      }
  }else if ((sym & INT_PLUS<<4)== (INT_PLUS<<4)){

      switch (sym){

        case INT_RIGHT | (INT_PLUS<<4) : return 1; break;
        case INT_LEFT | (INT_PLUS<<4) : return 1; break;
        case INT_UP | (INT_PLUS<<4) : return 1; break;
        case INT_DOWN | (INT_PLUS<<4) : return 1; break;
      
        default: return 0;  break;
      }
  }else if( ((sym & (INT_MINUS <<4))) == ((INT_MINUS<<4))){
     
     switch (sym) {

        case INT_RIGHT | (INT_MINUS<<4) : return 1; break;
        case INT_LEFT | (INT_MINUS<<4) : return 1; break;
        case INT_UP | (INT_MINUS<<4) : return 1; break;
        case INT_DOWN | (INT_MINUS<<4) : return 1; break;
        case (INT_MINUS <<4) | INT_MINUS: return 1; break;
        default: return 0;  break;
      }
  }else if (  (sym & (INT_MULT<<4))== (INT_MULT<<4) ){

      switch (sym) {
     
        case INT_RIGHT | (INT_MULT<<4) :return 1; break;
        case INT_LEFT | (INT_MULT<<4) :return 1; break;
        case INT_UP | (INT_MULT<<4) : return 1; break;
        case INT_DOWN | (INT_MULT<<4) : return 1; break;
        case (INT_MULT <<4) | INT_MULT : return 1; break;
        default: return 0;  break;
      }
  }else if( ((sym & (INT_LBRACKET<<4))== (INT_LBRACKET<<4)) || ((sym & (INT_RBRACKET<<4))== (INT_RBRACKET<<4))){
    
      switch (sym) {

        case (INT_LBRACKET <<4 ) | INT_MINUS : return 1; break;
        case (INT_RBRACKET <<4) | INT_MINUS :return 1; break;
        case (INT_LBRACKET <<4 ) | INT_LEFT : return 1; break;
        case (INT_RBRACKET <<4) | INT_LEFT : return 1; break;
        
        case (INT_LBRACKET <<4 ) | INT_RIGHT: return 1; break;
        case (INT_RBRACKET <<4) | INT_RIGHT :return 1; break;
        case (INT_LBRACKET <<4 ) | INT_UP : return 1; break;
        case (INT_RBRACKET <<4) | INT_UP : return 1; break;
        
        case (INT_LBRACKET <<4 ) | INT_DOWN : return 1; break;
        case (INT_RBRACKET <<4) | INT_DOWN : return 1; break;

        default: return 0;  break;  
      }
  }else{

    switch (sym) {

      case INT_RIGHT | (INT_DIV<<4) : return 1; break;
      case INT_LEFT | (INT_DIV<<4) :return 1; break;
      case INT_UP | (INT_DIV<<4) : return 1; break;
      case INT_DOWN | (INT_DIV<<4) : return 1; break;
      case (INT_DIV <<4) | INT_DIV : return 1; break; 


      case (INT_DOWN<<4)| INT_DOWN: return 1; break; //goes to first cell 
      case (INT_UP<<4)| INT_UP : return 1; break;; //goes to last cell 
      case (INT_LEFT<<4)| INT_LEFT : return 1; break; //goes to first cell in line
      case (INT_RIGHT<<4)| INT_RIGHT : return 1; break;
     
      case (INT_PRINT<<4) | INT_MINUS: return 1; break;
      case (INT_PRINT<<4) | INT_MULT: return 1; break;
      case (INT_PRINT<<4) | INT_DIV: return 1; break;
      case INT_PRINT | (INT_WILDCARD <<4)  : return 1; break;

      case (INT_WILDCARD<<4) | INT_READ : return 1; break;
      case (INT_WILDCARD<<4) | INT_LEFT: return 1; break;
      case (INT_WILDCARD<<4) | INT_RIGHT:  return 1; break;
      case INT_LPAR | ( INT_NEUT <<4): return 1; break;
      
      default: return 0; break;
    }
  } 
   
  return 0;
}

#define OP_EXEC(progr, dir) if((dir)){ (progr)= (progr)->next ;} else{ (progr)= (progr)->prev ;}

int exec_prgm( program* progr, CELLMATRIX* environment, S_STACK* stack, macrotable * table, unsigned char * printcheck) {

  if(! (progr && environment && stack && table)) {
    //("%p %p %p %p\n", progr, environment, stack, table);
    return -1;
  }
  instruction* curr = progr->head;
  
  unsigned short stack_ptr = 0;
  
 // int length= environment->size;
  int idx= environment->curindex; 

  int exec_direction= 1 ; //used when changing sense of execution from "right to left" to "left to right"
//modified when calling the "?>" and "?<" symbols.
 
  
  char safe_getchar[256]; //used in read function to pass stuff safely
  
  uint32_t matsize= environment->size* environment->size;
  while (curr) {
    
    unsigned short instruction = curr->symbol;

  

    switch (instruction) {

      /* default operations; the one that already are in SIASL */

        case (INT_LEFT<<4)| INT_NEUT: idx=OP_LEFT(idx, matsize);  break;

        case (INT_RIGHT<<4)| INT_NEUT: idx=OP_RIGHT(idx , matsize); break;

        case (INT_UP<<4)| INT_NEUT: idx=OP_UP(idx, matsize); break;

        case (INT_DOWN<<4) | INT_NEUT : idx= OP_DOWN(idx, matsize); break;

        case (INT_PLUS<<4) | INT_NEUT: environment->mat[idx]++; break;

        case(INT_MINUS <<4) | INT_NEUT: environment->mat[idx]--; break;

        case (INT_MULT <<4) | INT_NEUT :environment->mat[idx]*=default_mult_div; break;

        case (INT_DIV <<4) | INT_NEUT: if(default_mult_div) {environment->mat[idx]/=default_mult_div;} break;

        case (INT_READ<<4 )| INT_NEUT : 
            fflush(stdin);
            if(fgets(safe_getchar, 255, stdin)){
              
            environment->mat[idx]= safe_getchar[0];
            fflush(stdin);

            memset(safe_getchar, 0, 256*sizeof(char));
            }
            break;

        case (INT_PRINT<<4) | INT_NEUT: printf("%c", environment->mat[idx]); if(printcheck)*printcheck=1; break;

        case (INT_LBRACKET <<4 ) | INT_NEUT :
            if(exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
              } else {
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
              }
              break;
            }else{
              curr = stack->stack[--stack_ptr];
              continue;
            }
        case (INT_RBRACKET <<4) | INT_NEUT :
            if(!exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
              } else {
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
              }
              break;
            }else{
              curr = stack->stack[--stack_ptr];
              continue;
            }
      
      /*new predefined operations */

      /* movement operations */

      case (INT_DOWN<<4)| INT_DOWN: idx=0;  break; //goes to first cell 
      case (INT_UP<<4)| INT_UP : idx=(environment->size*environment->size)-1;  break; //goes to last cell 

      case (INT_LEFT<<4)| INT_LEFT : idx= idx- (idx % environment->size); break; //goes to first cell in line
      case (INT_RIGHT<<4)| INT_RIGHT : idx= idx + (environment->size -(idx % environment->size));   break; //goes to last cell in line 

      /* 
      arithmetic primites ; stores in the current cell the result of :
      curvalue= curvalue (airthmetic op) (movement op)->value

      for exemple: 

      +< stores in the current cell the value of mat[idx]+ mat[OP_RIGHT(idx, matsize)]
      v* stores in the current cell the value of mat[idx] * mat[OP_DOWN(idx, matsize)]
      */
      case INT_RIGHT | (INT_PLUS<<4) : environment->mat[idx]+=environment->mat[OP_RIGHT(idx, matsize)]; break;
      case INT_LEFT | (INT_PLUS<<4) : environment->mat[idx]+=environment->mat[OP_LEFT(idx, matsize)]; break;
      case INT_UP | (INT_PLUS<<4) : environment->mat[idx]+=environment->mat[OP_UP(idx, matsize)]; break;
      case INT_DOWN | (INT_PLUS<<4) : environment->mat[idx]+=environment->mat[OP_DOWN(idx, matsize)]; break;


      case INT_RIGHT | (INT_MINUS<<4) : environment->mat[idx]-=environment->mat[OP_RIGHT(idx, matsize)]; break;
      case INT_LEFT | (INT_MINUS<<4) : environment->mat[idx]-=environment->mat[OP_LEFT(idx, matsize)]; break;
      case INT_UP | (INT_MINUS<<4) : environment->mat[idx]-=environment->mat[OP_UP(idx, matsize)]; break;
      case INT_DOWN | (INT_MINUS<<4) : environment->mat[idx]-=environment->mat[OP_DOWN(idx, matsize)]; break;

      case INT_RIGHT | (INT_MULT<<4) : environment->mat[idx]*=environment->mat[OP_RIGHT(idx, matsize)]; break;
      case INT_LEFT | (INT_MULT<<4) : environment->mat[idx]*=environment->mat[OP_LEFT(idx, matsize)]; break;
      case INT_UP | (INT_MULT<<4) : environment->mat[idx]*=environment->mat[OP_UP(idx, matsize)]; break;
      case INT_DOWN | (INT_MULT<<4) : environment->mat[idx]*=environment->mat[OP_DOWN(idx, matsize)]; break;

      case INT_RIGHT | (INT_DIV<<4) : if(environment->mat[OP_RIGHT(idx,matsize)] ) environment->mat[idx]/=environment->mat[OP_RIGHT(idx, matsize)]; break;
      case INT_LEFT | (INT_DIV<<4) : if(environment->mat[OP_LEFT(idx,matsize)] ) environment->mat[idx]/=environment->mat[OP_LEFT(idx, matsize)]; break;
      case INT_UP | (INT_DIV<<4) : if(environment->mat[OP_UP(idx,matsize)] ) environment->mat[idx]/=environment->mat[OP_UP(idx, matsize)]; break;
      case INT_DOWN | (INT_DIV<<4) : if(environment->mat[OP_DOWN(idx,matsize)] ) environment->mat[idx]/=environment->mat[OP_DOWN(idx, matsize)]; break;

      /* arithmetic stuff */

      case (INT_MULT <<4) | INT_MULT :environment->mat[idx]*=environment->mat[idx]; break; //square operation 
      case (INT_DIV <<4) | INT_DIV : environment->mat[idx]= 1; break; //square operation 
      case (INT_PLUS <<4) | INT_PLUS : environment->mat[idx]*=2; break; //square operation 
      case (INT_MINUS <<4) | INT_MINUS: environment->mat[idx]=0; break; //square operation 


      /* print formats */

      case (INT_PRINT<<4) | INT_PLUS: printf("%d", environment->mat[idx]); if(printcheck)*printcheck=1; break;
      case (INT_PRINT<<4) | INT_MINUS: printf("%x", environment->mat[idx]);if(printcheck)*printcheck=1; break;
      case (INT_PRINT<<4) | INT_MULT: printf("%o", environment->mat[idx]); if(printcheck)*printcheck=1;break;
      case (INT_PRINT<<4) | INT_DIV: printf("%u", (unsigned) environment->mat[idx]); if(printcheck)*printcheck=1;break;
      case INT_PRINT | (INT_WILDCARD <<4)  : printf("%.2f", (float) environment->mat[idx]);if(printcheck)*printcheck=1; break;

      /* loop variations */

      /*arithmetic modes */
      case (INT_LBRACKET <<4 ) | INT_PLUS : //increments curcell each time going on loop 
            if(exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
              } else {
                environment->mat[idx]++;
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
              }
              
              break;
            }else{
              curr = stack->stack[--stack_ptr];
              environment->mat[idx]++;
              continue;
            }
        case (INT_RBRACKET <<4) | INT_PLUS : //increments curcell each time ending loop 
            if(!exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
              } else {
                environment->mat[idx]++;
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
              }
              
              break;
            }else{
              curr = stack->stack[--stack_ptr];
              environment->mat[idx]++;
              continue;
            }    
        
        case (INT_LBRACKET <<4 ) | INT_MINUS : //decrement curcell each time going in loop 
            if(exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
              } else {
                environment->mat[idx]--;
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
              }
              
              break;
            }else{
              curr = stack->stack[--stack_ptr];
              environment->mat[idx]++;
              continue;
            }
        case (INT_RBRACKET <<4) | INT_MINUS : //decrements curcell each time finishing loop 
            
            if(!exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
              } else {
                environment->mat[idx]--;
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
              }
              
              break;
            }else{
              curr = stack->stack[--stack_ptr];
              environment->mat[idx]++;
              continue;
            }
        /*movement modes */
        case (INT_LBRACKET <<4 ) | INT_LEFT : //goes to the left of curcell each time going in loop 
            if(exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
                idx=OP_LEFT(idx, matsize);
              } else {
            
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
                idx=OP_LEFT(idx, matsize);
              }
              
              break;
            }else{
              curr = stack->stack[--stack_ptr];
          
              idx=OP_LEFT(idx, matsize);
              continue;
            }
        case (INT_RBRACKET <<4) | INT_LEFT : //goes to the left of curcell each time ending loop
            if(!exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
                idx=OP_LEFT(idx, matsize);
              } else {
            
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
                idx=OP_LEFT(idx, matsize);
              }
              
              break;
            }else{
              curr = stack->stack[--stack_ptr];
          
              idx=OP_LEFT(idx, matsize);
              continue;
            }
        
        case (INT_LBRACKET <<4 ) | INT_RIGHT: //goes to the right of curcell each time going on loop 
            if(exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
                idx=OP_RIGHT(idx, matsize);
              } else {
            
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
                idx=OP_RIGHT(idx, matsize);
              }
              
              break;
            }else{
              curr = stack->stack[--stack_ptr];
          
              idx=OP_RIGHT(idx, matsize);
              continue;
            }
        case (INT_RBRACKET <<4) | INT_RIGHT ://goes to the right of curcell each time ending loop
            if(!exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
                idx=OP_RIGHT(idx, matsize);
              } else {
            
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
                idx=OP_RIGHT(idx, matsize);
              }
              
              break;
            }else{
              curr = stack->stack[--stack_ptr];
          
              idx=OP_RIGHT(idx, matsize);
              continue;
            }
        
        case (INT_LBRACKET <<4 ) | INT_UP : //goes to up of curcell each time going on loop 
            if(exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
                idx=OP_UP(idx, matsize);
              } else {
            
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
                idx=OP_UP(idx, matsize);
              }
              
              break;
            }else{
              curr = stack->stack[--stack_ptr];
          
              idx=OP_UP(idx, matsize);
              continue;
            }
        case (INT_RBRACKET <<4) | INT_UP : //goes to up of curcell each time ending loop
            
            if(!exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
                idx=OP_UP(idx, matsize);
              } else {
            
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
                idx=OP_UP(idx, matsize);
              }
              
              break;
            }else{
              curr = stack->stack[--stack_ptr];
          
              idx=OP_UP(idx, matsize);
              continue;
            }
        
        case (INT_LBRACKET <<4 ) | INT_DOWN : //goes down of curcell each time going on loop 
            
            if(exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
                idx=OP_DOWN(idx, matsize);
              } else {
            
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
                idx=OP_DOWN(idx, matsize);
              }
              
              break;
            }else{
              curr = stack->stack[--stack_ptr];
          
              idx=OP_DOWN(idx, matsize);
              continue;
            }
        case (INT_RBRACKET <<4) | INT_DOWN : //goes down of curcell each time ending loop
           if(!exec_direction){
              if (environment->mat[idx] == 0) {
                curr = curr->other;
                idx=OP_DOWN(idx, matsize);
              } else {
            
                if (stack_ptr >= STACK_SIZE) {
                    return -1; //stack overflow
                }
                stack->stack[stack_ptr++] = curr;
                idx=OP_DOWN(idx, matsize);
              }
              
              break;
            }else{
              curr = stack->stack[--stack_ptr];
          
              idx=OP_DOWN(idx, matsize);
              continue;
            }
            
      /* silly wildcard stuff */

      case (INT_WILDCARD<<4) | INT_READ : 

            fflush(stdin);
            if(fgets(safe_getchar, 255, stdin)){
              
            default_mult_div= safe_getchar[0];
            fflush(stdin);

            memset(safe_getchar, 0, 256*sizeof(char));
            }
          
          break;
      case (INT_WILDCARD<<4) | INT_LEFT: 
          exec_direction= 0;
          break;
      
      case (INT_WILDCARD<<4) | INT_RIGHT: 
          exec_direction= 1;
          break;
   
      case INT_LPAR | ( INT_NEUT <<4):
      
        if(exec_direction){
         
          if(!checkDefaultSymbol(curr->next->symbol)){

            program * checkPresence = findProg(table, curr->next->symbol);
            if(checkPresence){
              
              program *p1= instructToProg(curr->next->other);
              struct instruction * Instruct= checkPresence->head;

              checkPresence->head=p1->head; 
              checkPresence->tail=p1->tail;

              free(p1);
              free_instruct(Instruct);


            }else{
            
              program* p1= instructToProg(curr->next->other);
              appTable(table, mkMacroEntry(curr->next->symbol, p1));
            }
          }else{
   
            free_instruct(curr->next->other);
          }

          curr=curr->other;
          break;
          }else{
     
            break;
        }
      
      case (INT_RPAR<<4) | INT_NEUT:

        if(exec_direction){
          

        }else{
     
          curr=curr->other;
        }
        break;

      default: 
        if(1){
          program * tmpProg= findProg(table, instruction);
          
          if(tmpProg){
            exec_prgm(tmpProg, environment, stack, table, printcheck);
          }
        }
         break;
    }

    OP_EXEC(curr, exec_direction);
  }

  environment->curindex=idx;
  return 0;
}