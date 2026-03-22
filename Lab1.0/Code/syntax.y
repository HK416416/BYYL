%locations
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// ------------------ 修复：添加Flex提供的外部声明 ------------------
extern int yylex(void);
extern int yylineno;
extern FILE* yyin;
extern void yyrestart(FILE* input_file);
// ------------------------------------------------------------------

// 错误声明
extern void print_error(char type, int line, const char* msg);

// 语法树节点最大子节点数
#define MAX_CHILDREN 32

// 语法树节点类型
typedef enum {
    NODE_NONTERMINAL, // 非终结符
    NODE_TYPE,         // TYPE终结符
    NODE_ID,           // ID终结符
    NODE_INT,          // INT终结符
    NODE_FLOAT,        // FLOAT终结符
    NODE_TOKEN         // 其他终结符
} NodeType;

// 语法树节点结构体
typedef struct Node {
    NodeType type;
    char name[64];     // 节点名称
    int line;          // 行号
    union {
        int int_val;
        float float_val;
        char* str_val;
    } attr;
    int child_num;
    struct Node* children[MAX_CHILDREN];
} Node;

// 全局根节点
Node* root = NULL;

// 创建语法树节点
Node* create_node(NodeType type, const char* name, int line) {
    Node* node = (Node*)malloc(sizeof(Node));
    memset(node, 0, sizeof(Node));
    node->type = type;
    strncpy(node->name, name, sizeof(node->name)-1);
    node->line = line;
    node->child_num = 0;
    return node;
}

// 为节点添加子节点
void add_child(Node* parent, Node* child) {
    if (!parent || !child) return;
    if (parent->child_num >= MAX_CHILDREN) return;
    parent->children[parent->child_num++] = child;
}

// 递归打印语法树（先序遍历，缩进控制）
void print_tree(Node* node, int indent) {
    if (!node) return;

    // 跳过产生ε的非终结符
    if (node->type == NODE_NONTERMINAL && node->child_num == 0) {
        return;
    }

    // 打印缩进
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }

    // 按类型打印节点信息
    switch (node->type) {
        case NODE_NONTERMINAL:
            printf("%s (%d)\n", node->name, node->line);
            break;
        case NODE_TYPE:
            printf("TYPE: %s\n", node->attr.str_val);
            break;
        case NODE_ID:
            printf("ID: %s\n", node->attr.str_val);
            break;
        case NODE_INT:
            printf("INT: %d\n", node->attr.int_val);
            break;
        case NODE_FLOAT:
            printf("FLOAT: %.6f\n", node->attr.float_val);
            break;
        case NODE_TOKEN:
            printf("%s\n", node->name);
            break;
    }

    // 递归打印子节点
    for (int i = 0; i < node->child_num; i++) {
        print_tree(node->children[i], indent + 1);
    }
}

// 释放语法树内存
void free_tree(Node* node) {
    if (!node) return;
    for (int i = 0; i < node->child_num; i++) {
        free_tree(node->children[i]);
    }
    if (node->type == NODE_TYPE || node->type == NODE_ID) {
        free(node->attr.str_val);
    }
    free(node);
}

// yyerror函数，语法错误处理
void yyerror(const char* msg) {
    print_error('B', yylineno, msg);
}
%}

// 联合类型定义
%union {
    struct Node* node_val;
    int int_val;
    float float_val;
    char* str_val;
}

/* 终结符声明 */
%token <str_val> TYPE ID RELOP
%token <int_val> INT
%token <float_val> FLOAT
%token STRUCT IF ELSE WHILE RETURN VOID
%token LP RP LC RC LB RB SEMI COMMA DOT
%token ASSIGNOP AND OR NOT PLUS MINUS STAR DIV

/* 非终结符类型声明，全部为语法树节点 */
%type <node_val> Program ExtDefList ExtDef Specifier StructSpecifier OptTag Tag
%type <node_val> FunDec VarList ParamDec CompSt DefList Def DecList Dec VarDec
%type <node_val> StmtList Stmt Exp Args

/* 运算符优先级与结合性，从低到高 */
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right UMINUS NOT
%left DOT LB RP
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
/* 语法规则：完全匹配样例输出的非终结符结构 */
Program : ExtDefList {
            $$ = create_node(NODE_NONTERMINAL, "Program", @$.first_line);
            add_child($$, $1);
            root = $$;
            // 无错误时打印语法树
            if (yynerrs == 0) {
                print_tree(root, 0);
            }
            free_tree(root);
        }
        ;

ExtDefList : ExtDef ExtDefList {
                $$ = create_node(NODE_NONTERMINAL, "ExtDefList", @$.first_line);
                add_child($$, $1);
                add_child($$, $2);
            }
            | /* ε */ {
                $$ = create_node(NODE_NONTERMINAL, "ExtDefList", @$.first_line);
            }
            ;

