#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* strdup在C99中不是标准函数，需要声明 */
extern char* strdup(const char*);

/* 创建参数列表节点 */
ParamList* new_param_list(char* name, Type* type) {
    ParamList* param = (ParamList*)malloc(sizeof(ParamList));
    param->name = strdup(name);
    param->type = type;
    param->next = NULL;
    return param;
}

/* 释放参数列表内存 */
void free_param_list(ParamList* param) {
    while (param != NULL) {
        ParamList* next = param->next;
        free(param->name);
        free_type(param->type);
        free(param);
        param = next;
    }
}

/* 创建变量符号 */
Symbol* new_variable_symbol(char* name, Type* type, int line) {
    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->kind = SYMBOL_VARIABLE;
    symbol->line = line;
    symbol->is_defined = 1;
    symbol->u.variable.type = type;
    symbol->next = NULL;
    return symbol;
}

/* 创建函数符号 */
Symbol* new_function_symbol(char* name, Type* return_type, ParamList* params, int line) {
    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->kind = SYMBOL_FUNCTION;
    symbol->line = line;
    symbol->is_defined = 1;
    symbol->u.function.return_type = return_type;
    symbol->u.function.params = params;
    
    // 计算参数个数
    int count = 0;
    ParamList* p = params;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    symbol->u.function.param_count = count;
    
    symbol->next = NULL;
    return symbol;
}

/* 创建结构体符号 */
Symbol* new_struct_symbol(char* name, Type* type, int line) {
    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->kind = SYMBOL_STRUCT;
    symbol->line = line;
    symbol->is_defined = 1;
    symbol->u.structure.type = type;
    symbol->next = NULL;
    return symbol;
}

/* 释放符号内存 */
void free_symbol(Symbol* symbol) {
    if (symbol == NULL) return;
    
    free(symbol->name);
    
    switch (symbol->kind) {
        case SYMBOL_VARIABLE:
            free_type(symbol->u.variable.type);
            break;
        case SYMBOL_FUNCTION:
            free_type(symbol->u.function.return_type);
            free_param_list(symbol->u.function.params);
            break;
        case SYMBOL_STRUCT:
            free_type(symbol->u.structure.type);
            break;
    }
    
    free(symbol);
}

/* 初始化符号表 */
SymbolTable* init_symbol_table(int size) {
    SymbolTable* table = (SymbolTable*)malloc(sizeof(SymbolTable));
    table->size = size;
    table->count = 0;
    table->buckets = (Symbol**)calloc(size, sizeof(Symbol*));
    return table;
}

/* 销毁符号表 */
void destroy_symbol_table(SymbolTable* table) {
    if (table == NULL) return;
    
    for (int i = 0; i < table->size; i++) {
        Symbol* symbol = table->buckets[i];
        while (symbol != NULL) {
            Symbol* next = symbol->next;
            free_symbol(symbol);
            symbol = next;
        }
    }
    
    free(table->buckets);
    free(table);
}

/* PJW哈希函数（来自PDF） */
unsigned int hash_pjw(char* name) {
    unsigned int val = 0, i;
    for (; *name; ++name) {
        val = (val << 2) + *name;
        if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;
    }
    return val;
}

/* 插入符号到符号表 */
int insert_symbol(SymbolTable* table, Symbol* symbol) {
    if (table == NULL || symbol == NULL) return 0;
    
    unsigned int index = hash_pjw(symbol->name) % table->size;
    
    // 检查是否已存在同名符号
    Symbol* curr = table->buckets[index];
    while (curr != NULL) {
        if (strcmp(curr->name, symbol->name) == 0) {
            // 同名符号已存在
            return 0;
        }
        curr = curr->next;
    }
    
    // 插入到链表头部
    symbol->next = table->buckets[index];
    table->buckets[index] = symbol;
    table->count++;
    
    return 1;
}

/* 查找符号 */
Symbol* find_symbol(SymbolTable* table, char* name) {
    if (table == NULL || name == NULL) return NULL;
    
    unsigned int index = hash_pjw(name) % table->size;
    Symbol* symbol = table->buckets[index];
    
    while (symbol != NULL) {
        if (strcmp(symbol->name, name) == 0) {
            return symbol;
        }
        symbol = symbol->next;
    }
    
    return NULL;
}

/* 删除符号 */
int delete_symbol(SymbolTable* table, char* name) {
    if (table == NULL || name == NULL) return 0;
    
    unsigned int index = hash_pjw(name) % table->size;
    Symbol* prev = NULL;
    Symbol* curr = table->buckets[index];
    
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            if (prev == NULL) {
                table->buckets[index] = curr->next;
            } else {
                prev->next = curr->next;
            }
            
            free_symbol(curr);
            table->count--;
            return 1;
        }
        
        prev = curr;
        curr = curr->next;
    }
    
    return 0;
}

/* 检查变量是否已定义 */
int is_variable_defined(SymbolTable* table, char* name) {
    Symbol* symbol = find_symbol(table, name);
    return (symbol != NULL && symbol->kind == SYMBOL_VARIABLE);
}

/* 检查函数是否已定义 */
int is_function_defined(SymbolTable* table, char* name) {
    Symbol* symbol = find_symbol(table, name);
    return (symbol != NULL && symbol->kind == SYMBOL_FUNCTION && symbol->is_defined);
}

/* 检查结构体是否已定义 */
int is_struct_defined(SymbolTable* table, char* name) {
    Symbol* symbol = find_symbol(table, name);
    return (symbol != NULL && symbol->kind == SYMBOL_STRUCT);
}

/* 辅助函数：符号种类转字符串 */
static const char* symbol_kind_to_string(SymbolKind kind) {
    switch (kind) {
        case SYMBOL_VARIABLE: return "variable";
        case SYMBOL_FUNCTION: return "function";
        case SYMBOL_STRUCT: return "struct";
        default: return "unknown";
    }
}

/* 打印符号表（用于调试） */
void print_symbol_table(SymbolTable* table) {
    if (table == NULL) {
        printf("Symbol table is NULL\n");
        return;
    }
    
    printf("Symbol Table (size=%d, count=%d):\n", table->size, table->count);
    
    for (int i = 0; i < table->size; i++) {
        Symbol* symbol = table->buckets[i];
        if (symbol != NULL) {
            printf("  Bucket %d:\n", i);
            while (symbol != NULL) {
                char* type_str = NULL;
                
                switch (symbol->kind) {
                    case SYMBOL_VARIABLE:
                        type_str = type_to_string(symbol->u.variable.type);
                        printf("    %s (variable, line %d): %s\n", 
                               symbol->name, symbol->line, type_str);
                        break;
                    case SYMBOL_FUNCTION: {
                        type_str = type_to_string(symbol->u.function.return_type);
                        printf("    %s (function, line %d): returns %s, params=%d\n",
                               symbol->name, symbol->line, type_str, symbol->u.function.param_count);
                        break;
                    }
                    case SYMBOL_STRUCT:
                        type_str = type_to_string(symbol->u.structure.type);
                        printf("    %s (struct, line %d): %s\n",
                               symbol->name, symbol->line, type_str);
                        break;
                }
                
                if (type_str != NULL) {
                    free(type_str);
                }
                symbol = symbol->next;
            }
        }
    }
}