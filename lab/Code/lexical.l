%option yylineno
%{
#include "syntax.tab.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yycolumn = 1;
#define YY_USER_ACTION \
    yylloc.first_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_line = yylineno; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

int lexical_error = 0;
%}

digit       [0-9]
letter      [a-zA-Z_]
hex         [0-9a-fA-F]
octal       [0-7]

%x COMMENT

%%

"//".*               { /* 行注释，忽略 */ }
"/*"                 { BEGIN(COMMENT); }
<COMMENT>"*/"        { BEGIN(INITIAL); }
<COMMENT>.|\n        { /* 忽略 */ }

"int"                { yylval.node = new_type("int", yylineno); return TYPE; }
"float"              { yylval.node = new_type("float", yylineno); return TYPE; }
"struct"             { yylval.node = new_leaf("STRUCT", yylineno); return STRUCT; }
"return"             { yylval.node = new_leaf("RETURN", yylineno); return RETURN; }
"if"                 { yylval.node = new_leaf("IF", yylineno); return IF; }
"else"               { yylval.node = new_leaf("ELSE", yylineno); return ELSE; }
"while"              { yylval.node = new_leaf("WHILE", yylineno); return WHILE; }





0[0-7]+              {
    int val = 0;
    sscanf(yytext, "%o", &val);
    yylval.node = new_int(val, yylineno);
    return INT;
}



0[xX][0-9a-fA-F]+    {
    int val = 0;
    sscanf(yytext, "%x", &val);
    yylval.node = new_int(val, yylineno);
    return INT;
}


{digit}+"."{digit}*    {
    float val = atof(yytext);
    yylval.node = new_float(val, yylineno);
    return FLOAT;
}
{digit}*"."{digit}+    {
    float val = atof(yytext);
    yylval.node = new_float(val, yylineno);
    return FLOAT;
}


0|[1-9][0-9]*        {
    int val = atoi(yytext);
    yylval.node = new_int(val, yylineno);
    return INT;
}

0[xX][0-9a-fA-F]*[a-zA-Z_][0-9a-fA-F_]* {
    lexical_error = 1;
    fprintf(stderr, "Error type A at Line %d: Illegal hexadecimal number '%s'.\n", yylineno, yytext);
    yylval.node = new_int(0, yylineno);
    return INT;
}

0[0-7]*[89][0-9]*    {
    lexical_error = 1;
    fprintf(stderr, "Error type A at Line %d: Illegal octal number '%s'.\n", yylineno, yytext);
    yylval.node = new_int(0, yylineno);
    return INT;
}


{letter}({letter}|{digit})* {
    yylval.node = new_id(yytext, yylineno);
    return ID;
}


"+"                  { yylval.node = new_leaf("PLUS", yylineno); return PLUS; }
"-"                  { yylval.node = new_leaf("MINUS", yylineno); return MINUS; }
"*"                  { yylval.node = new_leaf("STAR", yylineno); return STAR; }
"/"                  { yylval.node = new_leaf("DIV", yylineno); return DIV; }
"="                  { yylval.node = new_leaf("ASSIGNOP", yylineno); return ASSIGNOP; }
"=="                 { yylval.node = new_leaf("EQ", yylineno); return EQ; }
"!="                 { yylval.node = new_leaf("NE", yylineno); return NE; }
"<"                  { yylval.node = new_leaf("LT", yylineno); return LT; }
"<="                 { yylval.node = new_leaf("LE", yylineno); return LE; }
">"                  { yylval.node = new_leaf("GT", yylineno); return GT; }
">="                 { yylval.node = new_leaf("GE", yylineno); return GE; }
"&&"                 { yylval.node = new_leaf("AND", yylineno); return AND; }
"||"                 { yylval.node = new_leaf("OR", yylineno); return OR; }
"!"                  { yylval.node = new_leaf("NOT", yylineno); return NOT; }
";"                  { yylval.node = new_leaf("SEMI", yylineno); return SEMI; }
","                  { yylval.node = new_leaf("COMMA", yylineno); return COMMA; }
"."                  { yylval.node = new_leaf("DOT", yylineno); return DOT; }
"("                  { yylval.node = new_leaf("LP", yylineno); return LP; }
")"                  { yylval.node = new_leaf("RP", yylineno); return RP; }
"["                  { yylval.node = new_leaf("LB", yylineno); return LB; }
"]"                  { yylval.node = new_leaf("RB", yylineno); return RB; }
"{"                  { yylval.node = new_leaf("LC", yylineno); return LC; }
"}"                  { yylval.node = new_leaf("RC", yylineno); return RC; }


[ \t\r]+             { /* 忽略 */ }
\n                   { yycolumn = 1; }


.                    {
    lexical_error = 1;
    fprintf(stderr, "Error type A at Line %d: Mysterious character '%s'.\n", yylineno, yytext);
}

%%