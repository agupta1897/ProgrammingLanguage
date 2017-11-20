#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lex.h"

#ifndef __ENVIRONMENTS_INCLUDED__
#define __ENVIRONMENTS_INCLUDED__

extern lexeme * insert (lexeme *inputvar, lexeme *inputvalue, lexeme *env );
extern lexeme * lookup( lexeme *variable, lexeme* env);
extern lexeme * extend (lexeme* varList, lexeme * valList, lexeme *env);
extern lexeme * createEnv();
extern lexeme * update( lexeme *variable, lexeme *value,  lexeme* env);


#endif


