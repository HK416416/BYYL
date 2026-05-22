%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* Forward declarations: actual AST types/implementations live in main.c */
/* Define Node type so actions can access node fields directly. This duplicates
   the definition in main.c but keeps the grammar self-contained. */
typedef struct Node {
    char* name;
    int line;
    int is_terminal;      /* 1: 终结符, 0: 非终结符 */
    union {
        struct {
            struct Node** children;
            int num_children;
        } nonterm;
        struct {
            char* value;   /* ID, STRING */
            int int_val;
            float float_val;
        } term;
    } u;
} Node;

extern Node* root; /* parser will set this when building AST (defined in main.c) */
extern int has_syntax_error; /* set by yyerror */
extern int has_error; /* set by lexer or parser on any error */

/* strdup may be missing from strict C declarations; declare it to avoid warnings */
extern char* strdup(const char*);

extern int yylineno;
void yyerror(const char* msg);
extern int yylex();

/* Prototypes for AST helpers implemented in main.c */
Node* create_node(const char* name, int line, int num_children, ...);
Node* create_terminal_node(const char* name, int line);
Node* create_id_node(const char* value, int line);
Node* create_int_node(int val, int line);
Node* create_float_node(float val, int line);
Node* create_string_node(const char* value, int line);
void print_tree(Node* node, int depth);
void free_node(Node* node);

%}

%locations
%define parse.error verbose

%union {
    struct Node* node;
}

/* 词法单元 */
%token AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE ELSE ENUM EXTERN FLOAT FOR GOTO IF INT LONG REGISTER RETURN SHORT SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE
%token <node> ID INT_CONST FLOAT_CONST STRING
%token PLUS MINUS STAR DIV MOD LT LE GT GE EQ NE AND OR NOT ASSIGNOP
%token LP RP LB RB LC RC COMMA SEMI DOT

/* 优先级与结合性（从低到高） */
%right ASSIGNOP
%left OR
%left AND
%left EQ NE
%left LT LE GT GE
%left PLUS MINUS
%left STAR DIV MOD
%right NOT
%left LP RP LB RB DOT

/* Resolve dangling-else: prefer shift (attach ELSE to nearest IF) */
%nonassoc LOWER_THAN_ELSE

/* 非终结符类型声明 */
%type <node> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag VarDec FunDec VarList ParamDec DecList Dec DefList Def CompSt StmtList Stmt Exp Args MatchedStmt UnmatchedStmt

%start Program

%%

Program         : ExtDefList                 { $$ = create_node("Program", @$.first_line, 1, $1); root = $$; }
                ;

ExtDefList      : ExtDef ExtDefList          { $$ = create_node("ExtDefList", @$.first_line, 2, $1, $2); }
                | /* empty */                { $$ = NULL; }
                ;

ExtDef          : Specifier ExtDecList SEMI  { $$ = create_node("ExtDef", @$.first_line, 3, $1, $2, create_terminal_node("SEMI", @3.first_line)); }
                | Specifier SEMI             { $$ = create_node("ExtDef", @$.first_line, 2, $1, create_terminal_node("SEMI", @2.first_line)); }
                | Specifier FunDec CompSt    { $$ = create_node("ExtDef", @$.first_line, 3, $1, $2, $3); }
                | Specifier FunDec SEMI      { $$ = create_node("ExtDef", @$.first_line, 3, $1, $2, create_terminal_node("SEMI", @3.first_line)); }
                | error SEMI                 { yyerrok; $$ = NULL; }
                ;

ExtDecList      : VarDec                     { $$ = create_node("ExtDecList", @1.first_line, 1, $1); }
                | VarDec COMMA ExtDecList    { $$ = create_node("ExtDecList", @$.first_line, 3, $1, create_terminal_node("COMMA", @2.first_line), $3); }
                ;

Specifier       : INT                         { Node* t = create_terminal_node("TYPE", @1.first_line); t->u.term.value = strdup("int"); $$ = create_node("Specifier", @1.first_line, 1, t); }
                | FLOAT                       { Node* t = create_terminal_node("TYPE", @1.first_line); t->u.term.value = strdup("float"); $$ = create_node("Specifier", @1.first_line, 1, t); }
                | StructSpecifier            { $$ = create_node("Specifier", @$.first_line, 1, $1); }
                ;

StructSpecifier : STRUCT OptTag LC DefList RC { $$ = create_node("StructSpecifier", @$.first_line, 5, create_terminal_node("STRUCT", @1.first_line), $2, create_terminal_node("LC", @3.first_line), $4, create_terminal_node("RC", @5.first_line)); }
                | STRUCT Tag                 { $$ = create_node("StructSpecifier", @$.first_line, 2, create_terminal_node("STRUCT", @1.first_line), $2); }
                ;

