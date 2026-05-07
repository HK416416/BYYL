#include "translate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char* strdup(const char*);

typedef struct Node {
    char* name;
    int line;
    int is_terminal;
    union {
        struct {
            struct Node** children;
            int num_children;
        } nonterm;
        struct {
            char* value;
            int int_val;
            float float_val;
        } term;
    } u;
} Node;

static FILE* ir_out = NULL;
static SymbolTable* g_symtab = NULL;
static int temp_counter = 0;
static int label_counter = 0;
static int var_counter = 0;
static int error_flag = 0;

#define MAX_VARS 512
typedef struct {
    char* source_name;
    char* ir_name;
    int is_array_param;
} NamePair;
static NamePair var_names[MAX_VARS];
static int var_name_count = 0;

static const char* lookup_ir_name(const char* source_name) {
    if (!source_name) return NULL;
    for (int i = 0; i < var_name_count; i++)
        if (strcmp(var_names[i].source_name, source_name) == 0)
            return var_names[i].ir_name;
    return NULL;
}
static int is_array_param_var(const char* source_name) {
    if (!source_name) return 0;
    for (int i = 0; i < var_name_count; i++)
        if (strcmp(var_names[i].source_name, source_name) == 0)
            return var_names[i].is_array_param;
    return 0;
}
static const char* register_variable(const char* source_name) {
    if (!source_name) return NULL;
    const char* existing = lookup_ir_name(source_name);
    if (existing) return existing;
    char* ir = (char*)malloc(32);
    snprintf(ir, 32, "v%d", ++var_counter);
    var_names[var_name_count].source_name = strdup(source_name);
    var_names[var_name_count].ir_name = ir;
    var_names[var_name_count].is_array_param = 0;
    var_name_count++;
    return ir;
}
static void mark_array_param(const char* source_name) {
    if (!source_name) return;
    for (int i = 0; i < var_name_count; i++)
        if (strcmp(var_names[i].source_name, source_name) == 0)
            { var_names[i].is_array_param = 1; return; }
}

#define MAX_DECS 256
typedef struct { char* ir_name; int total_bytes; } DecEntry;
static DecEntry dec_list[MAX_DECS];
static int dec_count = 0;
static void add_dec(const char* ir_name, int total_bytes) {
    dec_list[dec_count].ir_name = strdup(ir_name);
    dec_list[dec_count].total_bytes = total_bytes;
    dec_count++;
}
static void emit_all_decs(void) {
    for (int i = 0; i < dec_count; i++)
        fprintf(ir_out, "DEC %s %d\n", dec_list[i].ir_name, dec_list[i].total_bytes);
    dec_count = 0;
}

static Node* get_child(Node* node, int index) {
    if (!node || node->is_terminal || index < 0 || index >= node->u.nonterm.num_children) return NULL;
    return node->u.nonterm.children[index];
}
static int is_name(Node* node, const char* name) {
    return node && node->name && name && strcmp(node->name, name) == 0;
}
static char* get_id(Node* node) {
    if (!node || !node->is_terminal || !node->name || strcmp(node->name, "ID") != 0) return NULL;
    return node->u.term.value;
}
static int get_int(Node* node) {
    if (!node || !node->is_terminal || !node->name || strcmp(node->name, "INT") != 0) return 0;
    return node->u.term.int_val;
}
static char* new_temp(void) {
    char* t = (char*)malloc(32);
    snprintf(t, 32, "t%d", ++temp_counter);
    return t;
}
static char* new_label(void) {
    char* l = (char*)malloc(32);
    snprintf(l, 32, "label%d", ++label_counter);
    return l;
}

typedef struct { char* place; int is_addr; Type* type; } TransExp;

static TransExp trans_exp(Node* node);
static void trans_stmt(Node* node);
static void trans_cond(Node* cond, const char* lt, const char* lf);
static void trans_comp_st(Node* node);
static void trans_def_list(Node* node);
static void trans_ext_def(Node* node);
static void trans_ext_def_list(Node* node);

