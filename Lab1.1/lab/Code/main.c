#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "syntax.tab.h"

extern FILE* yyin;
extern int lexical_error;
extern int syntax_error;
extern Node* root;

int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* f = fopen(argv[1], "r");
        if (!f) {
            perror(argv[1]);
            return 1;
        }
        yyin = f;
        yyparse();
        fclose(f);
        if (lexical_error == 0 && syntax_error == 0) {
            if (root) {
                print_tree(root, 0);
            }
        }
    }
    return 0;
}