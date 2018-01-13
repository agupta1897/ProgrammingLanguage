#include"scanner.h"
#include"lex.h"
#include"recognizer.h"
#include"environment.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<strings.h>

lexeme* currentLex;
FILE *fp;

int check( types type) 
        { 
        return currentLex->type == type; 
        }
lexeme* advance() 
        { 
        lexeme *temp = currentLex;
        currentLex = lex(fp); 
        return temp;
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

lexeme* match( types type) 
        { 
        matchNoAdvance(type); 
        return advance(); 
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
int modulusPending()
{
    return check(MODULUS);
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
    return check(DEFARRAY);
}

int callArrayPending()
{
    return check(CALLARRAY);
}

int obracketPending()
{
    return check(OBRACKET);
}

int displaypending()
{
    return check(DISPLAY);
}

int setArrayPending()
{
    return check(SETARRAY);
}

lexeme* mathExp();
lexeme* statement();
lexeme* callArray();
lexeme* call();

lexeme* value()
{
    if(variablePending()) return match(VARIABLE);
    else if(stringPending()) return match(STRING);
    else return match(INTEGER);
}

lexeme* restofExp()
{
    lexeme *t;
    if(plusPending()) t = match(PLUS); else
    if(dividesPending()) t= match(DIVIDES); else
    if(timesPending()) t= match(TIMES); else
    if(modulusPending()) t=match(MODULUS); else
    t= match(MINUS);

    if(oparenPending()) t->left = mathExp();
    else
    
    if(integerPending()) t->left = match(INTEGER);
    else
    if(callArrayPending()) t->left = callArray();
    else
    t->left = match(VARIABLE);
    
    if (cparenPending()) match(CPAREN);
    else
    t->right = restofExp();

    return t;
}


lexeme* mathExp()
{
    lexeme *t = newLex(MATH);
    match(OPAREN);
    if(oparenPending()) t->left = mathExp(MATH); 
    else
    if(callArrayPending()) t->left = callArray();
    else
    if (integerPending()) t->left = match(INTEGER);
    else
    t->left = match(VARIABLE);

    if (cparenPending()) match(CPAREN);
    else
    t->right = restofExp();

    return t;
    
}

lexeme* functionBody()
{
    lexeme* t;
    if(cbracketPending())
    {
        match(CBRACKET);
        return NULL;
    }
    else{
    t = statement();
    t->right = functionBody();
    return t;
    }

}

lexeme* optionalArgs()
{
    lexeme* t = match(VARIABLE);
    if (commaPending()) 
    {
        match(COMMA);
        t->left = optionalArgs();
        return t;
    }
    else
    {
    match(CPAREN);
    return t;
    }
}

lexeme* optionalCallArgs()
{
    lexeme *t;
     t= newLex(ARG);
    if(callArrayPending()) t->left = callArray();
    else
    if(oparenPending()) t->left = mathExp();
    else
    if (callPending()) t->left = call();
    else
    t->left = value();

    if (commaPending()) 
    {
        match(COMMA);
        t->right = optionalCallArgs();
        return t;
    }
    else
    {
    match(CPAREN);
    return t;
    }
}

lexeme* function ()
{
    lexeme* t = match(LAMBDA);
    match(OPAREN);
    if (cparenPending())
    {
        match(CPAREN);
        match(OBRACKET);
       t->right =  functionBody();
       return t;
    }
    else
    {
    t->left =  optionalArgs();
    match(OBRACKET);
    t->right = functionBody();
    return t;
    }

}


  

lexeme* call()
{
    lexeme *t = match(CALL);
    t->left = match (VARIABLE);
    match(OPAREN);
    if (cparenPending())
    {
        match(CPAREN);
        return t;
    }
    else{
    t->right = optionalCallArgs();
    return t;
    }
}
lexeme* definition ()
{
    lexeme *t = match(DEFINE);
    
    t->left = match(VARIABLE);
    match(ASSIGN);
    if (lambdaPending())
    {
       t->right= function();
       return t;
    }
    else
    if(oparenPending()) t->right  = mathExp();
    else
    if(callPending())
    t->right = call();
    else
    if(callArrayPending())
    t->right = callArray();
    else
    t->right = value();
    return t;
}   

lexeme* callArray()
  {
     lexeme * t = match(CALLARRAY);
    t->left = match (VARIABLE);
    match(OSBRACKET);
    if(integerPending())
     t->right = match(INTEGER);
    else if(variablePending())
    t->right = match(VARIABLE);
    else
    t->right = mathExp();
    match(CSBRACKET);
    return t;
}

lexeme* assign()
{
    lexeme* t = match(SET);
    t->left = match(VARIABLE);
    match(ASSIGN);
    if(callPending()) t->right =  call();
    else 
    if(callArrayPending()) t->right = callArray();
    else
    if(oparenPending()) t->right = mathExp();
    else
     if(stringPending()) t->right = match(STRING);
    else
    if(variablePending())
    t->right = match(VARIABLE);
    else
     t->right = match(INTEGER);
   return t;
    }
    
lexeme* restofIfCond()
{
     lexeme *t;
    if(orPending()) t = match(OR);
        else t = match(AND);

    if(callArrayPending()) t->left = callArray();
    else
    if(oparenPending()) t->left = mathExp();
    else t->left= value();

    if(orPending() || andPending())
        t->right = restofIfCond();
    else if(equalsPending() || greaterthanPending() || lessthanPending() )
    {
        if(greaterthanPending()) t->right = match(GREATERTHAN);
        else
        if(lessthanPending()) t->right =  match(LESSTHAN);
        else
        t->right = match(EQUALS);
        
        if(callArrayPending()) t->right->left = callArray();
        else
        if(oparenPending()) t->right->left = mathExp();
        else
        t->right->left =  value();

        if(orPending() || andPending())
        t->right->right = restofIfCond();
        return t;
    }
    return t;
}

lexeme * iffunc()
{
    lexeme * t = match(IF);
    match(OPAREN);
    t->left = newLex(COND);
    t->right = newLex(THEN);

     if(callArrayPending()) t->left->left = callArray();
    else
    if(oparenPending()) t->left->left = mathExp();
    else t->left->left = value();
    

    if(cparenPending()){
            match(CPAREN);        
    }
    else if(equalsPending() || greaterthanPending() || lessthanPending() )
    {   
        if(greaterthanPending()) t->left->right = match(GREATERTHAN);
        else
        if(lessthanPending()) t->left->right= match(LESSTHAN);
        else
         t->left->right = match(EQUALS);
        
        if(callArrayPending()) t->left->right->left = callArray();
        else
        if(oparenPending()) t->left->right->left= mathExp();
        else
        t->left->right->left = value();

        if(cparenPending()){
            match(CPAREN);        
            }
            else
            {
            t->left->right->right = restofIfCond();
            match(CPAREN);
            }
    }
    else
    {
         //if(orPending() )
         t->left->right = restofIfCond();
         match(CPAREN);
    }

    match(OBRACKET);
    t->right->left = functionBody();

    if (elsePending())
    {
        match(ELSE);
        match(OBRACKET);
        t->right->right = functionBody();
    }
    return t;
}

lexeme* displaying()
{
    lexeme * t = match(DISPLAY);
    if (callPending()) t->left = call();
    else
    if (callArrayPending()) t->left = callArray();
    else
    if (callPending())
    t->left = call();
    else
    if (oparenPending()) t->left = mathExp();
    else
    t->left = value();
    return t;
}

lexeme*  whileloop()
{
    lexeme *t = match(WHILE);
    match(OPAREN);
    t->left = newLex(COND);

    if(callArrayPending()) t->left->left = callArray();
    else
    if(oparenPending()) t->left->left = mathExp();
    else t->left->left = value();

    if(greaterthanPending()) t->left->right = match(GREATERTHAN);
    else
    if(lessthanPending()) t->left->right = match(LESSTHAN);
    else
    {    
    t->left->right = match(EQUALS);
    }

    if(callArrayPending()) t->left->right->left = callArray();
    else
    if(oparenPending()) t->left->right->left = mathExp();
    else  t->left->right->left = value();

    if(orPending() || andPending())
        t->left->right->right = restofIfCond();

    match (CPAREN);
    match (OBRACKET);
   t->right= functionBody();
   return t;
}

lexeme *optionalArrayElem()
{   
    lexeme *t;
   
    t = value();
    if(commaPending())
    {
        match(COMMA);
        t->left = optionalArrayElem();
    }
    return t;
}
lexeme* array()
{
    lexeme* t= match(DEFARRAY);
    t->left = match (VARIABLE);
    match(OSBRACKET);
    if(integerPending())
    t->right = match(INTEGER);
    else
    t->right = match(VARIABLE);

  
    match(CSBRACKET);
      match(ASSIGN);
    if(obracketPending())
    {
        match(OBRACKET);
        t->right->left = optionalArrayElem();
        match(CBRACKET);
    }
    return t;
}

lexeme * setArray(){
    lexeme* t= match(SETARRAY);
    t->left = match(VARIABLE);
    match(OSBRACKET);

    if(integerPending())
    t->right = match(INTEGER);
    else
   // if(variablePending())
    t->right = match(VARIABLE);


    match(CSBRACKET);
    match(ASSIGN);

    if(oparenPending())
    {
    t->right->left = mathExp();
    }
    else
    if(callPending())
    {
        t->right->left = call();
    }
    else
    t->right->left = value();

  return t;
}

lexeme* statement()
{
    lexeme* t = newLex(NEXT);
    if(integerPending())
    {
        t->left = match(INTEGER);
    }
    else
    if(stringPending())
    {
        t->left = match(STRING);
    }
    else
    if(oparenPending())
    {
        t->left = mathExp();
    }
    else
    if(variablePending())
    {
        t->left = match(VARIABLE);
    }
    else
    if(lambdaPending())
    {
        t->left = function();
    }
    else
    if(definitionPending()) 
        t->left = definition();
     else
    if(setPending()) t->left = assign();
     else
     if(callPending()) t->left =  call();
     else
     if(ifPending())t->left = iffunc();
        else
    if(whilePending()) t->left = whileloop();
    else
    if(arrayPending()) t->left =  array();
    else
    if (callArrayPending())
     t->left = callArray();
    else
    if(setArrayPending())
        t->left = setArray();
    else
    if(displaypending())
    t->left = displaying();
    match(SEMICOLON);
    return t;
}

int indent = 0;
int nextcount = 0;
lexeme* parent;
int flag1 = 0;
lexeme* program( lexeme *t)
{
    lexeme *x = parent;
    if (flag1 == 0)
    {
        parent = t;
        flag1 = 1;
    }

    if (!check(END_OF_INPUT))
    {

        // if(ifPending())
        // {   
        // lexeme* t1 = newLex(NEXT);

        //    t1->left=  iffunc();
        //    t->right =t1;
        //    return program (t->right);
        // }
        
        t->right = statement();
        return program(t->right);

        }
    else
    return x;

    
}

    void indentprinter(FILE *fp)
    {
        for (int i = 0 ; i< indent; i++)
        {
            fprintf(fp," ");
        }
    }
    void printTree(FILE *fp, lexeme* tree, int flag){

        if (tree == NULL)
        {   
        return;
        }

        if(flag == 1)
        {
            indentprinter(fp);
            fprintf(fp, "Left  : ");
            Display(fp, tree);
        }
        else
        {
            indentprinter(fp);
            fprintf(fp, "Right : ");
            Display(fp, tree);
        }
        
        indent++;
        indent++;
        printTree(fp, tree->left, 1);
        
        printTree(fp,tree->right, 2);
        indent--;
        indent--;
    }

lexeme* recognize(FILE *file)
{   
    lexeme *t = newLex(PROGRAM);
    fp = file;
    currentLex = lex(fp);
    lexeme *x;
    x = program(t);
   // Display(stdout, x);
    
    return x;
}