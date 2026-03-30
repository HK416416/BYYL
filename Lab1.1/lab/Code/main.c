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

        // 添加这三行调试代码
        printf("DEBUG: lexical_error = %d\n", lexical_error);
        printf("DEBUG: syntax_error = %d\n", syntax_error);
        printf("DEBUG: root = %p\n", root);

        if (lexical_error == 0 && syntax_error == 0) {
            
            if (root) {
                printf("=== DEBUG: root exists, calling print_tree ===\n");  // 添加这行
                print_tree(root, 0);
            }
            else {
                printf("=== DEBUG: root is NULL ===\n");  // 添加这行
            }
        }
    }
    return 0;
}