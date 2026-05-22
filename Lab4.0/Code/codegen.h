#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>

/* 从IR输入文件读取，生成MIPS32汇编代码到输出文件
 * 返回: 0成功, 非0失败 */
int codegen_generate(FILE* ir_input, FILE* asm_output);

#endif // CODEGEN_H
