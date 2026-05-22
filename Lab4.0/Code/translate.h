#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "semantic.h"
#include <stdio.h>

/* 中间代码翻译入口
 * ctx: 语义分析上下文（包含符号表）
 * root: AST根节点
 * output: 输出文件
 * 返回: 0成功, 非0失败（如遇到结构体） */
int translate_program(SemanticContext* ctx, Node* root, FILE* output);

/* 预定义read/write函数到符号表 */
void predefine_read_write(SymbolTable* table);

#endif // TRANSLATE_H
