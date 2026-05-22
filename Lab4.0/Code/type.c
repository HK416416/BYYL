#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char* strdup(const char*);

/* 创建基本类型 */
Type* new_type_basic(BasicType basic) {
    Type* type = (Type*)malloc(sizeof(Type));
    type->kind = TYPE_KIND_BASIC;
    type->u.basic = basic;
    return type;
}

/* 创建数组类型 */
Type* new_type_array(Type* elem, int size) {
    Type* type = (Type*)malloc(sizeof(Type));
    type->kind = TYPE_KIND_ARRAY;
    type->u.array.elem = elem;
    type->u.array.size = size;
    return type;
}

/* 创建结构体类型 */
Type* new_type_structure(FieldList* fields) {
    Type* type = (Type*)malloc(sizeof(Type));
    type->kind = TYPE_KIND_STRUCTURE;
    type->u.structure = fields;
    return type;
}

/* 创建域列表节点 */
FieldList* new_field_list(char* name, Type* type, int line) {
    FieldList* field = (FieldList*)malloc(sizeof(FieldList));
    field->name = strdup(name);
    field->type = type;
    field->tail = NULL;
    field->line = line;
    return field;
}

/* 添加域到结构体 */
void add_field_to_structure(Type* struct_type, FieldList* field) {
    if (struct_type->kind != TYPE_KIND_STRUCTURE) {
        return;
    }
    
    FieldList* last = struct_type->u.structure;
    if (last == NULL) {
        struct_type->u.structure = field;
    } else {
        while (last->tail != NULL) {
            last = last->tail;
        }
        last->tail = field;
    }
}

/* 复制类型（深拷贝） */
Type* copy_type(Type* type) {
    if (type == NULL) return NULL;
    
    Type* new_type = (Type*)malloc(sizeof(Type));
    new_type->kind = type->kind;
    
    switch (type->kind) {
        case TYPE_KIND_BASIC:
            new_type->u.basic = type->u.basic;
            break;
        case TYPE_KIND_ARRAY:
            new_type->u.array.elem = copy_type(type->u.array.elem);
            new_type->u.array.size = type->u.array.size;
            break;
        case TYPE_KIND_STRUCTURE:
            // 复制结构体域列表
            if (type->u.structure != NULL) {
                FieldList* src = type->u.structure;
                FieldList* dst = new_field_list(src->name, copy_type(src->type), src->line);
                FieldList* dst_head = dst;
                
                src = src->tail;
                while (src != NULL) {
                    dst->tail = new_field_list(src->name, copy_type(src->type), src->line);
                    dst = dst->tail;
                    src = src->tail;
                }
                new_type->u.structure = dst_head;
            } else {
                new_type->u.structure = NULL;
            }
            break;
    }
    
    return new_type;
}

/* 释放类型内存 */
void free_type(Type* type) {
    if (type == NULL) return;
    
    switch (type->kind) {
        case TYPE_KIND_BASIC:
            // 基本类型没有额外内存
            break;
        case TYPE_KIND_ARRAY:
            free_type(type->u.array.elem);
            break;
        case TYPE_KIND_STRUCTURE:
            free_field_list(type->u.structure);
            break;
    }
    
    free(type);
}

/* 释放域列表内存 */
void free_field_list(FieldList* field) {
    while (field != NULL) {
        FieldList* next = field->tail;
        free(field->name);
        free_type(field->type);
        free(field);
        field = next;
    }
}

/* 检查两个类型是否结构等价 */
bool is_structurally_equivalent(Type* t1, Type* t2) {
    if (t1 == NULL || t2 == NULL) {
        return t1 == t2;  // 都为NULL则等价
    }
    
    if (t1->kind != t2->kind) {
        return false;
    }
    
    switch (t1->kind) {
        case TYPE_KIND_BASIC:
            return t1->u.basic == t2->u.basic;
            
        case TYPE_KIND_ARRAY:
            // 数组类型：根据 requirements 的数组例外规则，仅比较元素类型（递归）和维度结构，
            // 不比较具体的数组大小值。
            return is_structurally_equivalent(t1->u.array.elem, t2->u.array.elem);
            
        case TYPE_KIND_STRUCTURE: {
            // 结构体类型：比较域列表
            FieldList* f1 = t1->u.structure;
            FieldList* f2 = t2->u.structure;

            while (f1 != NULL && f2 != NULL) {
                // 域名可以不同，但类型必须结构等价
                if (!is_structurally_equivalent(f1->type, f2->type)) {
                    return false;
                }
                f1 = f1->tail;
                f2 = f2->tail;
            }

            // 两个列表必须同时结束
            return f1 == NULL && f2 == NULL;
        }
    }
    
    return false;
}

/* 检查两个类型是否兼容（用于赋值、运算等） */
bool is_type_compatible(Type* t1, Type* t2) {
    // 对于结构等价，类型兼容性就是结构等价
    return is_structurally_equivalent(t1, t2);
}

/* 辅助函数：类型种类转字符串 */
static const char* kind_to_string(TypeKind kind) {
    switch (kind) {
        case TYPE_KIND_BASIC: return "basic";
        case TYPE_KIND_ARRAY: return "array";
        case TYPE_KIND_STRUCTURE: return "structure";
        default: return "unknown";
    }
}

/* 辅助函数：基本类型转字符串 */
static const char* basic_type_to_string(BasicType basic) {
    switch (basic) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        default: return "unknown";
    }
}

/* 类型转字符串（用于调试） */
char* type_to_string(Type* type) {
    if (type == NULL) {
        return strdup("null");
    }
    
    char buffer[256];
    buffer[0] = '\0';
    
    switch (type->kind) {
        case TYPE_KIND_BASIC:
            snprintf(buffer, sizeof(buffer), "%s", basic_type_to_string(type->u.basic));
            break;
            
        case TYPE_KIND_ARRAY: {
            char* elem_str = type_to_string(type->u.array.elem);
            snprintf(buffer, sizeof(buffer), "array[%d] of %s", type->u.array.size, elem_str);
            free(elem_str);
            break;
        }
            
        case TYPE_KIND_STRUCTURE: {
            strcat(buffer, "struct {");
            FieldList* field = type->u.structure;
            int first = 1;
            while (field != NULL) {
                if (!first) strcat(buffer, ", ");
                char* field_type_str = type_to_string(field->type);
                strcat(buffer, field->name);
                strcat(buffer, ": ");
                strcat(buffer, field_type_str);
                free(field_type_str);
                field = field->tail;
                first = 0;
            }
            strcat(buffer, "}");
            break;
        }
    }
    
    return strdup(buffer);
}