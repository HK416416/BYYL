#include <stdio.h>
#include <stdlib.h>
#include "syntax.tab.h"

// 外部变量与函数声明（与syntax.y中的声明对应）
extern FILE* yyin;
extern int yyparse();
extern int yylineno;
extern void yyrestart(FILE* input_file);

int main(int argc, char** argv) {
    // 检查输入参数
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file.cmm>\n", argv[0]);
        return 1;
    }

    // 打开输入文件
    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror(argv[1]);
        return 1;
    }

    // 初始化词法分析输入
    yyin = input_file;
    yylineno = 1;
    yyrestart(input_file);

    // 执行语法分析
    yyparse();

    // 清理资源
    fclose(input_file);
    return 0;
}

// 词法/语法错误通用输出函数
void print_error(char type, int line, const char* msg) {
    fprintf(stderr, "Error type %c at Line %d: %s.\n", type, line, msg);
}