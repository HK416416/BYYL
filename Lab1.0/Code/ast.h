#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct Node {
    char* name;
    int line;
    int is_terminal;      // 1: 终结符, 0: 非终结符
    union {
        struct {
            struct Node** children;
            int num_children;
        } nonterm;
        struct {
            char* value;   // ID, STRING
            int int_val;
            float float_val;
        } term;
    } u;
} Node;

extern Node* root;
extern int has_syntax_error;
extern int has_error;

Node* create_node(const char* name, int line, int num_children, ...);
Node* create_terminal_node(const char* name, int line);
Node* create_id_node(const char* value, int line);
Node* create_int_node(int val, int line);
Node* create_float_node(float val, int line);
Node* create_string_node(const char* value, int line);
void print_tree(Node* node, int depth);
void free_node(Node* node);

#endif