OptTag          : ID                         { $$ = create_node("OptTag", @1.first_line, 1, $1); }
                | /* empty */                { $$ = NULL; }
                ;

Tag             : ID                         { $$ = create_node("Tag", @1.first_line, 1, $1); }
                ;

VarDec          : ID                         { $$ = create_node("VarDec", @1.first_line, 1, $1); }
                | VarDec LB INT_CONST RB     { $$ = create_node("VarDec", @$.first_line, 4, $1, create_terminal_node("LB", @2.first_line), $3, create_terminal_node("RB", @4.first_line)); }
                ;

FunDec          : ID LP VarList RP           { $$ = create_node("FunDec", @$.first_line, 4, $1, create_terminal_node("LP", @2.first_line), $3, create_terminal_node("RP", @4.first_line)); }
                | ID LP RP                   { $$ = create_node("FunDec", @$.first_line, 3, $1, create_terminal_node("LP", @2.first_line), create_terminal_node("RP", @3.first_line)); }
                ;

VarList         : ParamDec COMMA VarList     { $$ = create_node("VarList", @$.first_line, 3, $1, create_terminal_node("COMMA", @2.first_line), $3); }
                | ParamDec                   { $$ = create_node("VarList", @1.first_line, 1, $1); }
                ;

ParamDec        : Specifier VarDec           { $$ = create_node("ParamDec", @$.first_line, 2, $1, $2); }
                ;

CompSt          : LC DefList StmtList RC     { $$ = create_node("CompSt", @$.first_line, 4, create_terminal_node("LC", @1.first_line), $2, $3, create_terminal_node("RC", @4.first_line)); }
                ;

StmtList        : Stmt StmtList              { $$ = create_node("StmtList", @$.first_line, 2, $1, $2); }
                | /* empty */                { $$ = NULL; }
                ;

/* Use matched / unmatched statement grammar to resolve dangling-else unambiguously */
Stmt            : MatchedStmt
                | UnmatchedStmt
                ;

MatchedStmt     : Exp SEMI                   { $$ = create_node("Stmt", @$.first_line, 2, $1, create_terminal_node("SEMI", @2.first_line)); }
                | CompSt                     { $$ = create_node("Stmt", @$.first_line, 1, $1); }
                | RETURN Exp SEMI            { $$ = create_node("Stmt", @$.first_line, 3, create_terminal_node("RETURN", @1.first_line), $2, create_terminal_node("SEMI", @3.first_line)); }
                | WHILE LP Exp RP MatchedStmt       { $$ = create_node("Stmt", @$.first_line, 5, create_terminal_node("WHILE", @1.first_line), create_terminal_node("LP", @2.first_line), $3, create_terminal_node("RP", @4.first_line), $5); }
                | IF LP Exp RP MatchedStmt ELSE MatchedStmt { $$ = create_node("Stmt", @$.first_line, 7, create_terminal_node("IF", @1.first_line), create_terminal_node("LP", @2.first_line), $3, create_terminal_node("RP", @4.first_line), $5, create_terminal_node("ELSE", @6.first_line), $7); }
                | error SEMI                 { yyerrok; $$ = NULL; }
                ;

UnmatchedStmt   : IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { $$ = create_node("Stmt", @$.first_line, 5, create_terminal_node("IF", @1.first_line), create_terminal_node("LP", @2.first_line), $3, create_terminal_node("RP", @4.first_line), $5); }
                | IF LP Exp RP MatchedStmt ELSE UnmatchedStmt { $$ = create_node("Stmt", @$.first_line, 7, create_terminal_node("IF", @1.first_line), create_terminal_node("LP", @2.first_line), $3, create_terminal_node("RP", @4.first_line), $5, create_terminal_node("ELSE", @6.first_line), $7); }
                /* If the then-branch misses the trailing ';', catch it when ELSE appears and report Missing ";" */
                | IF LP Exp RP error ELSE Stmt { has_error = 1; has_syntax_error = 1; printf("Error type B at Line %d: Missing \";\".\n", yylineno); yyerrok; $$ = NULL; }
                ;

DefList         : Def DefList                { $$ = create_node("DefList", @$.first_line, 2, $1, $2); }
                | /* empty */                { $$ = NULL; }
                ;

Def             : Specifier DecList SEMI     { $$ = create_node("Def", @$.first_line, 3, $1, $2, create_terminal_node("SEMI", @3.first_line)); }
                ;

DecList         : Dec                        { $$ = create_node("DecList", @1.first_line, 1, $1); }
                | Dec COMMA DecList          { $$ = create_node("DecList", @$.first_line, 3, $1, create_terminal_node("COMMA", @2.first_line), $3); }
                ;

