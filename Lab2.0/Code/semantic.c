#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


typedef struct Node {
    char* name;
    int line;
    int is_terminal;      /* 1: 终结符, 0: 非终结符 */
    union {
        struct {
            struct Node** children;
            int num_children;
        } nonterm;
        struct {
            char* value;   /* ID, STRING */
            int int_val;
            float float_val;
        } term;
    } u;
} Node;

/* 外部变量声明 */
extern Node* root;
extern int has_error;

/* 全局变量：当前语义分析上下文 */
static SemanticContext* g_context = NULL;

/* 初始化语义分析上下文 */
SemanticContext* init_semantic_context() {
    SemanticContext* context = (SemanticContext*)malloc(sizeof(SemanticContext));
    context->global_table = init_symbol_table(16384);  // 使用0x3fff大小
    context->current_table = context->global_table;    // 当前使用全局表
    context->current_return_type = NULL;
    context->error_count = 0;
    context->has_error = 0;
    return context;
}

/* 销毁语义分析上下文 */
void destroy_semantic_context(SemanticContext* context) {
    if (context == NULL) return;
    
    destroy_symbol_table(context->global_table);
    free(context);
}

/* 报告语义错误 */
void report_semantic_error(SemanticContext* context, int error_type, int line, const char* fmt, ...) {
    if (context == NULL) return;
    
    context->error_count++;
    context->has_error = 1;
    has_error = 1;  // 设置全局错误标志
    
    printf("Error type %d at Line %d: ", error_type, line);
    
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    
    printf(".\n");
}

/* 执行语义分析 */
void semantic_analysis(Node* root) {
    if (root == NULL) return;
    
    g_context = init_semantic_context();
    analyze_program(g_context, root);
    destroy_semantic_context(g_context);
    g_context = NULL;
}

/* 辅助函数：获取节点的子节点 */
static Node* get_child(Node* node, int index) {
    if (node == NULL || node->is_terminal || index < 0 || index >= node->u.nonterm.num_children) {
        return NULL;
    }
    return node->u.nonterm.children[index];
}

/* 辅助函数：获取节点的名称 */
static const char* get_node_name(Node* node) {
    if (node == NULL) return NULL;
    return node->name;
}

/* 辅助函数：获取节点的行号 */
static int get_node_line(Node* node) {
    if (node == NULL) return 0;
    return node->line;
}

/* 辅助函数：检查节点是否是特定名称 */
static int is_node_name(Node* node, const char* name) {
    if (node == NULL || name == NULL) return 0;
    return strcmp(node->name, name) == 0;
}

/* 辅助函数：获取ID节点的值 */
static char* get_id_value(Node* node) {
    if (node == NULL || !node->is_terminal || strcmp(node->name, "ID") != 0) {
        return NULL;
    }
    return node->u.term.value;
}

/* 辅助函数：获取INT节点的值 */
static int get_int_value(Node* node) {
    if (node == NULL || !node->is_terminal || strcmp(node->name, "INT") != 0) {
        return 0;
    }
    return node->u.term.int_val;
}

/* 分析Program节点 */
void analyze_program(SemanticContext* context, Node* node) {
    if (!is_node_name(node, "Program")) return;
    
    Node* ext_def_list = get_child(node, 0);
    if (ext_def_list != NULL) {
        analyze_ext_def_list(context, ext_def_list);
    }
}

/* 分析ExtDefList节点 */
void analyze_ext_def_list(SemanticContext* context, Node* node) {
    if (node == NULL) return;
    
    if (is_node_name(node, "ExtDefList")) {
        Node* ext_def = get_child(node, 0);
        Node* ext_def_list = get_child(node, 1);
        
        if (ext_def != NULL) {
            analyze_ext_def(context, ext_def);
        }
        if (ext_def_list != NULL) {
            analyze_ext_def_list(context, ext_def_list);
        }
    }
}