static int extract_dims(Node* var_dec, int* dims, int max) {
    if (!var_dec || !is_name(var_dec, "VarDec")) return 0;
    Node* first = get_child(var_dec, 0);
    if (is_name(first, "VarDec")) {
        int n = extract_dims(first, dims, max);
        Node* ic = get_child(var_dec, 2);
        if (ic && n < max) { dims[n] = get_int(ic); return n + 1; }
        return n;
    } else if (is_name(first, "ID")) {
        if (var_dec->u.nonterm.num_children == 4 && max > 0) {
            Node* ic = get_child(var_dec, 2);
            if (ic) { dims[0] = get_int(ic); return 1; }
        }
        return 0;
    }
    return 0;
}
static char* get_var_dec_name(Node* var_dec) {
    if (!var_dec || !is_name(var_dec, "VarDec")) return NULL;
    Node* f = get_child(var_dec, 0);
    if (is_name(f, "ID")) return get_id(f);
    if (is_name(f, "VarDec")) return get_var_dec_name(f);
    return NULL;
}
static int calc_total_bytes(int* dims, int n) {
    int t = 4; for (int i = 0; i < n; i++) t *= dims[i]; return t;
}

static int check_struct_in_node(Node* node);
static int check_struct_in_children(Node* node) {
    if (!node || node->is_terminal) return 0;
    if (is_name(node, "StructSpecifier")) return 1;
    if (is_name(node, "Exp") && node->u.nonterm.num_children == 3 && is_name(get_child(node, 1), "DOT")) return 1;
    for (int i = 0; i < node->u.nonterm.num_children; i++)
        if (check_struct_in_node(get_child(node, i))) return 1;
    return 0;
}
static int check_struct_in_node(Node* node) { return check_struct_in_children(node); }

static int is_multi_dim_array_param(Node* pd) {
    if (!is_name(pd, "ParamDec")) return 0;
    Node* vd = get_child(pd, 1);
    if (!vd || !is_name(vd, "VarDec")) return 0;
    int dims[10];
    return extract_dims(vd, dims, 10) > 1;
}

/* ============================================================
 * EXPRESSION TRANSLATION
 * ============================================================ */
