/* 1- Definiciones -*/
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
#include "lib/util.c"

int yyerrorl();

%}

/* 2 - REGEX */
%option noyywrap
%option yylineno

WRITE WRITE
READ READ
POSICION posicion

DIGITO  [0-9]
LETRA   [a-zA-Z]
CTE     {DIGITO}+
ID      {LETRA}+({LETRA}|{DIGITO})*
ASIGNA  "="
PARA    "("
PARC    ")"
CA      "["
CC      "]"
COMA    ","
PYC     ";"
CTE_S   \"(\\.|[^"\\])*\"

/* 3 - Reglas */
%%
{WRITE}     {return WRITE;};
{READ}      {return READ;};
{POSICION}  {return POSICION;};
{CTE}       {   
                yylval.tipo_int = atoi(yytext);
                insertarEnTS(yytext, "CTE", "", yylval.tipo_int, 0);
                return CTE;
            };
{ID}        {
                yylval.tipo_string = yytext;
                insertarEnTS(yytext,"ID","",0,0);
                return ID;
            };
{ASIGNA}    {return ASIGNA;};
{PARA}      {return PARA;};
{PARC}      {return PARC;};
{CA}        {return CA;};
{CC}        {return CC;};
{COMA}      {return COMA;};
{PYC}       {return PYC;};
{CTE_S}     {
                yylval.tipo_string = yytext;
                char aux[100];
                strcpy(aux, "_");
                strcat(aux,yylval.tipo_string);
                removeChar(aux,'\"');
                removeChar(aux,':');
                replace_char(aux,' ', '_');
                insertarEnTS(aux, "CTE_STRING", yylval.tipo_string, 0, 0);
                strcpy(yylval.tipo_string, aux);
                return CTE_S;
            };

"\n"
"\t"
"\n\t"
"\r\t"
" "
.       yyerrorl();
%%

/* 4 - Código */
int yyerrorl(void) {
    printf("Error léxico: Caracter invalido: %s", yytext);
    exit(1);
}