/* 分析ExtDef节点 */
void analyze_ext_def(SemanticContext* context, Node* node) {
    if (!is_node_name(node, "ExtDef")) return;
    
    int num_children = node->u.nonterm.num_children;
    if (num_children < 2) return;
    
    Node* specifier = get_child(node, 0);
    Type* base_type = NULL;
    
    // 分析类型说明符
    analyze_specifier(context, specifier, &base_type);
    if (base_type == NULL) return;
    
    Node* second_child = get_child(node, 1);
    if (is_node_name(second_child, "ExtDecList")) {
        // 变量定义：Specifier ExtDecList SEMI
        // TODO: 实现变量定义分析
    } else if (is_node_name(second_child, "SEMI")) {
        // 类型声明：Specifier SEMI
        // 不做特殊处理
    } else if (is_node_name(second_child, "FunDec")) {
        // 函数定义：Specifier FunDec CompSt 或 Specifier FunDec SEMI
        Node* fun_dec = second_child;
        Node* third_child = get_child(node, 2);
        
        // 分析函数声明
        analyze_fun_dec(context, fun_dec, base_type);
        
        if (is_node_name(third_child, "CompSt")) {
            // 函数定义体
            context->current_return_type = base_type;
            analyze_comp_st(context, third_child, base_type);
            context->current_return_type = NULL;
        }
        // 如果是函数声明（SEMI），不需要分析函数体
    }
}

/* 分析Specifier节点 */
void analyze_specifier(SemanticContext* context, Node* node, Type** type) {
    if (node == NULL || type == NULL) return;
    
    if (is_node_name(node, "Specifier")) {
        Node* child = get_child(node, 0);
        if (child != NULL) {
            if (is_node_name(child, "TYPE")) {
                // 基本类型：int 或 float
                const char* type_name = child->u.term.value;
                if (strcmp(type_name, "int") == 0) {
                    *type = new_type_basic(TYPE_INT);
                } else if (strcmp(type_name, "float") == 0) {
                    *type = new_type_basic(TYPE_FLOAT);
                }
            } else if (is_node_name(child, "StructSpecifier")) {
                // 结构体类型
                analyze_struct_specifier(context, child, type);
            }
        }
    }
}

