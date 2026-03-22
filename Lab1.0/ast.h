#ifndef AST_H
#define AST_H

typedef struct ast_node {
    char* name;
    int lineno;
    int child_num;
    struct ast_node** children;
    union {
        char* string;
        int intval;
        float floatval;
    } data;
} ast_node;

ast_node* new_ast_node(const char* name, int lineno, int child_count, ...);
void free_ast(ast_node* node);
void print_ast(ast_node* node, int depth);

#endif