static TransExp trans_exp(Node* node) {
    TransExp r = {NULL, 0, NULL};
    if (!node || !is_name(node, "Exp")) return r;
    int nc = node->u.nonterm.num_children;
    Node* first = get_child(node, 0);

    /* ID */
    if (nc == 1 && is_name(first, "ID")) {
        char* name = get_id(first);
        if (!name) return r;
        Symbol* sym = find_symbol(g_symtab, name);
        const char* irn = lookup_ir_name(name);
        if (!irn) irn = register_variable(name);
        if (sym && sym->kind == SYMBOL_VARIABLE) {
            r.type = sym->u.variable.type;
            if (r.type && r.type->kind == TYPE_KIND_ARRAY) {
                /* DEC-allocated array: variable name IS the base address.
                   Array param: variable already holds the address. */
                r.place = strdup(irn); r.is_addr = 1;
            } else { r.place = strdup(irn); r.is_addr = 0; }
        } else { r.place = strdup(irn); r.is_addr = 0; }
        return r;
    }

    /* INT */
    if (nc == 1 && is_name(first, "INT")) {
        char* t = new_temp(); fprintf(ir_out, "%s := #%d\n", t, get_int(first));
        r.place = t; r.is_addr = 0; r.type = new_type_basic(TYPE_INT); return r;
    }

    /* FLOAT */
    if (nc == 1 && is_name(first, "FLOAT")) {
        char* t = new_temp(); fprintf(ir_out, "%s := #%d\n", t, (int)first->u.term.float_val);
        r.place = t; r.is_addr = 0; r.type = new_type_basic(TYPE_FLOAT); return r;
    }

    /* LP Exp RP */
    if (nc == 3 && is_name(first, "LP") && is_name(get_child(node, 2), "RP"))
        return trans_exp(get_child(node, 1));

    /* Array access: Exp LB Exp RB */
    if (nc == 4 && is_name(get_child(node, 1), "LB") && is_name(get_child(node, 3), "RB")) {
        Node* arr = get_child(node, 0);
        Node* idx = get_child(node, 2);
        TransExp ae = trans_exp(arr);
        TransExp ie = trans_exp(idx);
        if (!ae.place || !ie.place) return r;
        int es = 4;
        if (ae.type && ae.type->kind == TYPE_KIND_ARRAY) {
            Type* el = ae.type->u.array.elem;
            if (el && el->kind == TYPE_KIND_ARRAY) {
                int st = 4; Type* t = el;
                while (t && t->kind == TYPE_KIND_ARRAY) { st *= t->u.array.size; t = t->u.array.elem; }
                es = st;
            }
        }
        char* off = new_temp();
        if (es == 4) fprintf(ir_out, "%s := %s * #4\n", off, ie.place);
        else fprintf(ir_out, "%s := %s * #%d\n", off, ie.place, es);
        char* addr = new_temp();
        fprintf(ir_out, "%s := %s + %s\n", addr, ae.place, off);
        if (ae.type && ae.type->kind == TYPE_KIND_ARRAY) r.type = ae.type->u.array.elem;
        r.place = addr; r.is_addr = 1;
        free(off); free(ie.place);
        if (!ae.is_addr && ae.place) free(ae.place);
        return r;
    }

    /* Function call: ID LP ... */
    if (is_name(first, "ID") && nc >= 3 && is_name(get_child(node, 1), "LP")) {
        char* fn = get_id(first);
        if (fn && strcmp(fn, "read") == 0) {
            char* t = new_temp(); fprintf(ir_out, "READ %s\n", t);
            r.place = t; r.is_addr = 0; r.type = new_type_basic(TYPE_INT); return r;
        }
        if (fn && strcmp(fn, "write") == 0) {
            if (nc == 4) {
                Node* args = get_child(node, 2);
                Node* ae = NULL;
                if (is_name(args, "Args")) ae = get_child(args, 0);
                if (ae) {
                    TransExp arg = trans_exp(ae);
                    if (arg.is_addr) {
                        char* v = new_temp();
                        fprintf(ir_out, "%s := *%s\n", v, arg.place);
                        fprintf(ir_out, "WRITE %s\n", v);
                        free(v); free(arg.place);
                    } else {
                        fprintf(ir_out, "WRITE %s\n", arg.place);
                        free(arg.place);
                    }
                }
            }
            r.place = strdup("#0"); r.is_addr = 0; r.type = new_type_basic(TYPE_INT); return r;
        }
        /* Normal call */
        int ac = 0; char** aps = NULL;
        if (nc == 4) {
            Node* args = get_child(node, 2);
            Node* cur = args;
            while (cur && is_name(cur, "Args")) {
                Node* ae = get_child(cur, 0);
                char* aid = NULL; Symbol* as = NULL;
                if (ae && is_name(ae, "Exp") && ae->u.nonterm.num_children == 1 && is_name(get_child(ae, 0), "ID")) {
                    aid = get_id(get_child(ae, 0));
                    if (aid) as = find_symbol(g_symtab, aid);
                }
                ac++;
                aps = (char**)realloc(aps, ac * sizeof(char*));
                if (as && as->kind == SYMBOL_VARIABLE && as->u.variable.type && as->u.variable.type->kind == TYPE_KIND_ARRAY) {
                    const char* ir = lookup_ir_name(aid); if (!ir) ir = register_variable(aid);
                    aps[ac-1] = (char*)malloc(strlen(ir)+8);
                    sprintf(aps[ac-1], "%s", ir);
                } else {
                    TransExp arg = trans_exp(ae);
                    if (arg.is_addr) {
                        char* v = new_temp();
                        fprintf(ir_out, "%s := *%s\n", v, arg.place);
                        free(arg.place);
                        aps[ac-1] = v;
                    } else aps[ac-1] = arg.place;
                }
                if (cur->u.nonterm.num_children > 1) cur = get_child(cur, 2); else cur = NULL;
            }
        }
        for (int i = ac-1; i >= 0; i--) { fprintf(ir_out, "ARG %s\n", aps[i]); free(aps[i]); }
        free(aps);
        char* t = new_temp(); fprintf(ir_out, "%s := CALL %s\n", t, fn);
        r.place = t; r.is_addr = 0;
        Symbol* fs = find_symbol(g_symtab, fn);
        r.type = (fs && fs->kind == SYMBOL_FUNCTION) ? fs->u.function.return_type : new_type_basic(TYPE_INT);
        return r;
    }

    /* Assignment */
    if (nc == 3 && is_name(get_child(node, 1), "ASSIGNOP")) {
        Node* left = get_child(node, 0);
        Node* right = get_child(node, 2);
        TransExp rhs = trans_exp(right);
        TransExp lhs = trans_exp(left);
        if (!lhs.place || !rhs.place) return r;
        char* rv = rhs.place;
        if (rhs.is_addr) { rv = new_temp(); fprintf(ir_out, "%s := *%s\n", rv, rhs.place); free(rhs.place); }
        if (lhs.is_addr) { fprintf(ir_out, "*%s := %s\n", lhs.place, rv); free(lhs.place); }
        else { fprintf(ir_out, "%s := %s\n", lhs.place, rv); }
        r.place = strdup(lhs.is_addr ? rv : lhs.place);
        r.is_addr = 0;
        free(rv);
        return r;
    }

    /* Binary */
    if (nc == 3 && is_name(first, "Exp")) {
        Node* op = get_child(node, 1);
        Node* rn = get_child(node, 2);
        if (!op || !op->is_terminal) return r;
        const char* on = op->name;
        TransExp le = trans_exp(first);
        TransExp re = trans_exp(rn);
        if (!le.place || !re.place) return r;
        char* lv = le.place; char* rv = re.place;
        if (le.is_addr) { lv = new_temp(); fprintf(ir_out, "%s := *%s\n", lv, le.place); free(le.place); }
        if (re.is_addr) { rv = new_temp(); fprintf(ir_out, "%s := *%s\n", rv, re.place); free(re.place); }
        char* t = new_temp();
        if (!strcmp(on, "PLUS")) fprintf(ir_out, "%s := %s + %s\n", t, lv, rv);
        else if (!strcmp(on, "MINUS")) fprintf(ir_out, "%s := %s - %s\n", t, lv, rv);
        else if (!strcmp(on, "STAR")) fprintf(ir_out, "%s := %s * %s\n", t, lv, rv);
        else if (!strcmp(on, "DIV")) fprintf(ir_out, "%s := %s / %s\n", t, lv, rv);
        else if (!strcmp(on, "AND") || !strcmp(on, "OR")) {
            char* ls = new_label();
            if (!strcmp(on, "AND")) {
                fprintf(ir_out, "%s := %s\n", t, lv);
                fprintf(ir_out, "IF %s == #0 GOTO %s\n", lv, ls);
                fprintf(ir_out, "%s := %s\n", t, rv);
            } else {
                fprintf(ir_out, "%s := %s\n", t, lv);
                fprintf(ir_out, "IF %s != #0 GOTO %s\n", lv, ls);
                fprintf(ir_out, "%s := %s\n", t, rv);
            }
            fprintf(ir_out, "LABEL %s :\n", ls); free(ls);
        } else {
            const char* rel = "==";
            if (!strcmp(on, "LT")) rel = "<"; else if (!strcmp(on, "LE")) rel = "<=";
            else if (!strcmp(on, "GT")) rel = ">"; else if (!strcmp(on, "GE")) rel = ">=";
            else if (!strcmp(on, "EQ")) rel = "=="; else if (!strcmp(on, "NE")) rel = "!=";
            char* lt = new_label(); char* le_ = new_label();
            fprintf(ir_out, "IF %s %s %s GOTO %s\n", lv, rel, rv, lt);
            fprintf(ir_out, "%s := #0\n", t);
            fprintf(ir_out, "GOTO %s\n", le_);
            fprintf(ir_out, "LABEL %s :\n", lt);
            fprintf(ir_out, "%s := #1\n", t);
            fprintf(ir_out, "LABEL %s :\n", le_);
            free(lt); free(le_);
        }
        r.place = t; r.is_addr = 0; r.type = new_type_basic(TYPE_INT);
        free(lv); free(rv);
        return r;
    }

    /* Unary - */
    if (nc == 2 && is_name(first, "MINUS")) {
        TransExp in = trans_exp(get_child(node, 1));
        if (!in.place) return r;
        char* iv = in.place;
        if (in.is_addr) { iv = new_temp(); fprintf(ir_out, "%s := *%s\n", iv, in.place); free(in.place); }
        char* t = new_temp(); fprintf(ir_out, "%s := #0 - %s\n", t, iv);
        r.place = t; r.is_addr = 0; r.type = new_type_basic(TYPE_INT);
        free(iv); return r;
    }

    /* Unary ! */
    if (nc == 2 && is_name(first, "NOT")) {
        TransExp in = trans_exp(get_child(node, 1));
        if (!in.place) return r;
        char* iv = in.place;
        if (in.is_addr) { iv = new_temp(); fprintf(ir_out, "%s := *%s\n", iv, in.place); free(in.place); }
        char* t = new_temp(); char* lt = new_label(); char* le_ = new_label();
        fprintf(ir_out, "IF %s == #0 GOTO %s\n", iv, lt);
        fprintf(ir_out, "%s := #0\n", t); fprintf(ir_out, "GOTO %s\n", le_);
        fprintf(ir_out, "LABEL %s :\n", lt);
        fprintf(ir_out, "%s := #1\n", t); fprintf(ir_out, "LABEL %s :\n", le_);
        r.place = t; r.is_addr = 0; r.type = new_type_basic(TYPE_INT);
        free(lt); free(le_); free(iv); return r;
    }

    /* DOT */
    if (nc == 3 && is_name(get_child(node, 1), "DOT")) {
        fprintf(ir_out, "Cannot translate: Code contains struct type variables or struct parameters.\n");
        error_flag = 1; return r;
    }
    return r;
}