/* 分析StructSpecifier节点 */
void analyze_struct_specifier(SemanticContext* context, Node* node, Type** type) {
    if (node == NULL || type == NULL) return;
    
    if (is_node_name(node, "StructSpecifier")) {
        int num_children = node->u.nonterm.num_children;
        
        if (num_children == 2) {
            // STRUCT Tag
            Node* struct_node = get_child(node, 0);
            Node* tag_node = get_child(node, 1);
            
            if (is_node_name(tag_node, "Tag")) {
                Node* id_node = get_child(tag_node, 0);
                char* struct_name = get_id_value(id_node);
                
                if (struct_name != NULL) {
                    // 查找已定义的结构体
                    Symbol* symbol = find_symbol(context->global_table, struct_name);
                    if (symbol != NULL && symbol->kind == SYMBOL_STRUCT) {
                        *type = copy_type(symbol->u.structure.type);
                    } else {
                        // 结构体未定义
                        report_semantic_error(context, ERROR_UNDEFINED_STRUCTURE, 
                                            get_node_line(node), 
                                            "Undefined structure \"%s\"", struct_name);
                        *type = NULL;
                    }
                }
            }
        } else if (num_children == 5) {
            // STRUCT OptTag LC DefList RC
            Node* opt_tag = get_child(node, 1);
            Node* def_list = get_child(node, 3);
            
            char* struct_name = NULL;
            if (opt_tag != NULL && !is_node_name(opt_tag, "empty")) {
                Node* id_node = get_child(opt_tag, 0);
                struct_name = get_id_value(id_node);
                
                // 检查结构体名是否重复
                if (struct_name != NULL) {
                    Symbol* existing = find_symbol(context->global_table, struct_name);
                    if (existing != NULL) {
                        report_semantic_error(context, ERROR_REDEFINED_STRUCTURE,
                                            get_node_line(node),
                                            "Duplicated name \"%s\"", struct_name);
                        // 继续分析，但标记错误
                    }
                }
            }
            
            // 分析结构体定义
            FieldList* fields = NULL;
            FieldList* last_field = NULL;
            
            // 临时符号表用于检查域重复定义
            SymbolTable* field_table = init_symbol_table(64);
            
            // 分析DefList获取域列表
            Node* current = def_list;
            while (current != NULL && is_node_name(current, "DefList")) {
                Node* def = get_child(current, 0);
                if (def != NULL) {
                    // 分析单个定义
                    Node* specifier = get_child(def, 0);
                    Node* dec_list = get_child(def, 1);
                    
                    Type* field_base_type = NULL;
                    analyze_specifier(context, specifier, &field_base_type);
                    
                    // 分析DecList
                    Node* dec_list_node = dec_list;
                    while (dec_list_node != NULL && is_node_name(dec_list_node, "DecList")) {
                        Node* dec = get_child(dec_list_node, 0);
                        if (dec != NULL) {
                            char* field_name = NULL;
                            Type* field_type = NULL;
                            
                            // 分析VarDec获取域名和类型
                            Node* var_dec = get_child(dec, 0);
                            if (var_dec != NULL) {
                                // TODO: 实现analyze_var_dec_for_field
                                // 简化：假设是简单变量
                                if (is_node_name(var_dec, "VarDec")) {
                                    Node* id_node = get_child(var_dec, 0);
                                    if (id_node != NULL && is_node_name(id_node, "ID")) {
                                        field_name = get_id_value(id_node);
                                        field_type = copy_type(field_base_type);
                                    }
                                }
                            }
                            
                            if (field_name != NULL && field_type != NULL) {
                                // 检查域名是否重复
                                if (find_symbol(field_table, field_name) != NULL) {
                                    report_semantic_error(context, ERROR_REDEFINED_FIELD,
                                                        get_node_line(dec),
                                                        "Redefined field \"%s\"", field_name);
                                } else {
                                    // 添加到域表
                                    Symbol* field_symbol = new_variable_symbol(field_name, field_type, get_node_line(dec));
                                    insert_symbol(field_table, field_symbol);
                                    
                                    // 添加到域列表（需要类型副本，因为field_type会被field_table管理）
                                    FieldList* new_field = new_field_list(field_name, copy_type(field_type), get_node_line(dec));
                                    if (fields == NULL) {
                                        fields = new_field;
                                        last_field = new_field;
                                    } else {
                                        last_field->tail = new_field;
                                        last_field = new_field;
                                    }
                                }
                            }
                        }
                        
                        // 下一个Dec
                        if (dec_list_node->u.nonterm.num_children > 1) {
                            dec_list_node = get_child(dec_list_node, 2);
                        } else {
                            dec_list_node = NULL;
                        }
                    }
                }
                
                // 下一个Def
                if (current->u.nonterm.num_children > 1) {
                    current = get_child(current, 1);
                } else {
                    current = NULL;
                }
            }
            
            destroy_symbol_table(field_table);
            
            // 创建结构体类型
            Type* struct_type = new_type_structure(fields);
            
            // 如果有结构体名，添加到符号表
            if (struct_name != NULL) {
                Symbol* struct_symbol = new_struct_symbol(struct_name, struct_type, get_node_line(node));
                insert_symbol(context->global_table, struct_symbol);
            }
            
            // 返回给调用者的类型应该是副本，因为符号表会管理原始类型的内存
            *type = copy_type(struct_type);
        }
    }
}

/* 分析VarDec节点（返回变量名和完整类型） */
void analyze_var_dec(SemanticContext* context, Node* node, Type* base_type, char** name) {
    if (node == NULL || base_type == NULL || name == NULL) return;
    
    if (is_node_name(node, "VarDec")) {
        Node* first_child = get_child(node, 0);
        
        if (is_node_name(first_child, "ID")) {
            // 简单变量：ID
            *name = get_id_value(first_child);
            // 类型就是base_type
        } else if (is_node_name(first_child, "VarDec")) {
            // 数组变量：VarDec LB INT_CONST RB
            Node* var_dec = first_child;
            Node* int_const = get_child(node, 2);
            
            char* var_name = NULL;
            analyze_var_dec(context, var_dec, base_type, &var_name);
            
            if (var_name != NULL && int_const != NULL) {
                int array_size = get_int_value(int_const);
                Type* array_type = new_type_array(copy_type(base_type), array_size);
                *name = var_name;
                // 注意：这里需要返回数组类型，但函数签名只返回名称
                // 实际类型应该在调用处处理
            }
        }
    }
}

