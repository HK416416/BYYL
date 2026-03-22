#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


ast_node* new_ast_node(const char* name, int lineno, int child_count, ...) {
    ast_node* node = (ast_node*)malloc(sizeof(ast_node));
    node->name = strdup(name);
    node->lineno = lineno;
    node->child_num = child_count;
    node->children = (child_count > 0) ? (ast_node**)malloc(sizeof(ast_node*) * child_count) : NULL;
    
    va_list ap;
    va_start(ap, child_count);
    for (int i = 0; i < child_count; i++) {
        node->children[i] = va_arg(ap, ast_node*);
    }
    va_end(ap);
    
    memset(&node->data, 0, sizeof(node->data));
    return node;
}

void free_ast(ast_node* node) {
    if (!node) return;
    for (int i = 0; i < node->child_num; i++) {
        free_ast(node->children[i]);
    }
    if (node->children) free(node->children);
    if (node->name) free(node->name);
    if (node->data.string) free(node->data.string);
    free(node);
}

void print_ast(ast_node* node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; i++) printf("  ");
    
    if (strcmp(node->name, "TYPE") == 0) {
        printf("TYPE: %s\n", node->data.string);
    } else if (strcmp(node->name, "ID") == 0) {
        printf("ID: %s\n", node->data.string);
    } else if (strcmp(node->name, "INT") == 0) {
        printf("INT: %d\n", node->data.intval);
    } else if (strcmp(node->name, "FLOAT") == 0) {
        printf("FLOAT: %f\n", node->data.floatval);
    } else {
        printf("%s", node->name);
        if (node->lineno > 0) printf(" (%d)", node->lineno);
        printf("\n");
    }
    
    for (int i = 0; i < node->child_num; i++) {
        print_ast(node->children[i], depth + 1);
    }
}