/* ============================================================
 * CONDITION
 * ============================================================ */
static void trans_cond(Node* cond, const char* lt, const char* lf) {
    if (!cond || !is_name(cond, "Exp")) { fprintf(ir_out, "GOTO %s\n", lf); return; }
    int nc = cond->u.nonterm.num_children;
    Node* first = get_child(cond, 0);
    if (nc == 3 && is_name(first, "Exp")) {
        Node* op = get_child(cond, 1);
        const char* on = op ? op->name : "";
        if (!strcmp(on, "LT") || !strcmp(on, "LE") || !strcmp(on, "GT") || !strcmp(on, "GE") ||
            !strcmp(on, "EQ") || !strcmp(on, "NE")) {
            TransExp lhs = trans_exp(first);
            TransExp rhs = trans_exp(get_child(cond, 2));
            const char* rel = "==";
            if (!strcmp(on, "LT")) rel = "<"; else if (!strcmp(on, "LE")) rel = "<=";
            else if (!strcmp(on, "GT")) rel = ">"; else if (!strcmp(on, "GE")) rel = ">=";
            else if (!strcmp(on, "EQ")) rel = "=="; else if (!strcmp(on, "NE")) rel = "!=";
            char* lv = lhs.place; char* rv = rhs.place;
            if (lhs.is_addr) { lv = new_temp(); fprintf(ir_out, "%s := *%s\n", lv, lhs.place); free(lhs.place); }
            if (rhs.is_addr) { rv = new_temp(); fprintf(ir_out, "%s := *%s\n", rv, rhs.place); free(rhs.place); }
            fprintf(ir_out, "IF %s %s %s GOTO %s\n", lv, rel, rv, lt);
            fprintf(ir_out, "GOTO %s\n", lf);
            free(lv); free(rv); return;
        }
        if (!strcmp(on, "AND")) {
            char* mid = new_label();
            trans_cond(first, mid, lf);
            fprintf(ir_out, "LABEL %s :\n", mid);
            trans_cond(get_child(cond, 2), lt, lf);
            free(mid); return;
        }
        if (!strcmp(on, "OR")) {
            char* mid = new_label();
            trans_cond(first, lt, mid);
            fprintf(ir_out, "LABEL %s :\n", mid);
            trans_cond(get_child(cond, 2), lt, lf);
            free(mid); return;
        }
    }
    if (nc == 2 && is_name(first, "NOT")) { trans_cond(get_child(cond, 1), lf, lt); return; }
    if (nc == 3 && is_name(first, "LP")) { trans_cond(get_child(cond, 1), lt, lf); return; }
    TransExp val = trans_exp(cond);
    if (val.place) {
        char* v = val.place;
        if (val.is_addr) { v = new_temp(); fprintf(ir_out, "%s := *%s\n", v, val.place); free(val.place); }
        fprintf(ir_out, "IF %s != #0 GOTO %s\n", v, lt);
        fprintf(ir_out, "GOTO %s\n", lf);
        free(v);
    } else fprintf(ir_out, "GOTO %s\n", lf);
}