ExtDef : Specifier FunDec CompSt {
            $$ = create_node(NODE_NONTERMINAL, "ExtDef", @$.first_line);
            add_child($$, $1);
            add_child($$, $2);
            add_child($$, $3);
        }
        | Specifier SEMI {
            $$ = create_node(NODE_NONTERMINAL, "ExtDef", @$.first_line);
            add_child($$, $1);
            add_child($$, create_node(NODE_TOKEN, "SEMI", @2.first_line));
        }
        | Specifier DecList SEMI {
            $$ = create_node(NODE_NONTERMINAL, "ExtDef", @$.first_line);
            add_child($$, $1);
            add_child($$, $2);
            add_child($$, create_node(NODE_TOKEN, "SEMI", @3.first_line));
        }
        | error SEMI { yyerrok; }
        ;

Specifier : TYPE {
            $$ = create_node(NODE_NONTERMINAL, "Specifier", @$.first_line);
            Node* type_node = create_node(NODE_TYPE, "TYPE", @1.first_line);
            type_node->attr.str_val = $1;
            add_child($$, type_node);
        }
        | StructSpecifier {
            $$ = create_node(NODE_NONTERMINAL, "Specifier", @$.first_line);
            add_child($$, $1);
        }
        ;

StructSpecifier : STRUCT OptTag LC DefList RC {
                    $$ = create_node(NODE_NONTERMINAL, "StructSpecifier", @$.first_line);
                    add_child($$, create_node(NODE_TOKEN, "STRUCT", @1.first_line));
                    add_child($$, $2);
                    add_child($$, create_node(NODE_TOKEN, "LC", @3.first_line));
                    add_child($$, $4);
                    add_child($$, create_node(NODE_TOKEN, "RC", @5.first_line));
                }
                | STRUCT Tag {
                    $$ = create_node(NODE_NONTERMINAL, "StructSpecifier", @$.first_line);
                    add_child($$, create_node(NODE_TOKEN, "STRUCT", @1.first_line));
                    add_child($$, $2);
                }
                ;

OptTag : ID {
            $$ = create_node(NODE_NONTERMINAL, "OptTag", @$.first_line);
            Node* id_node = create_node(NODE_ID, "ID", @1.first_line);
            id_node->attr.str_val = $1;
            add_child($$, id_node);
        }
        | /* ε */ {
            $$ = create_node(NODE_NONTERMINAL, "OptTag", @$.first_line);
        }
        ;

Tag : ID {
        $$ = create_node(NODE_NONTERMINAL, "Tag", @$.first_line);
        Node* id_node = create_node(NODE_ID, "ID", @1.first_line);
        id_node->attr.str_val = $1;
        add_child($$, id_node);
    }
    ;

FunDec : ID LP VarList RP {
            $$ = create_node(NODE_NONTERMINAL, "FunDec", @$.first_line);
            Node* id_node = create_node(NODE_ID, "ID", @1.first_line);
            id_node->attr.str_val = $1;
            add_child($$, id_node);
            add_child($$, create_node(NODE_TOKEN, "LP", @2.first_line));
            add_child($$, $3);
            add_child($$, create_node(NODE_TOKEN, "RP", @4.first_line));
        }
        | ID LP RP {
            $$ = create_node(NODE_NONTERMINAL, "FunDec", @$.first_line);
            Node* id_node = create_node(NODE_ID, "ID", @1.first_line);
            id_node->attr.str_val = $1;
            add_child($$, id_node);
            add_child($$, create_node(NODE_TOKEN, "LP", @2.first_line));
            add_child($$, create_node(NODE_TOKEN, "RP", @3.first_line));
        }
        | error RP { yyerrok; }
        ;

VarList : ParamDec COMMA VarList {
            $$ = create_node(NODE_NONTERMINAL, "VarList", @$.first_line);
            add_child($$, $1);
            add_child($$, create_node(NODE_TOKEN, "COMMA", @2.first_line));
            add_child($$, $3);
        }
        | ParamDec {
            $$ = create_node(NODE_NONTERMINAL, "VarList", @$.first_line);
            add_child($$, $1);
        }
        ;

ParamDec : Specifier VarDec {
            $$ = create_node(NODE_NONTERMINAL, "ParamDec", @$.first_line);
            add_child($$, $1);
            add_child($$, $2);
        }
        ;

CompSt : LC DefList StmtList RC {
            $$ = create_node(NODE_NONTERMINAL, "CompSt", @$.first_line);
            add_child($$, create_node(NODE_TOKEN, "LC", @1.first_line));
            add_child($$, $2);
            add_child($$, $3);
            add_child($$, create_node(NODE_TOKEN, "RC", @4.first_line));
        }
        | error RC { yyerrok; }
        ;

