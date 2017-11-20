#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "environment.h"

lexeme* insert (lexeme *inputvar, lexeme *inputvalue, lexeme *env )
{
    lexeme *table = env->left;
    inputvar->left = table->left;
    table->left = inputvar;
    inputvalue->right = table->right;
    table->right = inputvalue;
    return inputvalue;
}

lexeme * lookup( lexeme *variable, lexeme* env)
{
    
    lexeme *table;
    lexeme *vars, *vals;
    while (env != NULL)
    {
       table =  env->left;
       vars = table->left;
       vals = table->right;
       while( vars != NULL)
       {
           if(strcmp(vars->name, variable->name ) == 0)
           {
               
              return vals;
           }   
        vars = vars->left;
        vals = vals->right;
        }
    env = env->right;
    }
fprintf(stderr, " Variable: %s is Undefined!", variable->name );
return NULL;
}

lexeme * update( lexeme *variable, lexeme *value,  lexeme* env)
{
    lexeme *table;
    lexeme *vars, *vals;
    //lexeme *temp;
    while (env != NULL)
    {
       table =  env->left;
       vars = table->left;
       vals = table->right;
        if(strcmp(vars->name, variable->name ) == 0)
        {
             value->right = vals->right;
             table->right = value;
             return vals;
        }
       while( vars->left != NULL)
       {
           if(strcmp(vars->left->name, variable->name ) == 0)
           {
               value->right = vals->right->right;
                vals->right = value;
                return vals;
           }   
        vars = vars->left;
        vals = vals->right;
    }
    env = env->right;
    }
fprintf(stdout, " SYMENTIC ERROR ~~ Line NO: %d Variable: %s is Undefined!", variable->lineNum,variable->name );

}


lexeme * extend (lexeme* varList, lexeme * valList, lexeme *env)
{
    
    lexeme *t = newLex(ENV);
    t->right = env;
    t->left = newLex(TABLE);
    t->left->left = varList;
    t->left->right = valList;
    return t;

}

lexeme *createEnv()
{
    return extend(NULL, NULL, NULL);
}