/* ============================================================
 * STATEMENT
 * ============================================================ */
static void trans_stmt(Node* node) {
    if (!node || !is_name(node, "Stmt")) return;
    int nc = node->u.nonterm.num_children;
    Node* first = get_child(node, 0);
    if (nc == 1 && is_name(first, "CompSt")) { trans_comp_st(first); return; }
    if (nc == 2 && is_name(first, "Exp") && is_name(get_child(node, 1), "SEMI")) {
        TransExp e = trans_exp(first); if (e.place) free(e.place); return;
    }
    if (nc == 3 && is_name(first, "RETURN")) {
        TransExp v = trans_exp(get_child(node, 1));
        if (v.place) {
            if (v.is_addr) { char* x = new_temp(); fprintf(ir_out, "%s := *%s\n", x, v.place); fprintf(ir_out, "RETURN %s\n", x); free(x); free(v.place); }
            else { fprintf(ir_out, "RETURN %s\n", v.place); free(v.place); }
        } return;
    }
    if (nc == 5 && is_name(first, "WHILE")) {
        char* lb = new_label(); char* lbo = new_label(); char* le_ = new_label();
        fprintf(ir_out, "LABEL %s :\n", lb);
        trans_cond(get_child(node, 2), lbo, le_);
        fprintf(ir_out, "LABEL %s :\n", lbo);
        trans_stmt(get_child(node, 4));
        fprintf(ir_out, "GOTO %s\n", lb);
        fprintf(ir_out, "LABEL %s :\n", le_);
        free(lb); free(lbo); free(le_); return;
    }
    if (nc == 5 && is_name(first, "IF")) {
        char* lthen = new_label(); char* lend = new_label();
        trans_cond(get_child(node, 2), lthen, lend);
        fprintf(ir_out, "LABEL %s :\n", lthen);
        trans_stmt(get_child(node, 4));
        fprintf(ir_out, "LABEL %s :\n", lend);
        free(lthen); free(lend); return;
    }
    if (nc == 7 && is_name(first, "IF")) {
        char* lthen = new_label(); char* lelse = new_label(); char* lend = new_label();
        trans_cond(get_child(node, 2), lthen, lelse);
        fprintf(ir_out, "LABEL %s :\n", lthen);
        trans_stmt(get_child(node, 4));
        fprintf(ir_out, "GOTO %s\n", lend);
        fprintf(ir_out, "LABEL %s :\n", lelse);
        trans_stmt(get_child(node, 6));
        fprintf(ir_out, "LABEL %s :\n", lend);
        free(lthen); free(lelse); free(lend); return;
    }
}

