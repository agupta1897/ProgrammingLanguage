#include"scanner.h"
#include"lex.h"
#include"environment.h"
#include"recognizer.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

lexeme* eval (lexeme* tree, lexeme* env);
lexeme *evalMath(lexeme *tree, lexeme *env);
lexeme *evalSimpleOp( lexeme* tree, lexeme *env, lexeme* eleftOparant);
lexeme *evalVarDefine(lexeme *tree, lexeme *env)
{
    lexeme* var = tree->left;

    lexeme* val = tree->right;
   

    return insert (var ,eval(val, env), env);
    // if(val->type == MATH)
    // {
    //     lexeme * t = evalMath(val, env);
    //     return insert(var, t, env);
    // }

    // return insert(var, val, env);
}

lexeme *evalSet (lexeme *tree, lexeme *env)
{

    lexeme* var = tree->left;
    lexeme* val;
    if(tree->right->type == MATH)
    {
        val = evalMath(tree->right, env);
    }
    else
    val = tree->right;
    update(var, val, env);
    return tree;
}

lexeme* evalPlus( lexeme* tree, lexeme* env, lexeme* eleftOparant)
{
    lexeme* eRightOparant = eval(tree->left, env);
    lexeme * result =  newLex(INTEGER);
    result->integerVal = eleftOparant->integerVal + eRightOparant->integerVal;
    result->lineNum = eRightOparant->lineNum;
    if(tree->right != NULL)
    {
        
        return evalSimpleOp(tree->right, env, result);
    }

    return result;
}

lexeme* evalMinus( lexeme* tree, lexeme* env, lexeme* eleftOparant)
{
    lexeme* eRightOparant = eval(tree->left, env);
    lexeme * result =  newLex(INTEGER);
    result->integerVal = eleftOparant->integerVal - eRightOparant->integerVal;
    result->lineNum = eRightOparant->lineNum;
    if(tree->right != NULL)
    {
        
        return evalSimpleOp(tree->right, env, result);
    }

    return result;
}

lexeme* evalDivides( lexeme* tree, lexeme* env, lexeme* eleftOparant)
{
    lexeme* eRightOparant = eval(tree->left, env);
    lexeme * result =  newLex(INTEGER);
    result->integerVal = eleftOparant->integerVal / eRightOparant->integerVal;
    result->lineNum = eRightOparant->lineNum;
    if(tree->right != NULL)
    {
        
        return evalSimpleOp(tree->right, env, result);
    }

    return result;
}

lexeme* evalCall( lexeme* tree, lexeme *env)
{
    
}

lexeme* evalTimes( lexeme* tree, lexeme* env, lexeme* eleftOparant)
{
    lexeme* eRightOparant = eval(tree->left, env);
    lexeme * result =  newLex(INTEGER);
    result->integerVal = eleftOparant->integerVal * eRightOparant->integerVal;
    result->lineNum = eRightOparant->lineNum;
    if(tree->right != NULL)
    {
        return evalSimpleOp(tree->right, env, result);
    }
    return result;
}

lexeme *evalSimpleOp( lexeme* tree, lexeme *env, lexeme* eleftOparant)
{
    
    if(tree->type == PLUS) 
       return evalPlus(tree, env, eleftOparant);
     else
     if(tree->type == MINUS)
         evalMinus(tree, env, eleftOparant);
    else
    if(tree->type == DIVIDES)
        evalDivides(tree, env, eleftOparant);
    else
        evalTimes(tree, env, eleftOparant);
}

lexeme *evalMath(lexeme *tree, lexeme *env)
{
    lexeme *left = tree->left;
    lexeme *right = tree->right;

    lexeme *evaluatedleft= eval(left, env);

    return evalSimpleOp(right, env, evaluatedleft);
    // if(left->type == MATH)
    // {
    //     eval(left, env);
    // }
    
}

lexeme *evalDisplay(lexeme *tree, lexeme *env)
{
    lexeme *value = lookup(tree->left, env);
    if (value == NULL)
    {
        exit(1);
    }
    else
    {
        if(value->type == INTEGER)
        {
            fprintf(stdout, "%d", value->integerVal);
        }
        else
        if(value->type == STRING)
        {
            fprintf(stdout,"%s", value->stringVal);
        }
    }
    return tree;
}

lexeme* eval (lexeme* tree, lexeme* env)
{
    switch(tree->type)
    {
        case NEXT:
            eval(tree->left, env);
            if(tree->right != NULL)
            {
                tree = tree->right;
                eval(tree, env);
            }
            return NULL;
        case MATH:
            return evalMath(tree, env);
        case INTEGER:
            return tree;
        case STRING:
            return tree;
        case SET:
            return evalSet(tree, env);
        case LAMBDA:
            return extend(tree->left, NULL, env);
        case DEFINE:
            return evalVarDefine(tree, env);
        case DISPLAY:
            return evalDisplay(tree, env);
        case CALL:
            return evalCall(tree, env);
        default:
            return tree;
    }
}

   
int main (int argc, char** argv)
{
    FILE *fp = fopen(argv[1],"r");
    lexeme *tree = recognize(fp);
    printTree(stdout, tree->right, 2);
    lexeme* env = createEnv();
    eval(tree->right, env );
    printTree(stdout, env, 2);
     

    // lexeme *token = malloc(sizeof(lexeme));
    // token->type = INTEGER;
    // token->integerVal = 30;
    
    // lexeme *var = malloc(sizeof(lexeme));
    // var->type = VARIABLE;
    // var->name = "Var1";
   
    // lexeme *token2 = malloc(sizeof(lexeme));
    // token2->type = INTEGER;
    // token2->integerVal = 33;


    // lexeme *var2 = malloc(sizeof(lexeme));
    // var2->type = VARIABLE;
    // var2->name = "Var2";

    // lexeme *token3 = malloc(sizeof(lexeme));
    // token3->type = INTEGER;
    // token3->integerVal = 44;

    
    // lexeme *var3 = malloc(sizeof(lexeme));
    // var3->type = VARIABLE;
    // var3->name = "Var3";


    // lexeme *env  = createEnv();
    // insert(var, token, env);
    // insert(var2, token2, env);
    // insert(var3, token3, env);


    // printTree(stdout, env , 2);
    // token2->integerVal = 894;

    //  update(var2, token2, env);
    // printTree(stdout, env , 2);
    

    // lexeme *var4 = malloc(sizeof(lexeme));
    // var4->type = VARIABLE;
    // var4->name = "Var4";


    //  lexeme *value = lookup(var4, env);
   
    

    fclose(fp);
    fprintf (stdout, "YEAHHH!\n");
    return 0;
}