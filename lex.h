#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifndef __LEX_INCLUDED__
#define __LEX_INCLUDED__


typedef enum
{
    OPAREN,
    CPAREN,
    COMMA,
    PLUS,
    TIMES,
    MINUS,
    DIVIDES,
    OBRACKET,
    CBRACKET,
    SEMICOLON,
    STRING,
    INTEGER,
    TILDE,
    END_OF_INPUT,
    VARIABLE,
    COMMENT,
    DEFINE,
    Null,
    SET,
    CALL,
    ASSIGN,
    UNKNOWN
    
} types;


typedef struct lexeme
    {
        char* name;
        char* stringVal;
        int integerVal;
        types type;
        int lineNum;

    } lexeme;

extern lexeme *newLex (types type);
extern lexeme *lex ();
extern void Display (FILE* fp, lexeme *l);
extern lexeme *newIntLex (FILE *fp);
extern lexeme *newVariableLex (FILE *fp);
extern lexeme *newStringLex (FILE *fp);
extern void recognize(FILE *fp);

#endif