#include"scanner.h"
#include"lex.h"
#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include<string.h>
#include<strings.h>


lexeme* currentLex;
FILE *fp;

int check( types type) 
        { 
        return currentLex->type == type; 
        }
void advance() 
        { 
        //lexeme *temp = currentLex;

        currentLex = lex(fp); 
        //return temp;
        } 

  
void matchNoAdvance(types type)
        {
        if (!check(type))
        {
            fprintf(stdout,"SYNTAX ERROR ~~ Line No: %d Expected -", currentLex->lineNum );
            lexeme *temp = malloc(sizeof(lexeme));
            temp->type = type;
            Display(stdout, temp);
            fprintf(stdout,"GOT: ");
            Display(stdout, currentLex);
            exit(-1);
        }
    }

void match( types type) 
        { 
        matchNoAdvance(type);
        advance(); 
        //return advance(); 
        }

int variablePending()
{
    return check(VARIABLE);
}

int stringPending()
{
    return check(STRING);
}

int integerPending()
{
    return check(INTEGER);
}

int definitionPending()
{
    return check(DEFINE);
}

int setPending()
{
    return check(SET);
}
int callPending()
{
    return check(CALL);
}

int cparenPending()
{
    return check(CPAREN);
}
int plusPending()
{
    return check(PLUS);
}
int dividesPending()
{
    return check(DIVIDES);
}
int timesPending()
{
    return check(TIMES);
}
int minusPending()
{
    return check(MINUS);
}
int oparenPending()
{
    return check(OPAREN);
}
int commaPending()
{
    return check(COMMA);
}
int lambdaPending()
{
    return check(LAMBDA);
}
int cbracketPending()
{
    return check(CBRACKET);
}

int orPending()
{
    return check(OR);
}

int andPending()
{
    return check(AND);
}

int ifPending()
{
    return check(IF);
}
int elsePending()
{
    return check(ELSE);
}
int equalsPending()
{
    return check(EQUALS);
}

int whilePending()
{
    return check(WHILE);
}

int greaterthanPending()
{
    return check(GREATERTHAN);
}
int lessthanPending()
{
    return check(LESSTHAN);
}
int arrayPending()
{
    return check(ARRAY);
}

void mathExp();
void statement();

void value()
{
    if(variablePending()) match(VARIABLE);
    else if(stringPending()) match(STRING);
    else match(INTEGER);
}

void restofExp()
{
    
    if(plusPending()) match(PLUS); else
    if(dividesPending()) match(DIVIDES); else
    if(timesPending()) match(TIMES); else
    match(MINUS);

    if(oparenPending()) mathExp();else
    if(integerPending()) match(INTEGER);else
    match(VARIABLE);

    if (cparenPending()) match(CPAREN);
    else
    restofExp();

}


void mathExp()
{
    match(OPAREN);
    if(oparenPending()) mathExp(); else
    if (integerPending()) match(INTEGER);
    else
    match(VARIABLE);

    if (cparenPending()) match(CPAREN);
    else
    restofExp();
    
}

void functionBody()
{
    if(cbracketPending())
    {
        match(CBRACKET);
    }
    else{
    statement();
    functionBody();
    }
}

void optionalArgs()
{
    match(VARIABLE);
    if (commaPending()) 
    {
        match(COMMA);
        optionalArgs();
    }
    else
    {
    match(CPAREN);
    match(OBRACKET);
    functionBody();
    }
}

void optionalCallArgs()
{
    value();
    if (commaPending()) 
    {
        match(COMMA);
        optionalCallArgs();
    }
    else
    {
    match(CPAREN);
    }
}

void function ()
{
    match(LAMBDA);
    match(OPAREN);
    if (cparenPending())
    {
        match(CPAREN);
        match(OBRACKET);
        functionBody();
    }
    else
    optionalArgs();

}


void definition ()
{
    match(DEFINE);
    match(VARIABLE);
    match(ASSIGN);
    if (lambdaPending())
    {
        function();
    }
    else
    value();
}     

void call()
{
    match(CALL);
    match (VARIABLE);
    match(OPAREN);
    if (cparenPending())
    {
        match(CPAREN);
    }
    else
    optionalCallArgs();

}

void assign()
{
     match(SET);
    match(VARIABLE);
    match(ASSIGN);
    //if(callPending()) call();
    if(oparenPending()) mathExp();
    else
     if(stringPending()) match(STRING);
    else
     match(INTEGER);
   // return t;
    }
    
void restofIfCond()
{
    if(orPending()) match(OR);
        else match(AND);

    if(oparenPending()) mathExp();
        else value();

    if(orPending() || andPending())
        restofIfCond();
    else if(equalsPending() || greaterthanPending() || lessthanPending() )
    {
        if(greaterthanPending()) match(GREATERTHAN);
        else
        if(lessthanPending()) match(LESSTHAN);
        else
        match(EQUALS);

        if(oparenPending()) mathExp();
        else
        value();
        if(orPending() || andPending())
        restofIfCond();
    }
}

void iffunc()
{
    match(IF);
    match(OPAREN);

    if(oparenPending()) mathExp();
    else value();
    

    if(cparenPending()){
            match(CPAREN);        
    }
    else if(equalsPending() || greaterthanPending() || lessthanPending() )
    {   
        if(greaterthanPending()) match(GREATERTHAN);
        else
        if(lessthanPending()) match(LESSTHAN);
        else
        match(EQUALS);
        if(oparenPending()) mathExp();
        else
        value();
        if(cparenPending()){
            match(CPAREN);        
            }
            else
            restofIfCond();
            match(CPAREN);
    }
    else
    {
         //if(orPending() )
         restofIfCond();
         match(CPAREN);
    }

    match(OBRACKET);
    functionBody();

    if (elsePending())
    {
        match(ELSE);
        match(OBRACKET);
        functionBody();
    }
}

void whileloop()
{
    match(WHILE);
    match(OPAREN);

    if(oparenPending()) mathExp();
    else value();

    if(greaterthanPending()) match(GREATERTHAN);
    else
    if(lessthanPending()) match(LESSTHAN);
    else
    {
        
    match(EQUALS);
    }
    if(oparenPending()) mathExp();
    else value();
    match (CPAREN);
    match (OBRACKET);
    functionBody();
}

void array()
{
    match(ARRAY);
    match (VARIABLE);
    match(OSBRACKET);
    if(integerPending())
    match(INTEGER);
    else
    match(VARIABLE);
    match(CSBRACKET);
}

void statement()
{
    if(definitionPending()) definition();
    else
    if(setPending()) assign();
    else
    if(callPending()) call();
    else
    if(ifPending()) iffunc();
    else
    if(whilePending()) whileloop();
    else
    if(arrayPending()) array();
    match(SEMICOLON);
}



void program()
{
    if (check(END_OF_INPUT)) return;
    else
        statement();
    program();
}

void recognize(FILE *file)
{
    fp = file;
    currentLex = lex(fp);
    program();
}