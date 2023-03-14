#include "cells.h"
#include "ast.h"
#include "cmdline_interp.h"
#include "exec.h"
#include "stack.h"
#include "globals.h"


#include "lex.yy.h"
#include "parser.tab.h"

#include "lex.hh.h"


#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <signal.h>


extern instruction * prog;

void sigint_handler( int sig){
    if(!progempty) {
        free_instruct(prog);
    }
    if(environment) free_mat(environment);
    if(stack) free_stack(stack);

    yylex_destroy();
    
    exit (SIGINT);

    return;
}

int main(int argc, char ** argv){  

    signal(SIGINT, *sigint_handler );

    u_char cmdline_mode=1, file_mode=0;

    u_char simode=1 , hexmode=0;


    u_char helpset=0;

    char* filename = NULL;
    int c;



    while ((c = getopt(argc, argv, "xshcf:")) != -1) {
        
        switch (c) {
      
        case 'h':
            helpset=1;;
            break;
        case 'x':
            
            simode=0;
            hexmode=1;
            break;
        case 's':
            
            simode=1;
            hexmode=0;
            break;
        case 'f':
            filename = optarg;
            cmdline_mode=0;
            file_mode=1;
            break;
        case 'c':
            
            cmdline_mode=1;
            file_mode=0;
            break;
      
        case '?':
            if (optopt == 'f')
            fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
            fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
            fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
       
            exit(-2);
        default:
            abort();
        }
    }

    if(helpset){

        printf("SIASL is a brainfuck like esolang; the options available are:\n-f to read a file\n-c to use the command line interactive interpret\n-x to use SIASL in hex mode;\n-s to use SIASL in symbol mode\nmore informations on the SIASL lang can be found in the README.txt file.\n"); 

        exit(0);

    }

    
    /*initialising environment and stack ; prog is initialised by parser.tab.c*/
    environment= init_mat(DEFAULT_ROWSIZE);
    stack= init_stack(STACK_SIZE);

    if (cmdline_mode) {

        if(hexmode){
            interactive_interp(environment, stack, 'x');

            free_mat(environment);
            free_stack(stack);
        }else if(simode){

            interactive_interp(environment, stack, 's');

            free_mat(environment);
            free_stack(stack);
        }else{
            
            fprintf(stderr ,"mode is neiter siasl nor hexa; exiting\n");

            free_mat(environment);
            free_stack(stack);

            exit(-2);
        }

         return 0;
         
    }else if(file_mode){

            if(hexmode){

                char* tmpfile_name= malloc(strnlen(filename, 256)+5);
                sprintf(tmpfile_name, "%s.tmp", filename);

                FILE * fsource =fopen(filename, "r");
                FILE * fdest = fopen(tmpfile_name, "w");

                hhin= fsource; 
                hhout = fdest ;

                hhlex();
                hhlex_destroy();

                fclose(fsource);
                fclose(fdest);

                yyin= fopen(tmpfile_name,"r");

                yyparse();
                progempty=0;

                fclose(yyin);
                remove(tmpfile_name);
                free(tmpfile_name);

            }else if(simode){
        
                yyin = fopen(filename, "r");
                if (!yyin) {
                    perror("Could not open file");
                    exit(-1);
                }
                yyparse();
                progempty=0;
                fclose(yyin);
            }else{
                fprintf(stderr ,"mode is neiter siasl nor hexa; exiting\n");

                free_mat(environment);
                free_stack(stack);

                exit(-2);
            }

        /*translating prog and executing*/
    
        parsed_to_int(prog);
        exec_prgm(prog, environment, stack);

        /*freeing everything after exec*/
        free_mat(environment);
        free_stack(stack);
        free_instruct(prog);
        progempty=1;

        yylex_destroy();
    }
    return 0;
}