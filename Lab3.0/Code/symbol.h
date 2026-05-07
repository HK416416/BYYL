#ifndef SYMBOL_H
#define SYMBOL_H

#include "type.h"

/* 符号种类 */
typedef enum {
    SYMBOL_VARIABLE,    // 变量
    SYMBOL_FUNCTION,    // 函数
    SYMBOL_STRUCT       // 结构体
} SymbolKind;

/* 函数参数列表 */
typedef struct ParamList_ ParamList;
struct ParamList_ {
    char* name;        // 参数名
    Type* type;        // 参数类型
    ParamList* next;   // 下一个参数
};

/* 符号结构体 */
typedef struct Symbol_ Symbol;
struct Symbol_ {
    char* name;           // 符号名
    SymbolKind kind;      // 符号种类
    int line;            // 定义行号
    int is_defined;      // 是否已定义（用于函数声明）
    
    union {
        // 变量
        struct {
            Type* type;   // 变量类型
        } variable;
        
        // 函数
        struct {
            Type* return_type;   // 返回类型
            ParamList* params;   // 参数列表
            int param_count;     // 参数个数
        } function;
        
        // 结构体
        struct {
            Type* type;   // 结构体类型定义
        } structure;
    } u;
    
    Symbol* next;        // 哈希表链表下一个
};

/* 符号表结构体 */
typedef struct {
    Symbol** buckets;    // 哈希桶数组
    int size;           // 哈希表大小
    int count;          // 符号数量
} SymbolTable;

/* 函数声明 */

// 创建参数列表节点
ParamList* new_param_list(char* name, Type* type);

// 释放参数列表内存
void free_param_list(ParamList* param);

// 创建变量符号
Symbol* new_variable_symbol(char* name, Type* type, int line);

// 创建函数符号
Symbol* new_function_symbol(char* name, Type* return_type, ParamList* params, int line);

// 创建结构体符号
Symbol* new_struct_symbol(char* name, Type* type, int line);

// 释放符号内存
void free_symbol(Symbol* symbol);

// 初始化符号表
SymbolTable* init_symbol_table(int size);

// 销毁符号表
void destroy_symbol_table(SymbolTable* table);

// 哈希函数
unsigned int hash_pjw(char* name);

// 插入符号到符号表
int insert_symbol(SymbolTable* table, Symbol* symbol);

// 查找符号
Symbol* find_symbol(SymbolTable* table, char* name);

// 删除符号
int delete_symbol(SymbolTable* table, char* name);

// 检查变量是否已定义
int is_variable_defined(SymbolTable* table, char* name);

// 检查函数是否已定义
int is_function_defined(SymbolTable* table, char* name);

// 检查结构体是否已定义
int is_struct_defined(SymbolTable* table, char* name);

// 打印符号表（用于调试）
void print_symbol_table(SymbolTable* table);

#endif // SYMBOL_H