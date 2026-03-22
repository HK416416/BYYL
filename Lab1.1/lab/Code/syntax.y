%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

extern int yylex();
void yyerror(const char* s);
extern int lexical_error;
int syntax_error = 0;
int last_error_line = -1;
Node* root = NULL;
extern int unput(int c);   /* 声明 unput，来自 Flex 生成的代码 */
%}

%locations
%define parse.error verbose

%union {
    int ival;
    float fval;
    char* sval;
    struct Node* node;
}

/* 终结符 */
%token <node> INT FLOAT ID TYPE STRUCT RETURN IF ELSE WHILE
%token <node> PLUS MINUS STAR DIV ASSIGNOP AND OR NOT
%token <node> LT LE GT GE EQ NE
%token <node> LP RP LC RC LB RB SEMI COMMA DOT

/* 非终结符 */
%type <node> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag
%type <node> VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args

/* 优先级和结合性 */
%right ASSIGNOP
%left OR
%left AND
%left LT LE GT GE EQ NE
%left PLUS MINUS
%left STAR DIV
%right NOT UMINUS
%left LP LB DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

Program: ExtDefList { root = $1; $$ = $1; }

ExtDefList: ExtDef ExtDefList { $$ = new_node("ExtDefList", @$.first_line, 2, $1, $2); }
          | /* empty */ { $$ = NULL; }

ExtDef: Specifier ExtDecList SEMI { $$ = new_node("ExtDef", @$.first_line, 3, $1, $2, $3); }
      | Specifier FunDec CompSt { $$ = new_node("ExtDef", @$.first_line, 3, $1, $2, $3); }

ExtDecList: VarDec { $$ = new_node("ExtDecList", @$.first_line, 1, $1); }
          | VarDec COMMA ExtDecList { $$ = new_node("ExtDecList", @$.first_line, 3, $1, $2, $3); }

Specifier: TYPE { $$ = $1; }
         | StructSpecifier { $$ = $1; }

StructSpecifier: STRUCT OptTag LC DefList RC { $$ = new_node("StructSpecifier", @$.first_line, 5, $1, $2, $3, $4, $5); }
               | STRUCT Tag { $$ = new_node("StructSpecifier", @$.first_line, 2, $1, $2); }

OptTag: ID { $$ = $1; }
      | /* empty */ { $$ = NULL; }

Tag: ID { $$ = $1; }

VarDec: ID { $$ = new_node("VarDec", @$.first_line, 1, $1); }
      | VarDec LB INT RB { $$ = new_node("VarDec", @$.first_line, 4, $1, $2, $3, $4); }

FunDec: ID LP VarList RP { $$ = new_node("FunDec", @$.first_line, 4, $1, $2, $3, $4); }
      | ID LP RP { $$ = new_node("FunDec", @$.first_line, 3, $1, $2, $3); }

VarList: ParamDec COMMA VarList { $$ = new_node("VarList", @$.first_line, 3, $1, $2, $3); }
       | ParamDec { $$ = new_node("VarList", @$.first_line, 1, $1); }

ParamDec: Specifier VarDec { $$ = new_node("ParamDec", @$.first_line, 2, $1, $2); }

CompSt: LC DefList StmtList RC { $$ = new_node("CompSt", @$.first_line, 4, $1, $2, $3, $4); }

StmtList: Stmt StmtList { $$ = new_node("StmtList", @$.first_line, 2, $1, $2); }
        | /* empty */ { $$ = NULL; }

Stmt: Exp SEMI { $$ = new_node("Stmt", @$.first_line, 2, $1, $2); }
    | CompSt { $$ = $1; }
    | RETURN Exp SEMI { $$ = new_node("Stmt", @$.first_line, 3, $1, $2, $3); }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { $$ = new_node("Stmt", @$.first_line, 5, $1, $2, $3, $4, $5); }
    | IF LP Exp RP Stmt ELSE Stmt { $$ = new_node("Stmt", @$.first_line, 7, $1, $2, $3, $4, $5, $6, $7); }
    | WHILE LP Exp RP Stmt { $$ = new_node("Stmt", @$.first_line, 5, $1, $2, $3, $4, $5); }
    | Exp error {
        if (last_error_line != yylloc.first_line) {
            syntax_error = 1;
            fprintf(stderr, "Error type B at Line %d: Missing \";\".\n", yylloc.first_line);
            last_error_line = yylloc.first_line;
        }
        yyclearin;
        yyerrok;
        $$ = NULL;
    }

DefList: Def DefList { $$ = new_node("DefList", @$.first_line, 2, $1, $2); }
       | /* empty */ { $$ = NULL; }

Def: Specifier DecList SEMI { $$ = new_node("Def", @$.first_line, 3, $1, $2, $3); }

DecList: Dec { $$ = new_node("DecList", @$.first_line, 1, $1); }
       | Dec COMMA DecList { $$ = new_node("DecList", @$.first_line, 3, $1, $2, $3); }

Dec: VarDec { $$ = new_node("Dec", @$.first_line, 1, $1); }
    | VarDec ASSIGNOP Exp { $$ = new_node("Dec", @$.first_line, 3, $1, $2, $3); }

Exp: Exp ASSIGNOP Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | Exp AND Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | Exp OR Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | Exp LT Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | Exp LE Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | Exp GT Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | Exp GE Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | Exp EQ Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | Exp NE Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | Exp PLUS Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | Exp MINUS Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | Exp STAR Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | Exp DIV Exp { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | LP Exp RP { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }
   | MINUS Exp %prec UMINUS { $$ = new_node("Exp", @$.first_line, 2, $1, $2); }
   | NOT Exp { $$ = new_node("Exp", @$.first_line, 2, $1, $2); }
   | ID LP Args RP { $$ = new_node("Exp", @$.first_line, 4, $1, $2, $3, $4); }
   | ID { $$ = new_node("Exp", @$.first_line, 1, $1); }
   | INT { $$ = new_node("Exp", @$.first_line, 1, $1); }
   | FLOAT { $$ = new_node("Exp", @$.first_line, 1, $1); }
   | Exp LB Exp RB { $$ = new_node("Exp", @$.first_line, 4, $1, $2, $3, $4); }
   | Exp LB Exp error {
        if (last_error_line != yylloc.first_line) {
            syntax_error = 1;
            fprintf(stderr, "Error type B at Line %d: Missing \"]\".\n", yylloc.first_line);
            last_error_line = yylloc.first_line;
        }
        yyclearin;
        int t;
        do {
            t = yylex();
        } while (t != SEMI && t != RC && t != 0);
        yyerrok;
        $$ = new_node("ERROR_EXP", yylloc.first_line, 0);
    }
   | Exp DOT ID { $$ = new_node("Exp", @$.first_line, 3, $1, $2, $3); }


Args: Exp COMMA Args { $$ = new_node("Args", @$.first_line, 3, $1, $2, $3); }
    | Exp { $$ = new_node("Args", @$.first_line, 1, $1); }

%%

void yyerror(const char* s) {
    syntax_error = 1;
    /* 不输出任何内容，由具体错误规则处理 */
}

#include "lex.yy.c"