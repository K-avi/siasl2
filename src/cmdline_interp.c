#include "cmdline_interp.h"
#include "ast.h"
#include "globals.h"
#include "lex.hh.h"

#include <stdio.h>
#include <string.h>



extern program * prog;
extern macrotable * table;

char hexTObf( char hexChr ){
   /*
   doesn't translate wildcard correctly atm 
   */
   switch (hexChr){
      
      case '0' : return '#'; break;
      case '1' : return '#'; break;
      case '2' : return '<'; break;
      case '3' : return '>'; break;
      case '4' : return '^'; break;
      case '5' : return 'v'; break;
      case '6' : return '+'; break;
      case '7' : return '-'; break;
      case '8' : return '*'; break;
      case '9' : return '/'; break;

      case 'a' : return ','; break;
      case 'b' : return '.'; break;
      case 'c' : return '['; break;
      case 'd' : return ']'; break;
      case 'e' : return '?'; break;
      case 'f' : return '#'; break;

      case 'A' : return ','; break;
      case 'B' : return '.'; break;
      case 'C' : return '['; break;
      case 'D' : return ']'; break;
      case 'E' : return '?'; break;
      case 'F' : return '#'; break;

      default: return -1 ;
   }

   return -1;
}


void interactive_interp( CELLMATRIX * environment, S_STACK * stack, unsigned char mode){
    
    if(!(mode == 'x' || mode=='s')) return ;

    char line[256];

    fflush(stdin); 
    fflush(stdout);

    char lprint=1;

    unsigned char printcheck=0;
    

    unsigned char syntax_err=0;

    printf("welcome to the (SIASL)² interactive interpreter!\n>>> ");
    while( fgets(line, 256, stdin)){

         
         fflush(stdin); 
         fflush(stdout);

         char * tmp= line; 

         while((*tmp==' '|| *tmp=='\t' || *tmp=='\n')) ++tmp;
         if((*tmp)=='\0' && lprint) {
            printf(">>> ");
            continue;
         }else if ((*tmp)=='\0'){
            lprint=1; 
            continue;
         }
         
         if(!lprint) lprint=1;
       
         if(!strncmp(line, "quit", 4)){
            break;
         }
         if(mode=='s'){

            yy_scan_string(line);
            progempty=0;
            syntax_err= yyparse(); 

         }else if (mode=='x'){
            char line_translation[256]; 
            unsigned char i=0;
            while(line[i]){

               line_translation[i]=hexTObf(line[i]);
               i++;
            }
            line_translation[i]=0;

            yy_scan_string(line_translation);
            progempty=0;
            syntax_err= yyparse(); 
         }
         
 
         if(!syntax_err) {

            exec_prgm(prog, environment, stack, table, &printcheck);
            free_prog(prog);
         }
         progempty=1;    
         yylex_destroy();

        if(printcheck){
         printf("\n>>> ");
         printcheck=0;
        }else if(syntax_err){
         printf("\n>>> ");
         printcheck=0;
        }else{
         printf(">>>");
         lprint=0;
         
        }
   }
   printf("bye!\n"); 
}