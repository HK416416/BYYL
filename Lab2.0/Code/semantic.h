#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "type.h"
#include "symbol.h"

/* 前向声明AST节点 */
typedef struct Node Node;

/* 语义分析上下文 */
typedef struct {
    SymbolTable* global_table;      // 全局符号表
    SymbolTable* current_table;     // 当前符号表（用于作用域，当前为全局）
    Type* current_return_type;      // 当前函数的返回类型
    int error_count;               // 错误计数
    int has_error;                 // 是否有错误
} SemanticContext;

/* 表达式类型信息 */
typedef struct {
    Type* type;                    // 表达式类型
    int is_lvalue;                 // 是否是左值
    int line;                      // 行号
} ExpTypeInfo;

/* 函数声明 */

// 初始化语义分析上下文
SemanticContext* init_semantic_context();

// 销毁语义分析上下文
void destroy_semantic_context(SemanticContext* context);

// 执行语义分析
void semantic_analysis(Node* root);

// 报告语义错误
void report_semantic_error(SemanticContext* context, int error_type, int line, const char* fmt, ...);

/* 错误类型枚举（对应PDF中的17种错误） */
typedef enum {
    ERROR_UNDEFINED_VARIABLE = 1,      // 变量在使用时未经定义
    ERROR_UNDEFINED_FUNCTION,          // 函数在调用时未经定义
    ERROR_REDEFINED_VARIABLE,          // 变量出现重复定义，或变量与前面定义过的结构体名字重复
    ERROR_REDEFINED_FUNCTION,          // 函数出现重复定义
    ERROR_TYPE_MISMATCH_ASSIGNMENT,    // 赋值号两边的表达式类型不匹配
    ERROR_NON_LVALUE_ASSIGNMENT,       // 赋值号左边出现一个只有右值的表达式
    ERROR_TYPE_MISMATCH_OPERANDS,      // 操作数类型不匹配或操作数类型与操作符不匹配
    ERROR_TYPE_MISMATCH_RETURN,        // return语句的返回类型与函数定义的返回类型不匹配
    ERROR_FUNCTION_ARGUMENT_MISMATCH,  // 函数调用时实参与形参的数目或类型不匹配
    ERROR_NON_ARRAY_SUBSCRIPT,         // 对非数组型变量使用数组访问操作符
    ERROR_NON_FUNCTION_CALL,           // 对普通变量使用函数调用操作符
    ERROR_NON_INTEGER_SUBSCRIPT,       // 数组访问操作符中出现非整数
    ERROR_NON_STRUCTURE_DOT,           // 对非结构体型变量使用"."操作符
    ERROR_UNDEFINED_FIELD,             // 访问结构体中未定义过的域
    ERROR_REDEFINED_FIELD,             // 结构体中域名重复定义
    ERROR_REDEFINED_STRUCTURE,         // 结构体的名字与前面定义过的结构体或变量的名字重复
    ERROR_UNDEFINED_STRUCTURE          // 直接使用未定义过的结构体来定义变量
} SemanticErrorType;

/* AST遍历函数（在semantic.c中实现） */
void analyze_program(SemanticContext* context, Node* node);
void analyze_ext_def_list(SemanticContext* context, Node* node);
void analyze_ext_def(SemanticContext* context, Node* node);
void analyze_specifier(SemanticContext* context, Node* node, Type** type);
void analyze_struct_specifier(SemanticContext* context, Node* node, Type** type);
void analyze_var_dec(SemanticContext* context, Node* node, Type* base_type, char** name, Type** type);
void analyze_fun_dec(SemanticContext* context, Node* node, Type* return_type);
void analyze_param_dec(SemanticContext* context, Node* node, ParamList** param);
void analyze_comp_st(SemanticContext* context, Node* node, Type* return_type);
void analyze_stmt_list(SemanticContext* context, Node* node);
void analyze_stmt(SemanticContext* context, Node* node);
ExpTypeInfo analyze_exp(SemanticContext* context, Node* node);
void analyze_def_list(SemanticContext* context, Node* node);
void analyze_def(SemanticContext* context, Node* node);
void analyze_dec_list(SemanticContext* context, Node* node, Type* base_type);
void analyze_dec(SemanticContext* context, Node* node, Type* base_type);

#endif // SEMANTIC_H