DefList : Def DefList {
            $$ = create_node(NODE_NONTERMINAL, "DefList", @$.first_line);
            add_child($$, $1);
            add_child($$, $2);
        }
        | /* ε */ {
            $$ = create_node(NODE_NONTERMINAL, "DefList", @$.first_line);
        }
        ;

Def : Specifier DecList SEMI {
        $$ = create_node(NODE_NONTERMINAL, "Def", @$.first_line);
        add_child($$, $1);
        add_child($$, $2);
        add_child($$, create_node(NODE_TOKEN, "SEMI", @3.first_line));
    }
    | error SEMI { yyerrok; }
    ;

DecList : Dec COMMA DecList {
            $$ = create_node(NODE_NONTERMINAL, "DecList", @$.first_line);
            add_child($$, $1);
            add_child($$, create_node(NODE_TOKEN, "COMMA", @2.first_line));
            add_child($$, $3);
        }
        | Dec {
            $$ = create_node(NODE_NONTERMINAL, "DecList", @$.first_line);
            add_child($$, $1);
        }
        ;

Dec : VarDec {
        $$ = create_node(NODE_NONTERMINAL, "Dec", @$.first_line);
        add_child($$, $1);
    }
    | VarDec ASSIGNOP Exp {
        $$ = create_node(NODE_NONTERMINAL, "Dec", @$.first_line);
        add_child($$, $1);
        add_child($$, create_node(NODE_TOKEN, "ASSIGNOP", @2.first_line));
        add_child($$, $3);
    }
    ;

VarDec : ID {
            $$ = create_node(NODE_NONTERMINAL, "VarDec", @$.first_line);
            Node* id_node = create_node(NODE_ID, "ID", @1.first_line);
            id_node->attr.str_val = $1;
            add_child($$, id_node);
        }
        | VarDec LB INT RB {
            $$ = create_node(NODE_NONTERMINAL, "VarDec", @$.first_line);
            add_child($$, $1);
            add_child($$, create_node(NODE_TOKEN, "LB", @2.first_line));
            Node* int_node = create_node(NODE_INT, "INT", @3.first_line);
            int_node->attr.int_val = $3;
            add_child($$, int_node);
            add_child($$, create_node(NODE_TOKEN, "RB", @4.first_line));
        }
        | error RB { yyerrok; }
        ;

StmtList : Stmt StmtList {
            $$ = create_node(NODE_NONTERMINAL, "StmtList", @$.first_line);
            add_child($$, $1);
            add_child($$, $2);
        }
        | /* ε */ {
            $$ = create_node(NODE_NONTERMINAL, "StmtList", @$.first_line);
        }
        ;

Stmt : Exp SEMI {
        $$ = create_node(NODE_NONTERMINAL, "Stmt", @$.first_line);
        add_child($$, $1);
        add_child($$, create_node(NODE_TOKEN, "SEMI", @2.first_line));
    }
    | CompSt {
        $$ = create_node(NODE_NONTERMINAL, "Stmt", @$.first_line);
        add_child($$, $1);
    }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
        $$ = create_node(NODE_NONTERMINAL, "Stmt", @$.first_line);
        add_child($$, create_node(NODE_TOKEN, "IF", @1.first_line));
        add_child($$, create_node(NODE_TOKEN, "LP", @2.first_line));
        add_child($$, $3);
        add_child($$, create_node(NODE_TOKEN, "RP", @4.first_line));
        add_child($$, $5);
    }
    | IF LP Exp RP Stmt ELSE Stmt {
        $$ = create_node(NODE_NONTERMINAL, "Stmt", @$.first_line);
        add_child($$, create_node(NODE_TOKEN, "IF", @1.first_line));
        add_child($$, create_node(NODE_TOKEN, "LP", @2.first_line));
        add_child($$, $3);
        add_child($$, create_node(NODE_TOKEN, "RP", @4.first_line));
        add_child($$, $5);
        add_child($$, create_node(NODE_TOKEN, "ELSE", @6.first_line));
        add_child($$, $7);
    }
    | WHILE LP Exp RP Stmt {
        $$ = create_node(NODE_NONTERMINAL, "Stmt", @$.first_line);
        add_child($$, create_node(NODE_TOKEN, "WHILE", @1.first_line));
        add_child($$, create_node(NODE_TOKEN, "LP", @2.first_line));
        add_child($$, $3);
        add_child($$, create_node(NODE_TOKEN, "RP", @4.first_line));
        add_child($$, $5);
    }
    | RETURN Exp SEMI {
        $$ = create_node(NODE_NONTERMINAL, "Stmt", @$.first_line);
        add_child($$, create_node(NODE_TOKEN, "RETURN", @1.first_line));
        add_child($$, $2);
        add_child($$, create_node(NODE_TOKEN, "SEMI", @3.first_line));
    }
    | SEMI {
        $$ = create_node(NODE_NONTERMINAL, "Stmt", @$.first_line);
        add_child($$, create_node(NODE_TOKEN, "SEMI", @1.first_line));
    }
    | error SEMI { yyerrok; }
    ;

