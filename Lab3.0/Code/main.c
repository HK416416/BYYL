#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern char* strdup(const char*);

/* 语义分析头文件 */
#include "semantic.h"

/* AST 类型与实现（从原 ast.h/语法文件中移动到这里） */
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

Node* root = NULL;
int has_syntax_error = 0; /* deprecated: kept for compatibility */
int has_error = 0; /* set to 1 when lexer or parser reports any error */

Node* create_node(const char* name, int line, int num_children, ...) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->name = strdup(name);
    node->line = line;
    node->is_terminal = 0;
    node->u.nonterm.num_children = num_children;
    node->u.nonterm.children = (Node**)malloc(num_children * sizeof(Node*));
    va_list args;
    va_start(args, num_children);
    for (int i = 0; i < num_children; ++i) {
        node->u.nonterm.children[i] = va_arg(args, Node*);
    }
    va_end(args);
    return node;
}

Node* create_terminal_node(const char* name, int line) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->name = strdup(name);
    node->line = line;
    node->is_terminal = 1;
    node->u.term.value = NULL;
    node->u.term.int_val = 0;
    node->u.term.float_val = 0.0;
    return node;
}

Node* create_id_node(const char* value, int line) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->name = strdup("ID");
    node->line = line;
    node->is_terminal = 1;
    node->u.term.value = strdup(value);
    return node;
}

Node* create_int_node(int val, int line) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->name = strdup("INT");
    node->line = line;
    node->is_terminal = 1;
    node->u.term.int_val = val;
    return node;
}

Node* create_float_node(float val, int line) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->name = strdup("FLOAT");
    node->line = line;
    node->is_terminal = 1;
    node->u.term.float_val = val;
    return node;
}

Node* create_string_node(const char* value, int line) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->name = strdup("STRING");
    node->line = line;
    node->is_terminal = 1;
    node->u.term.value = strdup(value);
    return node;
}

void print_tree(Node* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) printf("  ");
    if (node->is_terminal) {
        printf("%s", node->name);
        if (strcmp(node->name, "ID") == 0 && node->u.term.value) {
            printf(": %s", node->u.term.value);
        } else if (strcmp(node->name, "TYPE") == 0 && node->u.term.value) {
            printf(": %s", node->u.term.value);
        } else if (strcmp(node->name, "INT") == 0) {
            printf(": %d", node->u.term.int_val);
        } else if (strcmp(node->name, "FLOAT") == 0) {
            printf(": %f", node->u.term.float_val);
        }
        printf("\n");
    } else {
        printf("%s (%d)\n", node->name, node->line);
        for (int i = 0; i < node->u.nonterm.num_children; ++i) {
            print_tree(node->u.nonterm.children[i], depth + 1);
        }
    }
}

void free_node(Node* node) {
    if (!node) return;
    if (!node->is_terminal) {
        for (int i = 0; i < node->u.nonterm.num_children; ++i) {
            free_node(node->u.nonterm.children[i]);
        }
        free(node->u.nonterm.children);
    } else {
        if (node->u.term.value) free(node->u.term.value);
    }
    free(node->name);
    free(node);
}

extern int yyparse();
extern FILE* yyin;


#include "lex.yy.c"
#include "translate.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s input.cmm output.ir\n", argv[0]);
        return 1;
    }
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        printf("Cannot open file: %s\n", argv[1]);
        return 0;
    }
    yyin = f;
    yyparse();
    fclose(f);
    
    /* 如果没有词法或语法错误，进行语义分析 */
    if (!has_error && root) {
        /* 执行语义分析并保留上下文 */
        SemanticContext* ctx = semantic_analysis_with_context(root);
        
        /* 如果语义分析也没有错误，进行中间代码翻译 */
        if (!has_error && ctx) {
            FILE* out = fopen(argv[2], "w");
            if (out) {
                int ret = translate_program(ctx, root, out);
                fclose(out);
                if (ret != 0) {
                    /* Translation error — re-read and output error message */
                    /* translate_program already wrote error to file,
                       but we need to handle the "Cannot translate" case */
                }
            } else {
                printf("Cannot open output file: %s\n", argv[2]);
            }
        }
        
        if (ctx) destroy_semantic_context(ctx);
        free_node(root);
    }
    /* Always exit with status 0 per testing platform requirement */
    return 0;
}