/* 分析FunDec节点 */
void analyze_fun_dec(SemanticContext* context, Node* node, Type* return_type) {
    if (!is_node_name(node, "FunDec")) return;
    
    int num_children = node->u.nonterm.num_children;
    if (num_children < 1) return;
    
    Node* id_node = get_child(node, 0);
    char* func_name = get_id_value(id_node);
    if (func_name == NULL) return;
    
    // 检查函数是否已定义
    Symbol* existing = find_symbol(context->global_table, func_name);
    if (existing != NULL) {
        if (existing->kind == SYMBOL_FUNCTION) {
            report_semantic_error(context, ERROR_REDEFINED_FUNCTION,
                                get_node_line(node),
                                "Redefined function \"%s\"", func_name);
        } else {
            report_semantic_error(context, ERROR_REDEFINED_VARIABLE,
                                get_node_line(node),
                                "Redefined variable \"%s\"", func_name);
        }
        return;
    }
    
    ParamList* params = NULL;
    ParamList* last_param = NULL;
    
    if (num_children == 4) {
        // 有参数列表：ID LP VarList RP
        Node* var_list = get_child(node, 2);
        
        // 分析参数列表
        Node* current = var_list;
        while (current != NULL && is_node_name(current, "VarList")) {
            Node* param_dec = get_child(current, 0);
            
            if (param_dec != NULL && is_node_name(param_dec, "ParamDec")) {
                ParamList* param = NULL;
                analyze_param_dec(context, param_dec, &param);
                
                if (param != NULL) {
                    if (params == NULL) {
                        params = param;
                        last_param = param;
                    } else {
                        last_param->next = param;
                        last_param = param;
                    }
                }
            }
            
            // 下一个参数
            if (current->u.nonterm.num_children > 1) {
                current = get_child(current, 2);
            } else {
                current = NULL;
            }
        }
    }
    // 如果num_children == 3，则是无参函数：ID LP RP
    
    // 创建函数符号
    Symbol* func_symbol = new_function_symbol(func_name, return_type, params, get_node_line(node));
    insert_symbol(context->global_table, func_symbol);
}

/* 分析ParamDec节点 */
void analyze_param_dec(SemanticContext* context, Node* node, ParamList** param) {
    if (!is_node_name(node, "ParamDec") || param == NULL) return;
    
    Node* specifier = get_child(node, 0);
    Node* var_dec = get_child(node, 1);
    
    Type* param_type = NULL;
    analyze_specifier(context, specifier, &param_type);
    
    if (param_type != NULL && var_dec != NULL) {
        char* param_name = NULL;
        analyze_var_dec(context, var_dec, param_type, &param_name);
        
        if (param_name != NULL) {
            *param = new_param_list(param_name, param_type);
            
            // 同时将参数作为变量添加到当前符号表
            Symbol* var_symbol = new_variable_symbol(param_name, copy_type(param_type), get_node_line(var_dec));
            insert_symbol(context->current_table, var_symbol);
        }
    }
}

/* 分析CompSt节点 */
void analyze_comp_st(SemanticContext* context, Node* node, Type* return_type) {
    if (!is_node_name(node, "CompSt")) return;
    
    // CompSt: LC DefList StmtList RC
    Node* def_list = get_child(node, 1);
    Node* stmt_list = get_child(node, 2);
    
    // 分析局部定义
    if (def_list != NULL) {
        analyze_def_list(context, def_list);
    }
    
    // 分析语句列表
    if (stmt_list != NULL) {
        analyze_stmt_list(context, stmt_list);
    }
}

/* 分析StmtList节点 */
void analyze_stmt_list(SemanticContext* context, Node* node) {
    if (node == NULL) return;
    
    if (is_node_name(node, "StmtList")) {
        Node* stmt = get_child(node, 0);
        Node* stmt_list = get_child(node, 1);
        
        if (stmt != NULL) {
            analyze_stmt(context, stmt);
        }
        if (stmt_list != NULL) {
            analyze_stmt_list(context, stmt_list);
        }
    }
}