Exp : Exp ASSIGNOP Exp {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, $1);
        add_child($$, create_node(NODE_TOKEN, "ASSIGNOP", @2.first_line));
        add_child($$, $3);
    }
    | Exp OR Exp {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, $1);
        add_child($$, create_node(NODE_TOKEN, "OR", @2.first_line));
        add_child($$, $3);
    }
    | Exp AND Exp {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, $1);
        add_child($$, create_node(NODE_TOKEN, "AND", @2.first_line));
        add_child($$, $3);
    }
    | Exp RELOP Exp {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, $1);
        Node* relop_node = create_node(NODE_TOKEN, $2, @2.first_line);
        relop_node->attr.str_val = $2;
        add_child($$, relop_node);
        add_child($$, $3);
    }
    | Exp PLUS Exp {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, $1);
        add_child($$, create_node(NODE_TOKEN, "PLUS", @2.first_line));
        add_child($$, $3);
    }
    | Exp MINUS Exp {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, $1);
        add_child($$, create_node(NODE_TOKEN, "MINUS", @2.first_line));
        add_child($$, $3);
    }
    | Exp STAR Exp {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, $1);
        add_child($$, create_node(NODE_TOKEN, "STAR", @2.first_line));
        add_child($$, $3);
    }
    | Exp DIV Exp {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, $1);
        add_child($$, create_node(NODE_TOKEN, "DIV", @2.first_line));
        add_child($$, $3);
    }
    | LP Exp RP {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, create_node(NODE_TOKEN, "LP", @1.first_line));
        add_child($$, $2);
        add_child($$, create_node(NODE_TOKEN, "RP", @3.first_line));
    }
    | MINUS Exp %prec UMINUS {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, create_node(NODE_TOKEN, "MINUS", @1.first_line));
        add_child($$, $2);
    }
    | NOT Exp {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, create_node(NODE_TOKEN, "NOT", @1.first_line));
        add_child($$, $2);
    }
    | Exp DOT ID {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, $1);
        add_child($$, create_node(NODE_TOKEN, "DOT", @2.first_line));
        Node* id_node = create_node(NODE_ID, "ID", @3.first_line);
        id_node->attr.str_val = $3;
        add_child($$, id_node);
    }
    | ID LP Args RP {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        Node* id_node = create_node(NODE_ID, "ID", @1.first_line);
        id_node->attr.str_val = $1;
        add_child($$, id_node);
        add_child($$, create_node(NODE_TOKEN, "LP", @2.first_line));
        add_child($$, $3);
        add_child($$, create_node(NODE_TOKEN, "RP", @4.first_line));
    }
    | ID LP RP {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        Node* id_node = create_node(NODE_ID, "ID", @1.first_line);
        id_node->attr.str_val = $1;
        add_child($$, id_node);
        add_child($$, create_node(NODE_TOKEN, "LP", @2.first_line));
        add_child($$, create_node(NODE_TOKEN, "RP", @3.first_line));
    }
    | Exp LB Exp RB {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        add_child($$, $1);
        add_child($$, create_node(NODE_TOKEN, "LB", @2.first_line));
        add_child($$, $3);
        add_child($$, create_node(NODE_TOKEN, "RB", @4.first_line));
    }
    | ID {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        Node* id_node = create_node(NODE_ID, "ID", @1.first_line);
        id_node->attr.str_val = $1;
        add_child($$, id_node);
    }
    | INT {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        Node* int_node = create_node(NODE_INT, "INT", @1.first_line);
        int_node->attr.int_val = $1;
        add_child($$, int_node);
    }
    | FLOAT {
        $$ = create_node(NODE_NONTERMINAL, "Exp", @$.first_line);
        Node* float_node = create_node(NODE_FLOAT, "FLOAT", @1.first_line);
        float_node->attr.float_val = $1;
        add_child($$, float_node);
    }
    | error RP { yyerrok; }
    | error RB { yyerrok; }
    ;

Args : Exp COMMA Args {
        $$ = create_node(NODE_NONTERMINAL, "Args", @$.first_line);
        add_child($$, $1);
        add_child($$, create_node(NODE_TOKEN, "COMMA", @2.first_line));
        add_child($$, $3);
    }
    | Exp {
        $$ = create_node(NODE_NONTERMINAL, "Args", @$.first_line);
        add_child($$, $1);
    }
    ;
%%