#include "cells.h"
#include "ast.h"
#include "cmdline_interp.h"
#include "exec.h"
#include "macros.h"
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


extern program * prog;

void sigint_handler( int sig){
    if(!progempty) {
        free_prog(prog);
    }
    if(environment) free_mat(environment);
    if(stack) free_stack(stack);

    if(table) free_table(table);

    yylex_destroy();
    printf("\n");
    exit (SIGINT);

    return;
}

int main(int argc, char ** argv){  

    signal(SIGINT, *sigint_handler );

    unsigned char cmdline_mode=1, file_mode=0;
    unsigned char simode=1 , hexmode=0;
    unsigned char helpset=0;

    char* filename = NULL;
    int c;

    printf("(SIASL)² Copyright (C) 2023  Ivan MULOT-RADOJCIC\nThis program comes with ABSOLUTELY NO WARRANTY;\nfor details see the GPLv3 documentation.\nThis is free software, and you are welcome to redistribute it under certain conditions\n\n");

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
            fprintf(stderr, "Unknown option character `\\x%x'.", optopt);
       
            exit(-2);
        default:
            abort();
        }
    }

    if(helpset){

        printf("(SIASL)² is a brainfuck like esolang; the options available are:\n-f to read a file\n-c to use the command line interactive interpret\n-x to use the interpreter in hex mode;\n-s to use the interpreted in symbol mode\nmore informations on the (SIASL)² language can be found in the README.txt and docu/docu.html files.\n"); 
        exit(0);
    }
    
    /*initialising environment and stack ; prog is initialised by parser.tab.c*/
    environment= init_mat(DEFAULT_ROWSIZE);
    stack= init_stack(STACK_SIZE);
    table=init_table(_TABLE_DEF_SIZE, _ARRENT_DEF_SIZE);

    if (cmdline_mode) {

        if(hexmode){
            interactive_interp(environment, stack,'x');

            free_mat(environment);
            free_stack(stack);
            free_table(table);

        }else if(simode){

            interactive_interp(environment, stack,'s');

            free_mat(environment);
            free_stack(stack);
            free_table(table);

        }else{
            
            fprintf(stderr ,"mode is neiter symbol nor hexa; exiting\n");

            free_mat(environment);
            free_stack(stack);
            free_table(table);

            exit(-2);
        }

         return 0;
         
    }else if(file_mode){

            if(hexmode){

                

                FILE * fsource =fopen(filename, "r");
                if(!fsource){
                    
                    free_mat(environment);
                    free_stack(stack);
                    free_table(table);

                    fprintf(stderr, "Could not open file : exiting \n");
                    return -1;
                }

                char* tmpfile_name= malloc(strnlen(filename, 256)+5);
                sprintf(tmpfile_name, "%s.tmp", filename);
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
                    fprintf(stderr, "Could not open file : exiting \n");

                    free_mat(environment);
                    free_stack(stack);
                    free_table(table);

                    exit(-1);
                }
                yyparse();
                progempty=0;
                fclose(yyin);
            }else{
                fprintf(stderr ,"mode is neiter symbol nor hexa : exiting\n");

                free_mat(environment);
                free_stack(stack);

                exit(-2);
            }

        /*translating prog and executing*/
    
        unsigned char printcheck;
        exec_prgm(prog, environment, stack, table , &printcheck);

        /*freeing everything after exec*/
        free_mat(environment);
        free_stack(stack);
        free_prog(prog);
        free_table(table);
        
        progempty=1;

        yylex_destroy();
    }
    printf("\n");
    return 0;
}