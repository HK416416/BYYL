#include <stdio.h>
#include "ast.h"

extern int yyparse();
extern FILE* yyin;
extern Node* root;

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s input.cmm\n", argv[0]);
        return 1;
    }
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }
    yyin = f;
    yyparse();
    /* Only print the AST if no lexical or syntax error occurred during parsing */
    if (!has_error && root) {
        print_tree(root, 0);
        free_node(root);
    }
    fclose(f);
    return 0;
}