/* ============================================================
 * COMPST / DEFLIST
 * ============================================================ */
static void translate_initializers(Node* node);
static void trans_comp_st(Node* node) {
    if (!node || !is_name(node, "CompSt")) return;
    Node* dl = get_child(node, 1);
    Node* sl = get_child(node, 2);
    if (dl) trans_def_list(dl);
    emit_all_decs();
    if (dl) translate_initializers(dl);
    Node* cur = sl;
    while (cur && is_name(cur, "StmtList")) {
        Node* s = get_child(cur, 0);
        if (s) trans_stmt(s);
        if (cur->u.nonterm.num_children > 1) cur = get_child(cur, 1); else cur = NULL;
    }
}
static void trans_def_list(Node* node) {
    Node* cur = node;
    while (cur && is_name(cur, "DefList")) {
        Node* def = get_child(cur, 0);
        if (def && is_name(def, "Def")) {
            Node* dcl = get_child(def, 1);
            Node* cd = dcl;
            while (cd && is_name(cd, "DecList")) {
                Node* dec = get_child(cd, 0);
                if (dec && is_name(dec, "Dec")) {
                    Node* vd = get_child(dec, 0);
                    if (vd && is_name(vd, "VarDec")) {
                        char* vn = get_var_dec_name(vd);
                        if (vn) {
                            register_variable(vn);
                            int dims[10]; int nd = extract_dims(vd, dims, 10);
                            if (nd > 0) add_dec(lookup_ir_name(vn), calc_total_bytes(dims, nd));
                        }
                    }
                }
                if (cd->u.nonterm.num_children > 1) cd = get_child(cd, 2); else cd = NULL;
            }
        }
        if (cur->u.nonterm.num_children > 1) cur = get_child(cur, 1); else cur = NULL;
    }
}
static void translate_initializers(Node* node) {
    Node* cur = node;
    while (cur && is_name(cur, "DefList")) {
        Node* def = get_child(cur, 0);
        if (def && is_name(def, "Def")) {
            Node* dcl = get_child(def, 1);
            Node* cd = dcl;
            while (cd && is_name(cd, "DecList")) {
                Node* dec = get_child(cd, 0);
                if (dec && is_name(dec, "Dec") && dec->u.nonterm.num_children == 3) {
                    Node* vd = get_child(dec, 0);
                    Node* exp = get_child(dec, 2);
                    if (vd && exp) {
                        char* vn = get_var_dec_name(vd);
                        if (vn) {
                            const char* irn = lookup_ir_name(vn);
                            if (irn) {
                                TransExp val = trans_exp(exp);
                                if (val.place) { fprintf(ir_out, "%s := %s\n", irn, val.place); free(val.place); }
                            }
                        }
                    }
                }
                if (cd->u.nonterm.num_children > 1) cd = get_child(cd, 2); else cd = NULL;
            }
        }
        if (cur->u.nonterm.num_children > 1) cur = get_child(cur, 1); else cur = NULL;
    }
}

