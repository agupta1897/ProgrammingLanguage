#include"scanner.h"
#include"lex.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<strings.h>

lexeme* currentLex;
FILE *fp;
int lineNum = 1;

 void skipWhiteSpace(FILE *fp)
{
int ch;

/* read chars until a non-whitespace character is encountered */

while ((ch = fgetc(fp)) != EOF && (isspace(ch) || ch == '~'))
{
    if (ch == '\n')
    lineNum++;
    if (ch == '~')
    {
    readLine(fp);
    lineNum++;
    }
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
    if( strcasecmp(keyword, "def") == 0) token->type = DEFINE; else
    if( strcasecmp(keyword, "set") == 0) token->type = SET; else
    if( strcasecmp(keyword, "call") == 0) token->type = CALL; else
    if( strcasecmp(keyword, "lambda") == 0) token->type = LAMBDA; else
    if( strcasecmp(keyword, "if") == 0) token->type = IF; else
    if( strcasecmp(keyword, "else") == 0) token->type = ELSE; else
    if( strcasecmp(keyword, "or") == 0) token->type = OR; else
    if( strcasecmp(keyword, "and") == 0) token->type = AND; else
    if( strcasecmp(keyword, "equals") == 0) token->type = EQUALS; else
    if( strcasecmp(keyword, "while") == 0) token->type = WHILE; else
    if( strcasecmp(keyword, "defarray") == 0) token->type = DEFARRAY; else
    if( strcasecmp(keyword, "callarray") == 0) token->type = CALLARRAY; else
    if( strcasecmp(keyword, "display") == 0) token->type = DISPLAY; else
    if( strcasecmp(keyword, "setarray") == 0) token->type = SETARRAY; else
    if( strcasecmp (keyword, "null") == 0) token->type = Null;
     else
    {
        token->name =  keyword;
        token->type = VARIABLE;
    }
    token->lineNum = lineNum;
return token;
}



lexeme* lex(FILE *file)
{
    fp = file;
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
    case '%':
        return newLex(MODULUS);
    case ';':
        return newLex(SEMICOLON);
    case '=':
        return newLex(ASSIGN);
    case '<':
        return newLex(LESSTHAN);
    case '>':
        return newLex(GREATERTHAN);
    case '[':
        return newLex(OSBRACKET);
    case ']':
        return newLex(CSBRACKET);
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
            ungetc(ch,fp);
            skipWhiteSpace(fp);
            lex(fp);

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
        case MODULUS:
            s = "Modulus";
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
            s = "ASSIGN: =";
            break;
        case OSBRACKET:
             s = "SQUARE BRACKET - [";
            break;
        case CSBRACKET:
             s = "SQUARE BRACKET - ]";
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
    else if (l->type == LAMBDA)
        fprintf(out, "LAMBDA\n" );
    else if (l->type == IF)
        fprintf(out, "IF\n" );
    else if (l->type == ELSE)
        fprintf(out, "ELSE\n" );
    else if (l->type == AND)
        fprintf(out, "AND\n" );
    else if (l->type == OR)
        fprintf(out, "OR\n" );
    else if (l->type == EQUALS)
        fprintf(out, "EQUALS\n" );     
    else if (l->type == WHILE)
        fprintf(out, "WHILE\n" );
    else if (l->type == LESSTHAN)
        fprintf(out, "LESSTHAN\n" );
    else if (l->type == GREATERTHAN)
        fprintf(out, "GREATERTHAN\n" ); 
    else if (l->type == DEFARRAY)
        fprintf(out, "DEFARRAY\n" );       
    else if (l->type == PROGRAM)
        fprintf(out, "PROGRAM\n" );    
    else if (l->type == NEXT)
        fprintf(out, "NEXT\n" );
    else if (l->type == MATH)
        fprintf(out, "MATHEXP\n" );
    else if (l->type == CALLARRAY)
        fprintf(out, "CALL ARRAY\n" );
    else if (l->type == SETARRAY)
        fprintf(out, "SET ARRAY\n" );
    else if (l->type == COND)
        fprintf(out, "CONDITIONAL\n" );
    else if (l->type == THEN)
        fprintf(out, "THEN\n" );
    else if (l->type == DISPLAY)
        fprintf(out, "DISPLAY\n" );
    else if (l->type == ENV)
        fprintf(out, "Environment\n" );
    else if (l->type == TABLE)
        fprintf(out, "Table\n" );
    else if(l->type == ARG)
        fprintf(out, "ARG1\n" );
    else       
    fprintf(out, "%s \n", s);
}



