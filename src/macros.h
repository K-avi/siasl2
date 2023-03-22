#ifndef MACROS_H
#define MACROS_H

#include "parser.tab.h"
#include "ast.h"


#define _TABLE_DEF_SIZE 16
#define _ARRENT_DEF_SIZE 12

extern macrotable * init_table( unsigned short size, unsigned arrsize);

extern void free_entryarray ( entryarray * array);
extern void free_table( macrotable * mtable);

extern macroentry * mkMacroEntry ( Symbol sym, program * prog);
extern entryarray * initArr( int size);

extern void appArr( entryarray * arr, macroentry * element);
extern unsigned hashSymbol( Symbol sym, unsigned size);

extern void appTable ( macrotable * mtable, macroentry * entry);
extern program * findProg( macrotable * mtable, Symbol sym);

extern program * instructToProg( instruction * instruct);

#endif