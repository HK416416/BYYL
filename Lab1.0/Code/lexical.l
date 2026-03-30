%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax.tab.h"
#include "ast.h"

extern int yylineno;
extern char* yytext;
extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yycolumn = 1;

#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

%}

%option yylineno
%option noyywrap

digit       [0-9]
letter      [a-zA-Z_]
id          {letter}({letter}|{digit})*
bad_id_start {digit}+{letter}({letter}|{digit})*
decimal     (0|[1-9]{digit}*)
octal       0[0-7]+
hex         0[xX][0-9a-fA-F]+
invalid_oct 0[89][0-9]*
invalid_hex 0[xX][^0-9a-fA-F]*

%%

"auto"          { return AUTO; }
"break"         { return BREAK; }
"case"          { return CASE; }
"char"          { return CHAR; }
"const"         { return CONST; }
"continue"      { return CONTINUE; }
"default"       { return DEFAULT; }
"do"            { return DO; }
"double"        { return DOUBLE; }
"else"          { return ELSE; }
"enum"          { return ENUM; }
"extern"        { return EXTERN; }
"float"         { return FLOAT; }
"for"           { return FOR; }
"goto"          { return GOTO; }
"if"            { return IF; }
"int"           { return INT; }
"long"          { return LONG; }
"register"      { return REGISTER; }
"return"        { return RETURN; }
"short"         { return SHORT; }
"signed"        { return SIGNED; }
"sizeof"        { return SIZEOF; }
"static"        { return STATIC; }
"struct"        { return STRUCT; }
"switch"        { return SWITCH; }
"typedef"       { return TYPEDEF; }
"union"         { return UNION; }
"unsigned"      { return UNSIGNED; }
"void"          { return VOID; }
"volatile"      { return VOLATILE; }
"while"         { return WHILE; }

{bad_id_start}  {
    /* Report lexical error and convert the token into an ID consisting of the
       suffix after the leading digits so parsing can continue meaningfully. */
    extern int has_error;
    has_error = 1;
    fprintf(stderr, "Error type A at Line %d: Identifier cannot start with digit '%s'.\n", yylineno, yytext);
    /* find first non-digit in yytext */
    const char* s = yytext;
    int i = 0;
    while (s[i] && s[i] >= '0' && s[i] <= '9') i++;
    if (s[i]) {
        char* suffix = strdup(s + i);
        yylval.node = create_id_node(suffix, yylineno);
        free(suffix);
        return ID;
    } else {
        /* no suffix (token is all digits) — fall back to returning INT_CONST */
        yylval.node = create_int_node(atoi(yytext), yylineno);
        return INT_CONST;
    }
}
{decimal}       {
    yylval.node = create_int_node(atoi(yytext), yylineno);
    return INT_CONST;
}
{octal}         {
    int val;
    sscanf(yytext, "%o", &val);
    yylval.node = create_int_node(val, yylineno);
    return INT_CONST;
}
{hex}           {
    int val;
    sscanf(yytext, "%x", &val);
    yylval.node = create_int_node(val, yylineno);
    return INT_CONST;
}
{invalid_oct}   {
    fprintf(stderr, "Error type A at Line %d: Illegal octal number '%s'.\n", yylineno, yytext);
    exit(1);
}
{invalid_hex}   {
    fprintf(stderr, "Error type A at Line %d: Illegal hexadecimal number '%s'.\n", yylineno, yytext);
    exit(1);
}
{digit}+"."{digit}*|{digit}*"."{digit}+ {
    yylval.node = create_float_node(atof(yytext), yylineno);
    return FLOAT_CONST;
}
{id}            {
    yylval.node = create_id_node(yytext, yylineno);
    return ID;
}
\"[^\"]*\"      {
    char* s = strdup(yytext);
    yylval.node = create_string_node(s, yylineno);
    return STRING;
}
"//"[^\n]*      { /* 忽略单行注释 */ }
"/*"            {
    int c;
    while ((c = input()) != 0) {
        if (c == '*' && input() == '/') break;
        if (c == '\n') yylineno++;
    }
    /* 未闭合注释由词法错误处理，此处简化 */
}
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return STAR; }
"/"             { return DIV; }
"%"             { return MOD; }
"<"             { return LT; }
"<="            { return LE; }
">"             { return GT; }
">="            { return GE; }
"=="            { return EQ; }
"!="            { return NE; }
"&&"            { return AND; }
"||"            { return OR; }
"!"             { return NOT; }
"="             { return ASSIGN; }
"("             { return LP; }
")"             { return RP; }
"["             { return LB; }
"]"             { return RB; }
"{"             { return LC; }
"}"             { return RC; }
","             { return COMMA; }
";"             { return SEMI; }
"."             { return DOT; }
[ \t\r]+        { /* 忽略空白 */ }
\n              { yycolumn = 1; }
.               {
    extern int has_error;
    has_error = 1;
    fprintf(stderr, "Error type A at Line %d: Illegal character '%s'.\n", yylineno, yytext);
    /* skip unknown character and continue scanning */
}

%%