/* 分析Stmt节点 */
void analyze_stmt(SemanticContext* context, Node* node) {
    if (node == NULL) return;
    
    if (!is_node_name(node, "Stmt")) {
        return;
    }
    
    int num_children = node->u.nonterm.num_children;
    if (num_children == 0) {
        return;
    }
    
    Node* first_child = get_child(node, 0);
    
    // 处理表达式语句: Exp SEMI
    if (num_children == 2 && is_node_name(first_child, "Exp")) {
        Node* semi_node = get_child(node, 1);
        if (semi_node != NULL && is_node_name(semi_node, "SEMI")) {
            // 分析表达式（类型检查会自动进行）
            analyze_exp(context, first_child);
        }
    }
    // 处理RETURN语句: RETURN Exp SEMI
    else if (num_children == 3 && is_node_name(first_child, "RETURN")) {
        Node* exp_node = get_child(node, 1);
        Node* semi_node = get_child(node, 2);
        
        if (exp_node != NULL && is_node_name(exp_node, "Exp") &&
            semi_node != NULL && is_node_name(semi_node, "SEMI")) {
            
            ExpTypeInfo exp_info = analyze_exp(context, exp_node);
            
            // 检查返回类型
            if (context->current_return_type != NULL && exp_info.type != NULL) {
                if (!is_type_compatible(context->current_return_type, exp_info.type)) {
                    report_semantic_error(context, ERROR_TYPE_MISMATCH_RETURN,
                                        get_node_line(node),
                                        "Type mismatched for return");
                }
            }
        }
    }
}

/* 分析DefList节点 */
void analyze_def_list(SemanticContext* context, Node* node) {
    if (node == NULL) return;
    
    if (is_node_name(node, "DefList")) {
        Node* def = get_child(node, 0);
        Node* def_list = get_child(node, 1);
        
        if (def != NULL) {
            analyze_def(context, def);
        }
        if (def_list != NULL) {
            analyze_def_list(context, def_list);
        }
    }
}

/* 分析Def节点 */
void analyze_def(SemanticContext* context, Node* node) {
    if (!is_node_name(node, "Def")) return;
    
    // Def: Specifier DecList SEMI
    if (node->u.nonterm.num_children != 3) return;
    
    Node* specifier = get_child(node, 0);
    Node* dec_list = get_child(node, 1);
    // SEMI节点不需要处理
    
    // 分析类型说明符
    Type* base_type = NULL;
    analyze_specifier(context, specifier, &base_type);
    
    if (base_type != NULL && dec_list != NULL) {
        // 分析声明列表
        analyze_dec_list(context, dec_list, base_type);
    }
}

/* 分析DecList节点 */
void analyze_dec_list(SemanticContext* context, Node* node, Type* base_type) {
    if (node == NULL || base_type == NULL) return;
    
    if (is_node_name(node, "DecList")) {
        Node* dec = get_child(node, 0);
        Node* next_dec_list = NULL;
        
        if (node->u.nonterm.num_children == 3) {
            // Dec COMMA DecList
            next_dec_list = get_child(node, 2);
        }
        
        // 分析当前声明
        if (dec != NULL) {
            analyze_dec(context, dec, base_type);
        }
        
        // 分析后续声明
        if (next_dec_list != NULL) {
            analyze_dec_list(context, next_dec_list, base_type);
        }
    }
}

/* 分析Dec节点 */
void analyze_dec(SemanticContext* context, Node* node, Type* base_type) {
    if (!is_node_name(node, "Dec") || base_type == NULL) return;
    
    // Dec: VarDec 或 VarDec ASSIGNOP Exp
    if (node->u.nonterm.num_children < 1) return;
    
    Node* var_dec = get_child(node, 0);
    
    if (var_dec != NULL) {
        char* var_name = NULL;
        Type* var_type = copy_type(base_type);
        
        // 分析变量声明（可能包含数组维度）
        analyze_var_dec(context, var_dec, base_type, &var_name);
        
        if (var_name != NULL) {
            // 检查变量是否已定义
            Symbol* existing = find_symbol(context->global_table, var_name);
            if (existing != NULL) {
                report_semantic_error(context, ERROR_REDEFINED_VARIABLE,
                                    get_node_line(node),
                                    "Redefined variable \"%s\"", var_name);
            } else {
                // 添加到符号表
                Symbol* var_symbol = new_variable_symbol(var_name, var_type, get_node_line(node));
                insert_symbol(context->global_table, var_symbol);
            }
        }
        
        // 如果有初始化表达式，分析它
        if (node->u.nonterm.num_children == 3) {
            Node* assignop = get_child(node, 1);
            Node* exp = get_child(node, 2);
            
            if (assignop != NULL && is_node_name(assignop, "ASSIGNOP") && exp != NULL) {
                // 分析初始化表达式
                ExpTypeInfo exp_info = analyze_exp(context, exp);
                
                // 检查类型兼容性
                if (var_type != NULL && exp_info.type != NULL) {
                    if (!is_type_compatible(var_type, exp_info.type)) {
                        report_semantic_error(context, ERROR_TYPE_MISMATCH_ASSIGNMENT,
                                            get_node_line(node),
                                            "Type mismatched for assignment");
                    }
                }
            }
        }
    }
}

