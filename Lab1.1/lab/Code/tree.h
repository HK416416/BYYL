#ifndef TREE_H
#define TREE_H

typedef struct Node {
    char* name;
    int lineno;
    int child_num;
    struct Node** children;
    union {
        char* id;      // for ID
        char* type;    // for TYPE (int/float)
        int ival;      // for INT
        float fval;    // for FLOAT
    } attr;
} Node;

Node* new_node(const char* name, int lineno, int n, ...);
Node* new_leaf(const char* name, int lineno);
Node* new_id(const char* id, int lineno);
Node* new_int(int val, int lineno);
Node* new_float(float val, int lineno);
Node* new_type(const char* type, int lineno);
void print_tree(Node* root, int depth);

#endif