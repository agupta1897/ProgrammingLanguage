#include"scanner.h"
#include"lex.h"
#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include<string.h>
#include<strings.h>


lexeme* currentLex;
FILE *fp;
int lineNum = 1;

 void skipWhiteSpace(FILE *fp)
{
int ch;

/* read chars until a non-whitespace character is encountered */

while ((ch = fgetc(fp)) != EOF && isspace(ch))
{
    if (ch == '\n')
    lineNum++;
   // fprintf(stdout, "SKIPPING:%c\n", ch);
}

/* a non-space character got us out of the loop, so push it back */

if (ch != EOF) ungetc(ch,fp);
}


lexeme* newCommentLex ( FILE* fp)
{
    lexeme *token = malloc(sizeof(lexeme));
    if (token == 0)
    {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    token->stringVal =  readLine(fp);
    lineNum++;
    token->type = COMMENT;
    token->lineNum = lineNum;
return token;

}

lexeme* newLex (types t )
{
    //fprintf(stdout,"got: %d\n", t);
    lexeme *token = malloc(sizeof(lexeme));
    if (token == 0)
    {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    token->type = t;
    token->lineNum = lineNum;
   // Display(stdout, token);
   // fprintf(stdout,"-------\n");
    return token;
}

lexeme* newIntLex ( FILE* fp)
{
    lexeme *token = malloc(sizeof(lexeme));
    if (token == 0)
    {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    int value = 0;
    fscanf(fp,"%d",&value);
    token->integerVal = value;
    token->type = INTEGER;
    token->lineNum = lineNum;
return token;

}

lexeme* newStringLex ( FILE* fp)
{
    lexeme *token = malloc(sizeof(lexeme));
    if (token == 0)
    {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    token->stringVal =  readString(fp, lineNum);
    token->type = STRING;
    token->lineNum = lineNum;
return token;

}

lexeme* newVariableLex ( FILE* fp)
{
    lexeme *token = malloc(sizeof(lexeme));
    if (token == 0)
    {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    char *keyword = readToken(fp);
    if( strcasecmp(keyword, "define") == 0) token->type = DEFINE; else
    if( strcasecmp(keyword, "set") == 0) token->type = SET; else
    if( strcasecmp(keyword, "call") == 0) token->type = CALL; else
    if( strcasecmp (keyword, "null") == 0) token->type = Null;
     else
    {
        token->name =  keyword;
        token->type = VARIABLE;
    }
    token->lineNum = lineNum;
return token;
}



lexeme* lex()
{
    skipWhiteSpace (fp);
    char ch;
    ch = fgetc(fp);
    //fprintf(stdout, "%c\n", ch);
    if (ch == EOF)
    {
        return newLex(END_OF_INPUT);
    }

    switch (ch)
    {
    case '(':
        return newLex(OPAREN);
    case ')':
        return newLex(CPAREN);
    case ',':
        return newLex(COMMA);
    case '+':
        return newLex(PLUS);
    case '*':
        return newLex(TIMES);
    case '-':
        return newLex(MINUS);
    case '/': 
        return newLex(DIVIDES);
    case '{':
        return newLex(OBRACKET);
    case '}':
        return newLex(CBRACKET);
    case ';':
        return newLex(SEMICOLON);
     case '=':
        return newLex(ASSIGN);
    default:
        if(isdigit(ch))
        {
         ungetc(ch,fp);
         return newIntLex(fp);
        }
        else
        if( ch == '\"')
        {
            ungetc(ch,fp);
            return newStringLex(fp);
        }
        else
        if (isalpha(ch))
        {
            ungetc(ch, fp);
            return newVariableLex(fp);
        }
        else
        if( ch=='~')
        {
            return newCommentLex(fp);
        }
        return newLex(UNKNOWN);
    }
}


void Display ( FILE *out, lexeme *l)
{
    char *s;
    switch(l->type)
    {
        case OPAREN:
            s = "Open Parenthesis";
            break;
        case CPAREN:
            s = "Close Parenthesis";
            break;
        case COMMA:
            s = "Comma";
            break;
        case PLUS:
            s = "Plus";
            break;
        case TIMES:
            s = "Times";
            break;
        case MINUS:
            s = "Minus";
            break;
        case DIVIDES:
            s = "Divides";
            break;
        case OBRACKET:
            s = "Open Bracket";
            break;
        case CBRACKET:
            s = "Close Bracket";
            break;
        case SEMICOLON:
            s = "Semi Colon";
            break;
        case TILDE:
            s = "Tilde";
            break;
        case END_OF_INPUT:
            s = "ENDOFINPUT";
            break;
        case ASSIGN:
            s = "ASSIGN";
            break;
        default:
            s = "UNKNOWN";
            break;
    }
    if (l->type == STRING)
        fprintf(out, "STRING: %s\n", l->stringVal);
    else if (l->type == INTEGER)
        fprintf(out, "INTEGER: %d\n", l->integerVal);
    else if (l->type == VARIABLE)
        fprintf(out, "VARIABLE NAME: %s\n", l->name);  
    else if (l->type == COMMENT)
        fprintf(out, "COMMENT VALUE: %s\n", l->stringVal);
    else if (l->type == DEFINE)
        fprintf(out, "DEFINE\n");
    else if (l->type == SET)
        fprintf(out, "SET\n" );
    else if (l->type == CALL)
        fprintf(out, "CALL\n" );
    else
    fprintf(out, "%s \n", s);
}

int check( types type) 
        { 
        return currentLex->type == type; 
        }
void advance() 
        { 
        currentLex = lex(); 
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

int assignPending()
{
    return check(SET);
}
int callPending()
{
    return check(CALL);
}
int mathExpPending()
{
    return check(OPAREN);
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

void mathExp();

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

    if(mathExpPending()) mathExp();else
    if(integerPending()) match(INTEGER);else
    match(VARIABLE);

    if (cparenPending()) match(CPAREN);
    else
    restofExp();

}

void mathExp()
{
    match(OPAREN);
    if(mathExpPending()) mathExp(); else
    if (integerPending()) match(INTEGER);
    else
    match(VARIABLE);
    
    if (cparenPending()) match(CPAREN);
    else
    restofExp();
    
}


void definition ()
{
    match(DEFINE);
    match(VARIABLE);
    match(ASSIGN);
    value();
}     

// void call()
// {
//     match(CALL);
//     match(OPAREN);
//     if()
// }

void assign()
{
    match(SET);
    match(VARIABLE);
    match(ASSIGN);
    //if(callPending()) call();
    if(mathExpPending()) mathExp();
    else
     if(stringPending()) match(STRING);
    else
     match(INTEGER);
   
}

void statement()
{
    if(definitionPending()) definition();
    else
    if(assignPending()) assign();
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
    currentLex = lex();
    program(fp);
}