Dec             : VarDec                     { $$ = create_node("Dec", @$.first_line, 1, $1); }
                | VarDec ASSIGNOP Exp          { $$ = create_node("Dec", @$.first_line, 3, $1, create_terminal_node("ASSIGNOP", @2.first_line), $3); }
                ;

Exp             : Exp ASSIGNOP Exp             { $$ = create_node("Exp", @$.first_line, 3, $1, create_terminal_node("ASSIGNOP", @2.first_line), $3); }
                | Exp AND Exp                { $$ = create_node("Exp", @$.first_line, 3, $1, create_terminal_node("AND", @2.first_line), $3); }
                | Exp OR Exp                 { $$ = create_node("Exp", @$.first_line, 3, $1, create_terminal_node("OR", @2.first_line), $3); }
                | Exp LT Exp                 { Node* op = create_terminal_node("LT", @2.first_line); $$ = create_node("Exp", @$.first_line, 3, $1, op, $3); }
                | Exp LE Exp                 { Node* op = create_terminal_node("LE", @2.first_line); $$ = create_node("Exp", @$.first_line, 3, $1, op, $3); }
                | Exp GT Exp                 { Node* op = create_terminal_node("GT", @2.first_line); $$ = create_node("Exp", @$.first_line, 3, $1, op, $3); }
                | Exp GE Exp                 { Node* op = create_terminal_node("GE", @2.first_line); $$ = create_node("Exp", @$.first_line, 3, $1, op, $3); }
                | Exp EQ Exp                 { Node* op = create_terminal_node("EQ", @2.first_line); $$ = create_node("Exp", @$.first_line, 3, $1, op, $3); }
                | Exp NE Exp                 { Node* op = create_terminal_node("NE", @2.first_line); $$ = create_node("Exp", @$.first_line, 3, $1, op, $3); }
                | Exp PLUS Exp               { $$ = create_node("Exp", @$.first_line, 3, $1, create_terminal_node("PLUS", @2.first_line), $3); }
                | Exp MINUS Exp              { $$ = create_node("Exp", @$.first_line, 3, $1, create_terminal_node("MINUS", @2.first_line), $3); }
                | Exp STAR Exp               { $$ = create_node("Exp", @$.first_line, 3, $1, create_terminal_node("STAR", @2.first_line), $3); }
                | Exp DIV Exp                { $$ = create_node("Exp", @$.first_line, 3, $1, create_terminal_node("DIV", @2.first_line), $3); }
                | LP Exp RP                  { $$ = create_node("Exp", @$.first_line, 3, create_terminal_node("LP", @1.first_line), $2, create_terminal_node("RP", @3.first_line)); }
                | MINUS Exp %prec NOT        { $$ = create_node("Exp", @$.first_line, 2, create_terminal_node("MINUS", @1.first_line), $2); }
                | NOT Exp                    { $$ = create_node("Exp", @$.first_line, 2, create_terminal_node("NOT", @1.first_line), $2); }
                | ID                         { $$ = create_node("Exp", @1.first_line, 1, $1); }
                | INT_CONST                  { $$ = create_node("Exp", @1.first_line, 1, $1); }
                | FLOAT_CONST                { $$ = create_node("Exp", @1.first_line, 1, $1); }
                | STRING                     { $$ = create_node("Exp", @1.first_line, 1, $1); }
                | ID LP Args RP              { $$ = create_node("Exp", @$.first_line, 4, $1, create_terminal_node("LP", @2.first_line), $3, create_terminal_node("RP", @4.first_line)); }
                | ID LP RP                   { $$ = create_node("Exp", @$.first_line, 3, $1, create_terminal_node("LP", @2.first_line), create_terminal_node("RP", @3.first_line)); }
                | Exp LB Exp RB              { $$ = create_node("Exp", @$.first_line, 4, $1, create_terminal_node("LB", @2.first_line), $3, create_terminal_node("RB", @4.first_line)); }
                /* If there's a syntax error inside brackets (e.g. a[5,3]), report missing ']' and recover to the closing ']' */
                | Exp LB error RB           { has_error = 1; has_syntax_error = 1; printf("Error type B at Line %d: Missing \"]\".\n", yylineno); yyerrok; $$ = NULL; }
                | Exp DOT ID                 { $$ = create_node("Exp", @$.first_line, 3, $1, create_terminal_node("DOT", @2.first_line), $3); }
                ;

Args            : Exp COMMA Args             { $$ = create_node("Args", @$.first_line, 3, $1, create_terminal_node("COMMA", @2.first_line), $3); }
                | Exp                        { $$ = create_node("Args", @1.first_line, 1, $1); }
                ;

%%

void yyerror(const char* msg) {
    /* Record and print any bison-reported syntax error. */
    has_error = 1;
    has_syntax_error = 1;
    printf("Error type B at Line %d: %s.\n", yylineno, msg);
}