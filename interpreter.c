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
lexeme* evaluateCond( lexeme* tree, lexeme* env);

lexeme* evalDefArray(lexeme *tree, lexeme *env)
{
    lexeme* var = tree->left;
    lexeme* val = eval(tree->right,env);
    int size  = val->integerVal;
    lexeme *values = tree->right->left;
    val->array = malloc( (sizeof(lexeme))*((val->integerVal) + 1));

    int index = 0;
    while(values!=NULL )
    {
        if(index >= size)
        {
            fprintf(stdout, "SYMANTIC ERROR: Increase the Size of the Array! - Current Size: %d\n", size);
           
            if(tree->left->lineNum != 0)
            fprintf(stdout, "Line No: %d\n", tree->left->lineNum);
            exit(1);
        }
        val->array[index] = eval(values, env);
        index++;
        values = values->left;
    }
    val->array[index] = newLex(Null);
    return insert(var, eval(val, env), env );
}

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
    Display(stdout, var);
    printf(" Printing The Set Value:");
    printTree(stdout, tree->right,1);
    val = eval(tree->right, env);
    Display(stdout, var);
    printf("Printing The Set Value after evaluation:");
    printTree(stdout, val,1);
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

lexeme* evalModulus( lexeme* tree, lexeme* env, lexeme* eleftOparant)
{
    lexeme* eRightOparant = eval(tree->left, env);
    lexeme * result =  newLex(INTEGER);
    result->integerVal = (eleftOparant->integerVal % eRightOparant->integerVal);
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


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

lexeme* copier1(lexeme* var)
{

    if(var!= NULL)
    {
    if(var->type == LAMBDA)
    {
        lexeme *head = newLex(ARG);
        head->left = var;
        return head;
    }
    lexeme * temp = malloc (sizeof(lexeme));
    temp-> name = var->name;
    temp-> stringVal = var->stringVal;
    temp-> integerVal= var->integerVal;
    temp-> type = var->type;
    temp-> definingEnv = var->definingEnv;
    temp-> lineNum = var->lineNum;

    lexeme *head = newLex(ARG);
    head->left = temp;
    return head;
    }   
return NULL;
}

lexeme* evalArgs(lexeme* list, lexeme* env)
{

 if(list == NULL)
 {
     return NULL;
    }
    else
    {
       lexeme *newList;
       lexeme *head;
    //    if(list->type ==LAMBDA)
    //    {
    //         return list;
    //    }
    //    else
       if( list->left->type == VARIABLE)
       {
           newList = copier1 (lookup(list->left, env));
           head = newList;
        }
        else
            newList = copier1(list->left);
            head = newList;
        
         while(list->right != NULL)
         {
             list = list->right;
              if( list->left->type == VARIABLE)
                {
                    newList->right = copier1 (lookup(list->left, env));  
                    newList = newList ->right;
                }
                else
                {
                    newList->right = copier1(list->left);
                    newList = newList->right;
                }
        }
        return head;
    }
}


lexeme* evalFunc( lexeme *body, lexeme*env)
{
    lexeme* result;
    lexeme*body1 = body;
    // printf("PRINTING BODY OF FUNCTION CALL");
    // printTree(stdout, body1, 2);
    result = eval(body, env);
    //Display(stdout, result);
    if(result== NULL)
    {
        fprintf(stdout, "Result in NULL");
    }
    // fprintf(stdout, "Result to call:" );
    // Display(stdout, result);
    return result;
}


lexeme* evalCall( lexeme* tree, lexeme *env)
{
    lexeme *lambda = lookup(tree->left,env );
    lexeme *definingEnv = lambda->definingEnv;
    lexeme *varlist = lambda->left;
    lexeme *body = lambda->right;
    
    lexeme *valList = evalArgs(tree->right, env);

    lexeme *extendedEnv = extend(varlist, valList, definingEnv);
    //  printf("Printing Value List after evalArrgs\n");
    // printTree( stdout,tree->right , 2 );


    return evalFunc(body, extendedEnv);
}

lexeme* evalCallArray( lexeme* tree, lexeme *env)
{
  lexeme *x = lookup(tree->left, env);
  return x->array[eval(tree->right, env)->integerVal];
}

lexeme* evalSetArray(lexeme* tree, lexeme* env)
{
    lexeme *x = lookup(tree->left, env);
    int index = eval(tree->right, env)->integerVal;
    if  (x->integerVal <=  index)
    {
        fprintf(stdout, "SIMANTIC ERROR: Increase the Size of the Array! - Current Size: %d\n", x->integerVal);
        if(tree->left->lineNum != 0)
        fprintf(stdout, "Line No: %d\n", tree->left->lineNum);
    }
    lexeme * evaluatedVal = eval(tree->right->left, env);
    x->array[index] = evaluatedVal;
    return tree;
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

int checkForOparantConsistency( lexeme *left, lexeme *right)
{
    if(  left->type == right->type && left->type == INTEGER)
    {
        return 1;
    }
    if (left->type == right->type && left->type == STRING)
    {
        return 2;
    }

    fprintf(stdout, "SIMANTIC ERROR: Trying to compare 2 different types:\n");
    if(left->lineNum != 0)
    fprintf(stdout, "Look at Line No: %d\n", left->lineNum);
    fprintf(stdout, "OR Look at Line No: %dn", right->lineNum);
    exit(1);
    return 0;
}

lexeme* evalGreaterThan( lexeme* tree, lexeme* env, lexeme* eleftOparant)
{
    lexeme* eRightOparant = eval(tree->left, env);
   int stringOrInt =  checkForOparantConsistency(eleftOparant, eRightOparant);
    // 1 for Integer
    // 2 for String
  if( stringOrInt == 1)
  {

    lexeme* result =  newLex(INTEGER);
    if( eleftOparant->integerVal > eRightOparant->integerVal)
    {
         result->integerVal = 1;
    }
    else
    result->integerVal = 0;
    result->lineNum = eRightOparant->lineNum;
    if(tree->right != NULL)
    {
        if(tree->right->type == AND)
            {
                if (result->integerVal ==  0)
                {
                    return result;
                }
                else
                {
                   return evaluateCond(tree->right, env);
                }
            }
        else
        if(tree->right->type == OR)
        {
            if(result->integerVal == 1)
                return result;
                else
                {
                   return  evaluateCond(tree->right, env);
                }
        }

       
    }
    return result;
}
else
    {
        
    lexeme* result =  newLex(STRING);
    if( strcmp( eleftOparant->stringVal, eRightOparant->stringVal) > 0)
    {
         result->integerVal = 1;
    }
    else
    result->integerVal = 0;
    result->lineNum = eRightOparant->lineNum;
    if(tree->right != NULL)
    {
        if(tree->right->type == AND)
            {
                if (result->integerVal ==  0)
                {
                    return result;
                }
                else
                {
                   return evaluateCond(tree->right, env);
                }
            }
        else
        if(tree->right->type == OR)
        {
            if(result->integerVal == 1)
                return result;
                else
                {
                   return  evaluateCond(tree->right, env);
                }
        }

       
    }
    return result;
    }
}

lexeme* evalEquals( lexeme* tree, lexeme* env, lexeme* eleftOparant)
{
    lexeme* eRightOparant = eval(tree->left, env);
   int stringOrInt =  checkForOparantConsistency(eleftOparant, eRightOparant);
    // 1 for Integer
    // 2 for String
  if( stringOrInt == 1)
  {

    lexeme* result =  newLex(INTEGER);
    if( eleftOparant->integerVal == eRightOparant->integerVal)
    {
         result->integerVal = 1;
    }
    else
    result->integerVal = 0;
    result->lineNum = eRightOparant->lineNum;
    if(tree->right != NULL)
    {
        if(tree->right->type == AND)
            {
                if (result->integerVal ==  0)
                {
                    return result;
                }
                else
                {
                   return evaluateCond(tree->right, env);
                }
            }
        else
        if(tree->right->type == OR)
        {
            if(result->integerVal == 1)
                return result;
                else
                {
                   return  evaluateCond(tree->right, env);
                }
        }

       
    }
    return result;
}
else
    {
        
    lexeme* result =  newLex(STRING);
    if( strcmp( eleftOparant->stringVal, eRightOparant->stringVal) == 0)
    {
         result->integerVal = 1;
    }
    else
    result->integerVal = 0;
    result->lineNum = eRightOparant->lineNum;
    if(tree->right != NULL)
    {
        if(tree->right->type == AND)
            {
                if (result->integerVal ==  0)
                {
                    return result;
                }
                else
                {
                   return evaluateCond(tree->right, env);
                }
            }
        else
        if(tree->right->type == OR)
        {
            if(result->integerVal == 1)
                return result;
                else
                {
                   return  evaluateCond(tree->right, env);
                }
        }

       
    }
    return result;
    }
}

lexeme* evalLessThan( lexeme* tree, lexeme* env, lexeme* eleftOparant)
{
    lexeme* eRightOparant = eval(tree->left, env);
   int stringOrInt =  checkForOparantConsistency(eleftOparant, eRightOparant);
    // 1 for Integer
    // 2 for String
  if( stringOrInt == 1)
  {

    lexeme* result =  newLex(INTEGER);
    if( eleftOparant->integerVal < eRightOparant->integerVal)
    {
         result->integerVal = 1;
    }
    else
    result->integerVal = 0;
    result->lineNum = eRightOparant->lineNum;
    if(tree->right != NULL)
    {
        if(tree->right->type == AND)
            {
                if (result->integerVal ==  0)
                {
                    return result;
                }
                else
                {
                   return evaluateCond(tree->right, env);
                }
            }
        else
        if(tree->right->type == OR)
        {
            if(result->integerVal == 1)
                return result;
                else
                {
                   return  evaluateCond(tree->right, env);
                }
        }

       
    }
    return result;
}
else
    {
        
    lexeme* result =  newLex(STRING);
    if( strcmp( eleftOparant->stringVal, eRightOparant->stringVal) < 0)
    {
         result->integerVal = 1;
    }
    else
    result->integerVal = 0;
    result->lineNum = eRightOparant->lineNum;
    if(tree->right != NULL)
    {
        if(tree->right->type == AND)
            {
                if (result->integerVal ==  0)
                {
                    return result;
                }
                else
                {
                   return evaluateCond(tree->right, env);
                }
            }
        else
        if(tree->right->type == OR)
        {
            if(result->integerVal == 1)
                return result;
                else
                {
                   return  evaluateCond(tree->right, env);
                }
        }

       
    }
    return result;
    }
}


lexeme *evalSimpleOp( lexeme* tree, lexeme *env, lexeme* eleftOparant)
{
    
    if(tree->type == PLUS) 
       return evalPlus(tree, env, eleftOparant);
       else
        if(tree->type == MODULUS) 
       return evalModulus(tree, env, eleftOparant);
     else
     if(tree->type == MINUS)
         evalMinus(tree, env, eleftOparant);
    else
    if(tree->type == DIVIDES)
        evalDivides(tree, env, eleftOparant);
    else
    if(tree->type == TIMES)
        evalTimes(tree, env, eleftOparant);
    else
    if(tree->type == GREATERTHAN)
        evalGreaterThan(tree, env, eleftOparant);
   else
    if(tree->type == LESSTHAN)
        evalLessThan(tree, env, eleftOparant);
    else
    if(tree->type == EQUALS)
        evalEquals(tree, env, eleftOparant);


}

lexeme *evalMath(lexeme *tree, lexeme *env)
{
    lexeme *left = tree->left;
    lexeme *right = tree->right;
    lexeme *evaluatedLeft= eval(left, env);
    return evalSimpleOp(right, env, evaluatedLeft);
    // if(left->type == MATH)
    // {
    //     eval(left, env);
    // }   
}

lexeme* evaluateCond( lexeme* tree, lexeme* env)
{
    lexeme *left = tree->left;
    lexeme *right = tree->right;
    lexeme *evaluatedLeft= eval(left, env);
    return evalSimpleOp(right, env, evaluatedLeft);

    //lexeme *right = eval(tree->right->left, env);
}

lexeme * evalIf(lexeme* tree, lexeme *env)
{
    lexeme *conditional = tree->left;
    lexeme *elseBody = tree->right->right;
    lexeme *body = tree->right->left;
    
    lexeme* trueFlase = evaluateCond( conditional, env);
    if(trueFlase->integerVal == 0)
    {
        //False
        if(tree->right->right !=NULL)
       return eval(tree->right->right, env);
       else
       return tree;
    }
    else
    //TRUE
    return eval(tree->right->left, env);
        
}


lexeme * evalWhile(lexeme* tree, lexeme* env )
{
    lexeme *conditional = tree->left;

    lexeme *body = tree->right;
    
    lexeme* trueFlase = evaluateCond( conditional, env);
    while(trueFlase->integerVal == 1)
    {
        eval(body, env);
        trueFlase = evaluateCond(conditional, env);
    }
    return tree;
}


lexeme *evalDisplay(lexeme *tree, lexeme *env)
{
    
    if(tree->left->type == INTEGER)
    {
        fprintf(stdout, "%d", tree->left->integerVal);
    }
    else
    if(tree->left->type == STRING)
    {
         fprintf(stdout,"%s", tree->left->stringVal);
        }
    else
    if(tree->left->type == VARIABLE || tree->left->type == CALLARRAY)
    {
        lexeme *value = eval(tree->left, env);
       
        if (value == NULL)
        {
            fprintf(stdout, "VAlue was null");
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
            else
            {
                 fprintf(stdout, "Got : \n");
                 printTree(stdout, value, 1);
                 Display(stdout, value );
            }
            
        }
    }
    else
    {
        fprintf(stdout, "Got : ");
        Display(stdout, tree->left );
    }   
    return tree;
}





lexeme* eval (lexeme* tree, lexeme* env)
{
    switch(tree->type)
    {
        case NEXT:
            while(tree->right!=NULL)
            {
                eval(tree->left, env);
                tree= tree->right;
            }
            return eval(tree->left, env);
          

            // eval(tree->left, env);
            // if(tree->right != NULL)
            // {
            //     tree = tree->right;
            //     eval(tree, env);
            // }
            // return NULL;
        case MATH:
            return evalMath(tree, env);

        case INTEGER:
            return tree;
        case STRING:
            return tree;
        case SET:
            return evalSet(tree, env);
        case LAMBDA:
            //return extend(tree->left, NULL, env);+
            tree->definingEnv = env;
            return tree;
        case IF:
            return evalIf(tree, env);
        case DEFARRAY:
            return evalDefArray(tree, env);
        case DEFINE:
            return evalVarDefine(tree, env);
        case DISPLAY:
            return evalDisplay(tree, env);
        case CALL:
            return evalCall(tree, env);
        case CALLARRAY:
            return evalCallArray(tree, env);
        case SETARRAY:
            return evalSetArray(tree, env);
        case VARIABLE:
            return lookup(tree, env);
        case WHILE:
            return evalWhile(tree, env);
        
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
    //printTree(stdout, env, 2);
     

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

    // lexeme *token4 = malloc(sizeof(lexeme));
    // token4->type = INTEGER;
    // token4->integerVal = 844;


    // lexeme *env  = createEnv();
    // insert(var, token, env);
    // insert(var2, token2, env);
    // insert(var3, token3, env);

    // printTree(stdout, env , 2);

    // update(var2, token4, env);
    // printTree(stdout, env , 2);

    // // token2->integerVal = 894;

    // //  update(var2, token2, env);
    // // printTree(stdout, env , 2);
    // // Display( stdout,  lookup(var2, env));

    //  lexeme *var4 = malloc(sizeof(lexeme));
    //  var4->type = VARIABLE;
    //  var4->name = "Var4";


    // //  lexeme *value = lookup(var4, env);
   
    

    fclose(fp);
    fprintf (stdout, "YEAHHH!\n");
    return 0;
}