static void trans_function(Node* ext_def) {
    if (!ext_def || !is_name(ext_def, "ExtDef")) return;
    Node* fd = get_child(ext_def, 1);
    Node* cs = get_child(ext_def, 2);
    if (!fd || !is_name(fd, "FunDec") || !cs || !is_name(cs, "CompSt")) return;
    Node* id = get_child(fd, 0);
    char* fn = get_id(id);
    if (!fn) return;
    fprintf(ir_out, "FUNCTION %s :\n", fn);
    dec_count = 0;
    int np = fd->u.nonterm.num_children;
    if (np == 4) {
        Node* vl = get_child(fd, 2);
        Node* cur = vl;
        while (cur && is_name(cur, "VarList")) {
            Node* pd = get_child(cur, 0);
            if (pd && is_name(pd, "ParamDec")) {
                Node* vd = get_child(pd, 1);
                if (vd && is_name(vd, "VarDec")) {
                    char* pn = get_var_dec_name(vd);
                    if (pn) {
                        const char* irn = register_variable(pn);
                        int dims[10];
                        if (extract_dims(vd, dims, 10) > 0) mark_array_param(pn);
                        fprintf(ir_out, "PARAM %s\n", irn);
                    }
                }
            }
            if (cur->u.nonterm.num_children > 1) cur = get_child(cur, 2); else cur = NULL;
        }
    }
    trans_comp_st(cs);
}

static void trans_ext_def_list(Node* node) {
    Node* cur = node;
    while (cur && is_name(cur, "ExtDefList")) {
        Node* ed = get_child(cur, 0);
        if (ed && is_name(ed, "ExtDef")) trans_ext_def(ed);
        if (cur->u.nonterm.num_children > 1) cur = get_child(cur, 1); else cur = NULL;
    }
}
static void trans_ext_def(Node* node) {
    if (!node || !is_name(node, "ExtDef")) return;
    if (node->u.nonterm.num_children == 3 && is_name(get_child(node, 1), "FunDec") && is_name(get_child(node, 2), "CompSt"))
        trans_function(node);
}

int translate_program(SemanticContext* ctx, Node* root, FILE* output) {
    if (!ctx || !root || !output) return 1;
    ir_out = output; g_symtab = ctx->global_table; error_flag = 0;
    var_name_count = 0; var_counter = 0;
    if (check_struct_in_node(root)) { fprintf(output, "Cannot translate: Code contains struct type variables or struct parameters.\n"); return 1; }
    Node* el = get_child(root, 0);
    Node* cur = el;
    while (cur && is_name(cur, "ExtDefList")) {
        Node* ed = get_child(cur, 0);
        if (ed && is_name(ed, "ExtDef")) {
            Node* fd = get_child(ed, 1);
            Node* cs = get_child(ed, 2);
            if (fd && is_name(fd, "FunDec") && cs) {
                int np = fd->u.nonterm.num_children;
                if (np == 4) {
                    Node* vl = get_child(fd, 2); Node* vc = vl;
                    while (vc && is_name(vc, "VarList")) {
                        if (is_multi_dim_array_param(get_child(vc, 0))) {
                            fprintf(output, "Cannot translate: Code contains variables of multi-dimensional array type or parameters of array type.\n");
                            return 1;
                        }
                        if (vc->u.nonterm.num_children > 1) vc = get_child(vc, 2); else vc = NULL;
                    }
                }
            }
        }
        if (cur->u.nonterm.num_children > 1) cur = get_child(cur, 1); else cur = NULL;
    }
    trans_ext_def_list(el);
    for (int i = 0; i < var_name_count; i++) { free(var_names[i].source_name); free(var_names[i].ir_name); }
    var_name_count = 0;
    return error_flag ? 1 : 0;
}