/* 分析Exp节点 */
ExpTypeInfo analyze_exp(SemanticContext* context, Node* node) {
    ExpTypeInfo info = {NULL, 0, 0};
    
    if (node == NULL) return info;
    
    info.line = get_node_line(node);
    
    if (!is_node_name(node, "Exp")) {
        return info;
    }
    
    int num_children = node->u.nonterm.num_children;
    if (num_children == 0) {
        return info;
    }
    
    Node* first_child = get_child(node, 0);
    
    // 处理二元形式（可能是赋值或其它二元运算）: Exp OP Exp
    if (num_children == 3 && is_node_name(first_child, "Exp")) {
        Node* op_node = get_child(node, 1);
        Node* right_exp = get_child(node, 2);

        if (op_node != NULL && is_node_name(op_node, "ASSIGNOP")) {
            // 赋值表达式
            ExpTypeInfo left_info = analyze_exp(context, first_child);

            // 检查左值
            if (!left_info.is_lvalue) {
                report_semantic_error(context, ERROR_NON_LVALUE_ASSIGNMENT,
                                    info.line, "The left-hand side of an assignment must be a variable");
            }

            // 分析右表达式
            ExpTypeInfo right_info = analyze_exp(context, right_exp);

            // 检查类型兼容性
            if (left_info.type != NULL && right_info.type != NULL) {
                if (!is_type_compatible(left_info.type, right_info.type)) {
                    report_semantic_error(context, ERROR_TYPE_MISMATCH_ASSIGNMENT,
                                        info.line, "Type mismatched for assignment");
                }
            }

            // 赋值表达式的结果类型是左表达式的类型
            info.type = copy_type(left_info.type);
            info.is_lvalue = 0;  // 赋值表达式本身不是左值
        }
        else if (op_node != NULL && op_node->is_terminal) {
            // 其它二元操作符：算术/关系/逻辑等
            const char* op_name = op_node->name;
            if (strcmp(op_name, "PLUS") == 0 || strcmp(op_name, "MINUS") == 0 ||
                strcmp(op_name, "STAR") == 0 || strcmp(op_name, "DIV") == 0 ||
                strcmp(op_name, "MOD") == 0 ||
                strcmp(op_name, "LT") == 0 || strcmp(op_name, "LE") == 0 ||
                strcmp(op_name, "GT") == 0 || strcmp(op_name, "GE") == 0 ||
                strcmp(op_name, "EQ") == 0 || strcmp(op_name, "NE") == 0 ||
                strcmp(op_name, "AND") == 0 || strcmp(op_name, "OR") == 0) {

                // 分析左表达式
                ExpTypeInfo left_info = analyze_exp(context, first_child);
                // 分析右表达式
                ExpTypeInfo right_info = analyze_exp(context, right_exp);

                // 检查类型兼容性
                if (left_info.type != NULL && right_info.type != NULL) {
                    if (!is_type_compatible(left_info.type, right_info.type)) {
                        report_semantic_error(context, ERROR_TYPE_MISMATCH_OPERANDS,
                                            info.line, "Type mismatched for operands");
                    }
                }

                // 设置结果类型（简化：使用左操作数的类型）
                if (left_info.type != NULL) {
                    info.type = copy_type(left_info.type);
                }
                info.is_lvalue = 0;  // 二元表达式不是左值
            }
        }
    }
    // 处理ID节点
    else if (num_children == 1 && is_node_name(first_child, "ID")) {
        char* id_name = get_id_value(first_child);
        if (id_name != NULL) {
            Symbol* symbol = find_symbol(context->global_table, id_name);
            if (symbol == NULL) {
                report_semantic_error(context, ERROR_UNDEFINED_VARIABLE,info.line, "Undefined variable \"%s\"", id_name);
                info.is_lvalue = 1;
            } else if (symbol->kind == SYMBOL_VARIABLE) {
                info.type = copy_type(symbol->u.variable.type);
                info.is_lvalue = 1;
            }
        }
    }
    // 处理其他表达式类型（简化：暂时返回空类型）
    
    return info;
}
