#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* strdup may not be declared depending on feature macros; declare it to avoid implicit-declaration warnings */
extern char* strdup(const char*);


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

/* 辅助函数：获取FLOAT节点的值 */
static float get_float_value(Node* node) {
    if (node == NULL || !node->is_terminal || strcmp(node->name, "FLOAT") != 0) {
        return 0.0f;
    }
    return node->u.term.float_val;
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
        Node* extdec = second_child;
        while (extdec != NULL && is_node_name(extdec, "ExtDecList")) {
            Node* var_dec = get_child(extdec, 0);
            if (var_dec != NULL && is_node_name(var_dec, "VarDec")) {
                char* var_name = NULL;
                Type* var_type = NULL;
                analyze_var_dec(context, var_dec, base_type, &var_name, &var_type);
                if (var_name != NULL) {
                    Symbol* existing = find_symbol(context->global_table, var_name);
                    if (existing != NULL) {
                        report_semantic_error(context, ERROR_REDEFINED_VARIABLE,
                                            get_node_line(var_dec),
                                            "Redefined variable \"%s\"", var_name);
                    } else {
                        // 使用 var_type（包含数组维度等完整类型信息），如果为 NULL 则回退到 base_type
                        Type* final_type = (var_type != NULL) ? var_type : copy_type(base_type);
                        Symbol* var_symbol = new_variable_symbol(var_name, final_type, get_node_line(var_dec));
                        insert_symbol(context->global_table, var_symbol);
                    }
                }
            }

            if (extdec->u.nonterm.num_children > 1) extdec = get_child(extdec, 2);
            else extdec = NULL;
        }
    } else if (is_node_name(second_child, "SEMI")) {
        // 类型声明：Specifier SEMI
        // 不做特殊处理
    } else if (is_node_name(second_child, "FunDec")) {
        // 函数定义：Specifier FunDec CompSt 或 Specifier FunDec SEMI
        Node* fun_dec = second_child;
        Node* third_child = get_child(node, 2);

        // 如果是函数声明（SEMI），根据 requirements 不支持函数声明，报告错误类型 B
        if (third_child != NULL && is_node_name(third_child, "SEMI")) {
            /* Report incomplete definition as Error type B at the declaration line */
            Node* id_node = get_child(fun_dec, 0);
            char* func_name = get_id_value(id_node);
            int line = get_node_line(node);
            /* set global syntax error flags similar to parser's behavior */
            extern int has_syntax_error;
            has_error = 1;
            has_syntax_error = 1;
            if (func_name != NULL) {
                printf("Error type B at Line %d: Incomplete definition of function \"%s\".\n", line, func_name);
            } else {
                printf("Error type B at Line %d: Incomplete definition of function.\n", line);
            }
        } else {
            // 正常的函数定义：先分析函数声明并插入符号，再分析函数体
            analyze_fun_dec(context, fun_dec, base_type);

            if (is_node_name(third_child, "CompSt")) {
                // 函数定义体
                context->current_return_type = base_type;
                analyze_comp_st(context, third_child, base_type);
                context->current_return_type = NULL;
            }
        }
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
                            
                            // 分析VarDec获取域名和完整类型（包括数组维度）
                            Node* var_dec = get_child(dec, 0);
                            if (var_dec != NULL && is_node_name(var_dec, "VarDec")) {
                                Type* full_field_type = NULL;
                                analyze_var_dec(context, var_dec, field_base_type, &field_name, &full_field_type);
                                if (field_name != NULL) {
                                    // 如果 analyze_var_dec 返回了完整类型（如数组），使用之；否则使用基类型副本
                                    field_type = (full_field_type != NULL) ? full_field_type : copy_type(field_base_type);
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
void analyze_var_dec(SemanticContext* context, Node* node, Type* base_type, char** name, Type** type) {
    if (node == NULL || base_type == NULL || name == NULL || type == NULL) return;
    
    // 初始化返回值为NULL
    *name = NULL;
    *type = NULL;
    
    if (is_node_name(node, "VarDec")) {
        Node* first_child = get_child(node, 0);
        
        if (is_node_name(first_child, "ID")) {
            // 简单变量：ID
            *name = get_id_value(first_child);
            *type = copy_type(base_type);
        } else if (is_node_name(first_child, "VarDec")) {
            // 数组变量：VarDec LB INT_CONST RB
            Node* var_dec = first_child;
            Node* int_const = get_child(node, 2);
            
            char* var_name = NULL;
            Type* elem_type = NULL;
            analyze_var_dec(context, var_dec, base_type, &var_name, &elem_type);
            
            if (var_name != NULL && int_const != NULL) {
                int array_size = get_int_value(int_const);
                // 如果elem_type不为NULL，说明是嵌套数组（多维数组）
                // 否则使用base_type作为元素类型
                Type* element_type = (elem_type != NULL) ? elem_type : copy_type(base_type);
                Type* array_type = new_type_array(element_type, array_size);
                *name = var_name;
                *type = array_type;
            } else if (var_name != NULL) {
                // 没有INT_CONST？不应该发生，但处理一下
                *name = var_name;
                *type = copy_type(base_type);
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
        Type* param_full_type = NULL;
        analyze_var_dec(context, var_dec, param_type, &param_name, &param_full_type);
        
        if (param_name != NULL) {
            // 使用完整的类型（包括数组维度）
            Type* actual_type = (param_full_type != NULL) ? param_full_type : copy_type(param_type);
            *param = new_param_list(param_name, actual_type);
            
            // 同时将参数作为变量添加到当前符号表
            Symbol* var_symbol = new_variable_symbol(param_name, copy_type(actual_type), get_node_line(var_dec));
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
    // 处理IF语句: IF LP Exp RP Stmt 或 IF LP Exp RP MatchedStmt ELSE MatchedStmt
    else if (num_children >= 5 && is_node_name(first_child, "IF")) {
        // 第三个子节点是条件表达式 (索引2: IF, LP, Exp, RP, ...)
        Node* exp_node = get_child(node, 2);
        if (exp_node != NULL && is_node_name(exp_node, "Exp")) {
            ExpTypeInfo exp_info = analyze_exp(context, exp_node);
            
            // 检查条件表达式类型是否为int
            if (exp_info.type != NULL) {
                if (!(exp_info.type->kind == TYPE_KIND_BASIC && exp_info.type->u.basic == TYPE_INT)) {
                    report_semantic_error(context, ERROR_TYPE_MISMATCH_OPERANDS,
                                        get_node_line(exp_node),
                                        "Condition expression must be int type");
                }
            }
        }
        
        // 分析then分支 (索引4)
        Node* then_stmt = get_child(node, 4);
        if (then_stmt != NULL) {
            analyze_stmt(context, then_stmt);
        }
        
        // 如果有else分支 (7个子节点)，分析else分支 (索引6)
        if (num_children == 7) {
            Node* else_stmt = get_child(node, 6);
            if (else_stmt != NULL) {
                analyze_stmt(context, else_stmt);
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
        Type* var_type = NULL;
        
        // 分析变量声明（可能包含数组维度）
        analyze_var_dec(context, var_dec, base_type, &var_name, &var_type);
        
        if (var_name != NULL) {
            // 如果var_type为NULL，使用base_type作为后备
            if (var_type == NULL) {
                var_type = copy_type(base_type);
            }
            
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
    
    // 调试输出（已注释）
    // printf("DEBUG analyze_exp: node name=%s, num_children=%d, first_child name=%s\n",
    //        node->name, num_children, first_child ? first_child->name : "NULL");
    
    // 处理数组访问：Exp LB Exp RB
    if (num_children == 4 && is_node_name(get_child(node, 1), "LB") && is_node_name(get_child(node, 3), "RB")) {
        Node* left = get_child(node, 0);
        Node* index = get_child(node, 2);

        ExpTypeInfo left_info = analyze_exp(context, left);
        ExpTypeInfo idx_info = analyze_exp(context, index);

        if (left_info.type == NULL || left_info.type->kind != TYPE_KIND_ARRAY) {
            /* 如果左侧是简单标识符，使用标识符名字并报告该标识符所在行；否则保持原有通用信息 */
            char* id_name = NULL;
            Node* id_probe = NULL;
            /* 尝试向下寻找最内层的 ID 终结符以获得变量名和确切行号 */
            Node* probe = left;
            while (probe != NULL) {
                if (probe->is_terminal && strcmp(probe->name, "ID") == 0) { id_probe = probe; break; }
                if (!probe->is_terminal && probe->u.nonterm.num_children > 0) {
                    probe = get_child(probe, 0);
                } else break;
            }
            if (id_probe != NULL) id_name = get_id_value(id_probe);
            if (id_name != NULL) {
                report_semantic_error(context, ERROR_NON_ARRAY_SUBSCRIPT, get_node_line(id_probe), "\"%s\" is not an array", id_name);
            } else {
                report_semantic_error(context, ERROR_NON_ARRAY_SUBSCRIPT, info.line, "Not an array");
            }
            return info;
        }

        // check index is integer
        if (idx_info.type == NULL || !(idx_info.type->kind == TYPE_KIND_BASIC && idx_info.type->u.basic == TYPE_INT)) {
            // 尝试获取下标表达式的文本表示
            char* subscript_text = NULL;
            if (index != NULL) {
                // 如果是常量节点，尝试获取其值
                if (index->is_terminal) {
                    const char* tname = index->name;
                    if (strcmp(tname, "INT") == 0 || strcmp(tname, "INT_CONST") == 0) {
                        int int_val = get_int_value(index);
                        subscript_text = (char*)malloc(32);
                        snprintf(subscript_text, 32, "%d", int_val);
                    } else if (strcmp(tname, "FLOAT") == 0 || strcmp(tname, "FLOAT_CONST") == 0) {
                        float float_val = get_float_value(index);
                        subscript_text = (char*)malloc(32);
                        snprintf(subscript_text, 32, "%.1f", float_val);
                    } else if (strcmp(tname, "ID") == 0) {
                        subscript_text = get_id_value(index);
                    }
                }
            }
            
            if (subscript_text != NULL) {
                report_semantic_error(context, ERROR_NON_INTEGER_SUBSCRIPT, info.line, "\"%s\" is not an integer", subscript_text);
                // 如果subscript_text是动态分配的，需要释放
                if (subscript_text != get_id_value(index)) { // 不是ID的字符串（是动态分配的）
                    free(subscript_text);
                }
            } else {
                report_semantic_error(context, ERROR_NON_INTEGER_SUBSCRIPT, info.line, "Array subscript is not an integer");
            }
            return info;
        }

        // element type is the array element type
        info.type = copy_type(left_info.type->u.array.elem);
        info.is_lvalue = 1;
        return info;
    }

    // 处理结构体成员访问：Exp DOT ID
    if (num_children == 3 && is_node_name(get_child(node, 1), "DOT")) {
        Node* left = get_child(node, 0);
        Node* dot_node = get_child(node, 1);
        Node* field_node = get_child(node, 2);
        
        ExpTypeInfo left_info = analyze_exp(context, left);
        
        // 检查左侧表达式是否是结构体类型
        if (left_info.type == NULL || left_info.type->kind != TYPE_KIND_STRUCTURE) {
            /* 如果左侧是简单标识符，使用标识符名字并报告该标识符所在行；否则保持原有通用信息 */
            char* id_name = NULL;
            Node* id_probe = NULL;
            /* 尝试向下寻找最内层的 ID 终结符以获得变量名和确切行号 */
            Node* probe = left;
            while (probe != NULL) {
                if (probe->is_terminal && strcmp(probe->name, "ID") == 0) { id_probe = probe; break; }
                if (!probe->is_terminal && probe->u.nonterm.num_children > 0) {
                    probe = get_child(probe, 0);
                } else break;
            }
            if (id_probe != NULL) id_name = get_id_value(id_probe);
            if (id_name != NULL) {
                report_semantic_error(context, ERROR_NON_STRUCTURE_DOT, get_node_line(id_probe), "\"%s\" is not a structure", id_name);
            } else {
                report_semantic_error(context, ERROR_NON_STRUCTURE_DOT, info.line, "Not a structure");
            }
            return info;
        }
        
        // 获取域名
        char* field_name = NULL;
        if (field_node != NULL && is_node_name(field_node, "ID")) {
            field_name = get_id_value(field_node);
        }
        
        if (field_name == NULL) {
            // 不应该发生，但处理一下
            return info;
        }
        
        // 在结构体域中查找该域名
        FieldList* field = left_info.type->u.structure;
        Type* field_type = NULL;
        int found = 0;
        
        while (field != NULL) {
            if (field->name != NULL && strcmp(field->name, field_name) == 0) {
                field_type = field->type;
                found = 1;
                break;
            }
            field = field->tail;
        }
        
        if (found == 0) {
            // 报告未定义的域错误（错误类型14）
            report_semantic_error(context, ERROR_UNDEFINED_FIELD, info.line,
                                "Structure has no field named \"%s\"", field_name);
            return info;
        }
        
        // 结构体成员访问是左值
        info.type = copy_type(field_type);
        info.is_lvalue = 1;
        return info;
    }

    // 处理二元形式（可能是赋值或其它二元运算）: Exp OP Exp
    if (num_children == 3 && is_node_name(first_child, "Exp")) {
        Node* op_node = get_child(node, 1);
        Node* right_exp = get_child(node, 2);

        if (op_node != NULL && is_node_name(op_node, "ASSIGNOP")) {
            // 赋值表达式
            ExpTypeInfo left_info = analyze_exp(context, first_child);

            // 检查左值：仅在左侧类型已知且不是左值时报告（避免在左侧已产生其它错误时重复报告）
            if (!left_info.is_lvalue && left_info.type != NULL) {
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
            // printf("DEBUG: Binary op name = %s\n", op_name);
            if (strcmp(op_name, "PLUS") == 0 || strcmp(op_name, "MINUS") == 0 ||
                strcmp(op_name, "STAR") == 0 || strcmp(op_name, "DIV") == 0 ||
                strcmp(op_name, "MOD") == 0 ||
                strcmp(op_name, "LT") == 0 || strcmp(op_name, "LE") == 0 ||
                strcmp(op_name, "GT") == 0 || strcmp(op_name, "GE") == 0 ||
                strcmp(op_name, "EQ") == 0 || strcmp(op_name, "NE") == 0 ||
                strcmp(op_name, "AND") == 0 || strcmp(op_name, "OR") == 0 ||
                strcmp(op_name, "RELOP") == 0) {

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
    // 处理整型/浮点/字符串常量
    else if (num_children == 1 && first_child->is_terminal) {
        const char* tname = first_child->name;
        if (strcmp(tname, "INT") == 0 || strcmp(tname, "INT_CONST") == 0) {
            info.type = new_type_basic(TYPE_INT);
            info.is_lvalue = 0;
        } else if (strcmp(tname, "FLOAT") == 0 || strcmp(tname, "FLOAT_CONST") == 0) {
            info.type = new_type_basic(TYPE_FLOAT);
            info.is_lvalue = 0;
        } else if (strcmp(tname, "STRING") == 0) {
            // strings are not used in type checks in this lab, leave as NULL or handle as needed
            info.type = NULL;
            info.is_lvalue = 0;
        }
    }
    // 处理函数调用：ID LP RP 或 ID LP Args RP
    else if (is_node_name(first_child, "ID") && num_children >= 3) {
        // Expect forms: ID LP RP (3 children) or ID LP Args RP (4 children)
        Node* second = get_child(node, 1);
        if (second != NULL && is_node_name(second, "LP")) {
            char* func_name = get_id_value(first_child);
            if (func_name != NULL) {
                Symbol* symbol = find_symbol(context->global_table, func_name);
                if (symbol == NULL) {
                    report_semantic_error(context, ERROR_UNDEFINED_FUNCTION, info.line, "Undefined function \"%s\"", func_name);
                } else if (symbol->kind != SYMBOL_FUNCTION) {
                    report_semantic_error(context, ERROR_NON_FUNCTION_CALL, info.line, "\"%s\" is not a function", func_name);
                } else {
                    // symbol is a function; check arguments if present
                    ParamList* param = symbol->u.function.params;
                    if (num_children == 4) {
                        // First, collect actual argument types (and analyze each arg expression)
                        Node* args = get_child(node, 2);
                        Node* cur = args;
                        int arg_count = 0;
                        char** arg_type_strs = NULL;

                        while (cur != NULL && is_node_name(cur, "Args")) {
                            Node* arg_exp = get_child(cur, 0);
                            ExpTypeInfo arg_info = analyze_exp(context, arg_exp);

                            char* tstr = NULL;
                            if (arg_info.type != NULL) {
                                tstr = type_to_string(arg_info.type);
                            } else {
                                tstr = strdup("null");
                            }

                            arg_type_strs = (char**)realloc(arg_type_strs, sizeof(char*) * (arg_count + 1));
                            arg_type_strs[arg_count] = tstr;
                            arg_count++;

                            if (cur->u.nonterm.num_children > 1) cur = get_child(cur, 2);
                            else cur = NULL;
                        }

                        // Build parameter type list string
                        int param_count = 0;
                        ParamList* ptmp = symbol->u.function.params;
                        char** param_type_strs = NULL;
                        while (ptmp != NULL) {
                            char* pts = NULL;
                            if (ptmp->type != NULL) pts = type_to_string(ptmp->type);
                            else pts = strdup("null");
                            param_type_strs = (char**)realloc(param_type_strs, sizeof(char*) * (param_count + 1));
                            param_type_strs[param_count] = pts;
                            param_count++;
                            ptmp = ptmp->next;
                        }

                        // Compare counts and types
                        int mismatch = 0;
                        if (arg_count != param_count) mismatch = 1;
                        else {
                            for (int i = 0; i < arg_count; ++i) {
                                // reconstruct Type from param_type_strs isn't straightforward; use is_type_compatible instead
                                // find corresponding param Type
                                ParamList* pp = symbol->u.function.params;
                                for (int j = 0; j < i && pp != NULL; ++j) pp = pp->next;
                                Type* ptype = pp ? pp->type : NULL;
                                // we only have arg_type_strs (strings); better to re-analyze arg expression types
                                // but we didn't keep ExpTypeInfo types array; so re-analyze quickly
                                // (acceptable since args are simple in tests)
                                // get arg node again: traverse args from start
                                Node* cur2 = args; int idx = 0; Node* argnode = NULL;
                                while (cur2 != NULL && is_node_name(cur2, "Args")) {
                                    if (idx == i) { argnode = get_child(cur2, 0); break; }
                                    idx++;
                                    if (cur2->u.nonterm.num_children > 1) cur2 = get_child(cur2, 2);
                                    else cur2 = NULL;
                                }
                                ExpTypeInfo arg_info = analyze_exp(context, argnode);
                                if (ptype == NULL || arg_info.type == NULL || !is_type_compatible(ptype, arg_info.type)) {
                                    mismatch = 1; break;
                                }
                            }
                        }

                        if (mismatch) {
                            // build signature strings
                            // params
                            int buf_len = 256;
                            char* params_buf = (char*)malloc(buf_len);
                            params_buf[0] = '\0';
                            strcat(params_buf, "");
                            for (int i = 0; i < param_count; ++i) {
                                if (i > 0) strcat(params_buf, ", ");
                                strcat(params_buf, param_type_strs[i]);
                            }
                            // args
                            char* args_buf = (char*)malloc(buf_len);
                            args_buf[0] = '\0';
                            for (int i = 0; i < arg_count; ++i) {
                                if (i > 0) strcat(args_buf, ", ");
                                strcat(args_buf, arg_type_strs[i]);
                            }

                            // wrap with parentheses in printf
                            char params_wrapped[512];
                            char args_wrapped[512];
                            snprintf(params_wrapped, sizeof(params_wrapped), "%s", params_buf);
                            snprintf(args_wrapped, sizeof(args_wrapped), "%s", args_buf);

                            report_semantic_error(context, ERROR_FUNCTION_ARGUMENT_MISMATCH, info.line,
                                                "Function \"%s(%s)\" is not applicable for arguments \"(%s)\"",
                                                func_name, params_wrapped, args_wrapped);

                            free(params_buf);
                            free(args_buf);
                        }

                        // free temp arrays
                        for (int i = 0; i < arg_count; ++i) free(arg_type_strs[i]);
                        free(arg_type_strs);
                        for (int i = 0; i < param_count; ++i) free(param_type_strs[i]);
                        free(param_type_strs);
                    } else {
                        // no args; if function expects params, report mismatch
                        if (symbol->u.function.params != NULL) {
                            // build param string
                            int param_count = 0; ParamList* ptmp2 = symbol->u.function.params;
                            char** param_type_strs2 = NULL;
                            while (ptmp2 != NULL) {
                                char* pts = NULL;
                                if (ptmp2->type != NULL) pts = type_to_string(ptmp2->type);
                                else pts = strdup("null");
                                param_type_strs2 = (char**)realloc(param_type_strs2, sizeof(char*) * (param_count + 1));
                                param_type_strs2[param_count] = pts;
                                param_count++;
                                ptmp2 = ptmp2->next;
                            }
                            int buf_len = 256; char* params_buf2 = (char*)malloc(buf_len); params_buf2[0] = '\0';
                            for (int i = 0; i < param_count; ++i) {
                                if (i > 0) strcat(params_buf2, ", ");
                                strcat(params_buf2, param_type_strs2[i]);
                            }
                            report_semantic_error(context, ERROR_FUNCTION_ARGUMENT_MISMATCH, info.line,
                                                "Function \"%s(%s)\" is not applicable for arguments \"()\"",
                                                func_name, params_buf2);
                            for (int i = 0; i < param_count; ++i) free(param_type_strs2[i]);
                            free(param_type_strs2);
                            free(params_buf2);
                        }
                    }

                    // set return type
                    if (symbol->u.function.return_type != NULL) {
                        info.type = copy_type(symbol->u.function.return_type);
                    }
                    info.is_lvalue = 0;
                }
            }
        }
    }
    // 处理其他表达式类型（简化：暂时返回空类型）
    
    return info;
}
