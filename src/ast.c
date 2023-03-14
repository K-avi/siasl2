#include "ast.h"
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>




Symbol symbol_from_syllable( unsigned char sil1 , unsigned char  sil2){
    
   
     if (sil1 > sil2) {
        Symbol ret= {sil2, sil1};
        return ret;
     }else {
        Symbol ret= {sil1, sil2};
        return ret;
     }

}

instruction * mkinstruction(int token) {
  instruction* ret = (instruction*)malloc(sizeof(instruction));
  ret->token = token;
  ret->next = NULL;
 
  ret->other = NULL;
  return ret;
}

//append would be better if list was doubly linked cuz O(1) vs O(n) and shit...
instruction * add_instruction( instruction* program, instruction * instruct){
   
    instruction* curr = program;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = instruct;
    return program;
}


void free_instruct( instruction* parsed_prog){
    
    if(!parsed_prog) return;
    
    instruction* tmp= parsed_prog, *tmp1=parsed_prog;
  
    while (tmp){

        tmp1=tmp1->next;

        free(tmp);

        tmp=tmp1;
    }
}

int bison_token_to_internal( int token){
    /*
    converts flex/bison generated token to internal rep; 
    will be usefull down the line cuz multiple parsers n shit
    */
   
    switch (token){
        case PRINT: return INT_PRINT; break;
        case READ: return INT_READ; break;
        case LEFT: return INT_LEFT; break;
        case RIGHT: return INT_RIGHT; break; 
        case UP: return INT_UP; break;
        case DOWN: return INT_DOWN; break;
        case PLUS: return INT_PLUS; break;
        case MINUS: return INT_MINUS; break;
        case MULT: return INT_MULT; break;
        case DIV: return INT_DIV; break;
        case NEUTRAL: return INT_NEUT;
        case LBRACKET: return INT_LBRACKET; break;
        case RBRACKET: return INT_RBRACKET; break;
        default: return -1;
        
    }

    return -1;
}//not tested

int parsed_to_int( instruction * parsed_prog){
    /*
    turns the list of parsed instruction tokens retrieved via flex/bison 
    into the list of internal instructions. 
    This is gonna be VERY usefull down the line cuz there will be multiple parsers for the language
    */
    instruction* curr= parsed_prog;
   
    while(curr){
       
        int curtoken= bison_token_to_internal(curr->token);
        if(curtoken<=0){
            return curtoken; //return token error
        }

        curr->token=curtoken;
     
        curr=curr->next; 
    }
    return 1; //returns OK
}//not tested


char token_to_char(int token){
    
    switch (token) {
    case INT_LEFT : return '<'; break;
    case INT_RIGHT: return '>'; break;
    case INT_UP : return '^'; break;
    case INT_DOWN : return 'v'; break;
    case INT_PLUS : return '+'; break;
    case INT_MINUS : return '-'; break;
    case INT_MULT : return '*'; break;
    case INT_DIV : return '/'; break;
    case INT_LBRACKET : return '['; break;
    case INT_RBRACKET : return ']'; break;
    case INT_NEUT: return '#'; break;
    case INT_READ: return ','; break;
    case INT_PRINT: return '.'; break;
    default : return '?'; break;
    }

    return '?';
}

void printprgm( instruction * prgm){

    instruction * cur=prgm; 

    while(cur){
        printf("%c", token_to_char(cur->token));
        cur=cur->next;
    }
    printf("\n");
}


