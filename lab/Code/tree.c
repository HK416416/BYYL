#define _GNU_SOURCE
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


Node* new_node(const char* name, int lineno, int n, ...) {
    va_list ap;
    va_start(ap, n);
    Node** children = malloc(sizeof(Node*) * n);
    int count = 0;
    for (int i = 0; i < n; i++) {
        Node* child = va_arg(ap, Node*);
        if (child != NULL) {
            children[count++] = child;
        }
    }
    va_end(ap);

    Node* node = malloc(sizeof(Node));
    node->name = strdup(name);
    node->lineno = lineno;
    node->child_num = count;
    node->children = malloc(sizeof(Node*) * count);
    for (int i = 0; i < count; i++) {
        node->children[i] = children[i];
    }
    free(children);

    node->attr.id = NULL;
    node->attr.type = NULL;
    node->attr.ival = 0;
    node->attr.fval = 0.0;
    return node;
}

Node* new_leaf(const char* name, int lineno) {
    return new_node(name, lineno, 0);
}

Node* new_id(const char* id, int lineno) {
    Node* n = new_leaf("ID", lineno);
    n->attr.id = strdup(id);
    return n;
}

Node* new_int(int val, int lineno) {
    Node* n = new_leaf("INT", lineno);
    n->attr.ival = val;
    return n;
}

Node* new_float(float val, int lineno) {
    Node* n = new_leaf("FLOAT", lineno);
    n->attr.fval = val;
    return n;
}

Node* new_type(const char* type, int lineno) {
    Node* n = new_leaf("TYPE", lineno);
    n->attr.type = strdup(type);
    return n;
}

void print_tree(Node* root, int depth) {
    if (root == NULL) return;
    for (int i = 0; i < depth; i++) printf("  ");

    if (strcmp(root->name, "ID") == 0) {
        printf("ID: %s\n", root->attr.id);
    } else if (strcmp(root->name, "TYPE") == 0) {
        printf("TYPE: %s\n", root->attr.type);
    } else if (strcmp(root->name, "INT") == 0) {
        printf("INT: %d\n", root->attr.ival);
    } else if (strcmp(root->name, "FLOAT") == 0) {
        printf("FLOAT: %f\n", root->attr.fval);
    } else {
        printf("%s", root->name);
        if (root->child_num > 0) {
            printf(" (%d)", root->lineno);
        }
        printf("\n");
    }

    for (int i = 0; i < root->child_num; i++) {
        print_tree(root->children[i], depth + 1);
    }
}