#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "environment.h"
#include"recognizer.h"

lexeme* insert (lexeme *inputvar, lexeme *inputvalue, lexeme *env )
{
    lexeme *table = env->left;
    inputvar->left = table->left;
    table->left = inputvar;
    lexeme *arg = newLex(ARG);
    arg->left = inputvalue;
    arg->right = table->right;
    table->right = arg;
   // inputvalue->right = table->right;
   
    return arg;
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
               
              return vals-> left;
           }   
        vars = vars->left;
        vals = vals->right;
        }
    env = env->right;
    }
fprintf(stderr, " Variable: %s is Undefined!", variable->name );
return NULL;
}

lexeme * update( lexeme *variable, lexeme *value1,  lexeme* env)
{
    lexeme *table;
    lexeme *vars, *vals;
    lexeme *value = newLex(ARG);
    value->left = value1;
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
exit(1);
}

lexeme * newArgToken(){
    return newLex(ARG);
}


lexeme * remove1( lexeme* valList)
{
    lexeme* head = valList;
    while(valList != NULL)
    {
        valList->left->right = NULL;
        valList = valList->right;
    }
    return head;
}

lexeme * updateValList( lexeme* valList )
{
    if(valList == NULL)
    return NULL;
    else
    {
        lexeme* head = newArgToken();
        lexeme* ptr= head;
        ptr->left = valList;
        
        valList = valList->right;
        while(valList !=NULL)
        {
            ptr->right = newArgToken();
            ptr->right->left = valList;
            ptr = ptr->right;
            valList= valList->right;
        }
        return remove1(head);
    }
}

lexeme * extend (lexeme* varList, lexeme * valList, lexeme *env)
{
    
    lexeme *t = newLex(ENV);
    t->right = env;
    t->left = newLex(TABLE);

   // printTree( stdout, updateValList(valList) , 2 );

  //  printf("DONE\n");
    t->left->left = varList;
    t->left->right =  updateValList(valList);
    return t;

}

lexeme *createEnv()
{
    return extend(NULL, NULL, NULL);
}