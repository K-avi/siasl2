#include "cmdline_interp.h"
#include "ast.h"
#include "globals.h"
#include "lex.hh.h"

#include <stdio.h>
#include <string.h>



extern instruction * prog;


char hexTObf( char hexChr ){
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
      case 'e' : return '#'; break;
      case 'f' : return '#'; break;

      case 'A' : return ','; break;
      case 'B' : return '.'; break;
      case 'C' : return '['; break;
      case 'D' : return ']'; break;
      case 'E' : return '#'; break;
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

   // printf("before loop %p %p %p\n", prog, environment, stack);
    printf("welcome to the SIASL interactive interpreter!\n>>> ");
    while( fgets(line, 256, stdin)){

         
         fflush(stdin); 
         fflush(stdout);

         char * tmp= line; 

         while(*tmp==' '|| *tmp=='\t' || *tmp=='\n') ++tmp;
         if((*tmp)=='\0') {
            printf(">>> ");
            continue;
         }
       
         if(!strncmp(line, "quit", 4)){
            break;
         }
         if(mode=='s'){

            yy_scan_string(line);
            progempty=0;
            yyparse(); 

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
            yyparse(); 
         }
         

         parsed_to_int(prog);
 
         exec_prgm(prog, environment, stack);
         
         
      
          free_instruct(prog); 
          progempty=1;    
          yylex_destroy();

        if(mode =='s' && strchr(line, '.')){
         printf("\n>>> ");
        }else if(mode =='x' && strchr(line, 'b')){
         printf("\n>>>");
        }else{
         printf(">>>");
        }
   }
   printf("bye!\n"); 
}