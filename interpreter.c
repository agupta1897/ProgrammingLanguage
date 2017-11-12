#include"scanner.h"
#include"lex.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main (int argc, char** argv)
{
    FILE *fp = fopen(argv[1],"r");
    (void) recognize(fp);
    // Display(stdout, l);
    // while (l->type != END_OF_INPUT)
    // {
    //     l = lex(fp);
    //     Display (stdout, l);        
    // }
    fclose(fp);
    fprintf (stdout, "SUCKER!\n");
    return 0;
}