#ifndef TYPE_H
#define TYPE_H

#include <stdbool.h>

/* 基本类型枚举 */
typedef enum { TYPE_INT, TYPE_FLOAT } BasicType;

/* 类型种类枚举 */
typedef enum { 
    TYPE_KIND_BASIC,      // 基本类型
    TYPE_KIND_ARRAY,      // 数组类型
    TYPE_KIND_STRUCTURE   // 结构体类型
} TypeKind;

/* 前向声明 */
typedef struct Type_ Type;
typedef struct FieldList_ FieldList;

/* 类型结构体 */
struct Type_ {
    TypeKind kind;
    union {
        BasicType basic;                     // 基本类型
        struct { Type* elem; int size; } array; // 数组类型：元素类型和大小
        FieldList* structure;                // 结构体类型：域列表
    } u;
};

/* 结构体域列表 */
struct FieldList_ {
    char* name;      // 域名
    Type* type;      // 域类型
    FieldList* tail; // 下一个域
    int line;        // 定义行号（用于错误报告）
};

/* 函数声明 */

// 创建基本类型
Type* new_type_basic(BasicType basic);

// 创建数组类型
Type* new_type_array(Type* elem, int size);

// 创建结构体类型
Type* new_type_structure(FieldList* fields);

// 创建域列表节点
FieldList* new_field_list(char* name, Type* type, int line);

// 添加域到结构体
void add_field_to_structure(Type* struct_type, FieldList* field);

// 复制类型（深拷贝）
Type* copy_type(Type* type);

// 释放类型内存
void free_type(Type* type);

// 释放域列表内存
void free_field_list(FieldList* field);

// 检查两个类型是否结构等价
bool is_structurally_equivalent(Type* t1, Type* t2);

// 检查两个类型是否兼容（用于赋值、运算等）
bool is_type_compatible(Type* t1, Type* t2);

// 类型转字符串（用于调试）
char* type_to_string(Type* type);

#endif // TYPE_H