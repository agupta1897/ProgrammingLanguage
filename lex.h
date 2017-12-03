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
    EQUALS,
    LAMBDA,
    IF,
    ELSE,
    COND,
    THEN,
    WHILE,
    OR,
    AND,
    LESSTHAN,
    GREATERTHAN,
    DEFARRAY,
    SETARRAY,
    CALLARRAY,
    OSBRACKET,
    CSBRACKET,
    PROGRAM,
    NEXT,
    MATH,
    DISPLAY,
    ENV,
    TABLE,
    ARG,
    MODULUS,
    UNKNOWN
    
} types;


typedef struct lexeme
    {
        char* name;
        char* stringVal;
        int integerVal;
        types type;
        int lineNum;
        struct lexeme *right;
        struct lexeme *left;
        struct lexeme *definingEnv;
        struct lexeme **array;

    } lexeme;

extern lexeme *newLex (types type);
extern lexeme *lex (FILE *fp);
extern void Display (FILE* fp, lexeme *l);
extern lexeme *newIntLex (FILE *fp);
extern lexeme *newVariableLex (FILE *fp);
extern lexeme *newStringLex (FILE *fp);
//extern void recognize(FILE *fp);

#endif