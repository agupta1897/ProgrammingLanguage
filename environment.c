#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"environment.h"
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
                if(vals == NULL)
                {
                    fprintf(stderr, "FATAL ERROR: Value for Variable %s is NULL or Not Found\n", variable->name);
                    exit(1);
                }
            //    fprintf(stdout, "testing*************************\n");
            //    Display(stdout, vals);
              return vals-> left;
           }   
        vars = vars->left;
        vals = vals->right;
        }
    env = env->right;
    }
fprintf(stdout, " Variable: %s is Undefined!\n", variable->name );
if(variable->lineNum != 0 )
{
    fprintf(stdout, "Line Number %d\n", variable->lineNum);
}
exit(1);
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




// lexeme* copier(lexeme* var)
// {
//     if(var!= NULL)
//     {
//     lexeme * temp = malloc (sizeof(lexeme));
//     temp-> name = var->name;
//     temp-> stringVal = var->stringVal;
//     temp-> integerVal= var->integerVal;
//     temp-> type = var->type;
//     temp-> definingEnv = var->definingEnv;
//     temp-> lineNum = var->lineNum;

//     return temp;
//     }   
// return NULL;
// }


// lexeme * updateValList( lexeme* valList )
// {
//     if(valList == NULL)
//     return NULL;
//     else
//     {
//         lexeme* head = newArgToken();
//         lexeme* ptr= head;
//         ptr->left = copier (valList);

//         valList = valList->right;
//         while(valList !=NULL)
//         {
//             ptr->right = newArgToken();
//             ptr->right->left = copier(valList);
//             ptr = ptr->right;
//             valList= valList->right;
//         }
//         return head;
//     }
// }


lexeme * extend (lexeme* varList, lexeme * valList, lexeme *env)
{
    
    lexeme *t = newLex(ENV);
    t->right = env;
    t->left = newLex(TABLE);
    //lexeme *temp = updateValList(valList);

   // copier(varList);
    //printf("DONE\n");
    t->left->left = varList;
    t->left->right =  valList;
   
    // printf("Printing Environment: \n");
    // printTree( stdout, t, 2 );
     return t;
}

lexeme *createEnv()
{
    return extend(NULL, NULL, NULL);
}