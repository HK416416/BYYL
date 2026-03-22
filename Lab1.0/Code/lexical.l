%option yylineno
%option noyywrap
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax.tab.h"

// 列号维护
int yycolumn = 1;
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

// 错误声明
extern void print_error(char type, int line, const char* msg);
%}

/* 正则定义 */
digit           [0-9]
oct_digit       [0-7]
hex_digit       [0-9a-fA-F]
letter          [a-zA-Z_]
whitespace      [ \t\r]
newline         \n

/* 数字定义 */
dec_int         [1-9]{digit}*
oct_int         0{oct_digit}+
hex_int         0[xX]{hex_digit}+
int_num         {dec_int}|{oct_int}|{hex_int}

float_num       ({digit}*\.{digit}+)|({digit}+\.{digit}*)
exp_float       ({float_num}|{digit}+)[eE][+-]?{digit}+

/* 标识符 */
id              {letter}({letter}|{digit})*

/* 运算符优先级：长匹配优先 */
%%
"/*"            {
                    // 块注释处理
                    char c;
                    int prev = 0;
                    int start_line = yylineno;
                    while ((c = input()) != 0) {
                        if (c == '\n') {
                            yylineno++;
                            yycolumn = 1;
                        }
                        if (prev == '*' && c == '/') break;
                        prev = c;
                    }
                    if (c == 0) {
                        print_error('A', start_line, "Unclosed block comment");
                    }
                }

"//"            {
                    // 单行注释处理
                    char c;
                    while ((c = input()) != '\n' && c != 0);
                    if (c == '\n') {
                        yylineno++;
                        yycolumn = 1;
                    }
                }

"int"|"float"   { yylval.str_val = strdup(yytext); return TYPE; }
"struct"        { return STRUCT; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"return"        { return RETURN; }
"void"          { return VOID; }

"<="|">="|"=="|"!="|"<"|">"  { yylval.str_val = strdup(yytext); return RELOP; }
"="             { return ASSIGNOP; }
"&&"            { return AND; }
"||"            { return OR; }
"!"             { return NOT; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return STAR; }
"/"             { return DIV; }
"("             { return LP; }
")"             { return RP; }
"{"             { return LC; }
"}"             { return RC; }
"["             { return LB; }
"]"             { return RB; }
";"             { return SEMI; }
","             { return COMMA; }
"."             { return DOT; }

{int_num}       {
                    // 整数解析：十进制/八进制/十六进制
                    char* endptr;
                    if (yytext[0] == '0' && (yytext[1] == 'x' || yytext[1] == 'X')) {
                        // 十六进制
                        yylval.int_val = strtol(yytext, &endptr, 16);
                        if (*endptr != '\0') {
                            char msg[128];
                            snprintf(msg, sizeof(msg), "Illegal hexadecimal number '%s'", yytext);
                            print_error('A', yylineno, msg);
                        }
                    } else if (yytext[0] == '0' && yytext[1] != '\0') {
                        // 八进制
                        yylval.int_val = strtol(yytext, &endptr, 8);
                        if (*endptr != '\0') {
                            char msg[128];
                            snprintf(msg, sizeof(msg), "Illegal octal number '%s'", yytext);
                            print_error('A', yylineno, msg);
                        }
                    } else {
                        // 十进制
                        yylval.int_val = atoi(yytext);
                    }
                    return INT;
                }

{float_num}|{exp_float} {
                    // 浮点数解析（含指数形式）
                    char* endptr;
                    yylval.float_val = strtof(yytext, &endptr);
                    if (*endptr != '\0') {
                        char msg[128];
                        snprintf(msg, sizeof(msg), "Illegal floating point number '%s'", yytext);
                        print_error('A', yylineno, msg);
                    }
                    return FLOAT;
                }

{id}            { yylval.str_val = strdup(yytext); return ID; }

{whitespace}    { /* 忽略空白符 */ }

{newline}       { yycolumn = 1; /* 换行重置列号 */ }

.               {
                    // 未定义字符，错误类型A
                    char msg[128];
                    snprintf(msg, sizeof(msg), "Mysterious character '%s'", yytext);
                    print_error('A', yylineno, msg);
                }
%%