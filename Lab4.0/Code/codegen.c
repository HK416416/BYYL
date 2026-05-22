#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * IR representation
 * ================================================================ */
#define MAX_IR     4096
#define MAX_VARS    512

typedef enum {
    IR_NONE,
    IR_FUNCTION, IR_PARAM,
    IR_LABEL, IR_GOTO,
    IR_ASSIGN_CONST, IR_ASSIGN_VAR, IR_ASSIGN_BINOP,
    IR_ASSIGN_LOAD, IR_STORE,
    IR_ASSIGN_CALL, IR_ARG,
    IR_RETURN,
    IR_IF_GOTO, IR_READ, IR_WRITE, IR_DEC
} IROp;

typedef struct { IROp op; char a[4][64]; int line; } IR;

static IR   irs[MAX_IR];
static int  nir = 0;
static FILE* asm_out = NULL;

/* ================================================================
 * Stack frame for one function
 * ================================================================ */
typedef struct {
    char name[64];
    /* variable → stack offset (from $sp after prologue) */
    char vars[MAX_VARS][64];
    int  offsets[MAX_VARS];   /* offset from $sp */
    int  nvar;
    int  nparam;
    char params[16][64];
    int  param_offsets[16];
    int  frame_size;          /* total bytes allocated on stack */
    int  uses_decs;           /* has DEC arrays */
    int  has_calls;
} Frame;

static Frame frm;

/* ================================================================
 * Var tracking
 * ================================================================ */
static int find_var(const char* name) {
    for (int i = 0; i < frm.nvar; i++)
        if (!strcmp(frm.vars[i], name)) return i;
    return -1;
}

static void add_var(const char* name, int bytes) {
    if (!name || !name[0] || name[0] == '#') return;
    if (find_var(name) >= 0) return;
    if (frm.nvar >= MAX_VARS) return;
    strncpy(frm.vars[frm.nvar], name, 63);
    frm.offsets[frm.nvar] = frm.frame_size;
    frm.frame_size += bytes;
    frm.nvar++;
}

/* ================================================================
 * IR Parser
 * ================================================================ */
static int is_const(const char* s) { return s && s[0] == '#'; }

static void parse_ir(FILE* in) {
    char buf[512];
    nir = 0;
    int ln = 0;
    while (fgets(buf, sizeof(buf), in) && nir < MAX_IR) {
        ln++;
        int len = (int)strlen(buf);
        while (len > 0 && (buf[len-1]=='\n'||buf[len-1]=='\r'||
                           buf[len-1]==' ' ||buf[len-1]=='\t'))
            buf[--len] = '\0';
        if (len == 0) continue;

        char* tk[16];
        int nt = 0;
        char* p = buf;
        while (*p && nt < 16) {
            while (*p == ' ' || *p == '\t') p++;
            if (!*p) break;
            tk[nt++] = p;
            while (*p && *p != ' ' && *p != '\t') p++;
            if (*p) { *p = '\0'; p++; }
        }
        if (!nt) continue;

        IR ir; memset(&ir, 0, sizeof(ir)); ir.line = ln;

        if (nt >= 2 && !strcmp(tk[0], "FUNCTION")) {
            ir.op = IR_FUNCTION;
            char* nm = tk[1];
            int nl2 = (int)strlen(nm);
            if (nl2 > 0 && nm[nl2-1] == ':') nm[nl2-1] = '\0';
            strncpy(ir.a[0], nm, 63);
        }
        else if (nt >= 2 && !strcmp(tk[0], "PARAM")) {
            ir.op = IR_PARAM; strncpy(ir.a[0], tk[1], 63);
        }
        else if (nt >= 2 && !strcmp(tk[0], "LABEL")) {
            ir.op = IR_LABEL;
            char* nm = tk[1];
            int nl2 = (int)strlen(nm);
            if (nl2 > 0 && nm[nl2-1] == ':') nm[nl2-1] = '\0';
            strncpy(ir.a[0], nm, 63);
        }
        else if (nt >= 2 && !strcmp(tk[0], "READ")) {
            ir.op = IR_READ; strncpy(ir.a[0], tk[1], 63);
        }
        else if (nt >= 2 && !strcmp(tk[0], "WRITE")) {
            ir.op = IR_WRITE; strncpy(ir.a[0], tk[1], 63);
        }
        else if (nt >= 3 && !strcmp(tk[0], "DEC")) {
            ir.op = IR_DEC;
            strncpy(ir.a[0], tk[1], 63);
            strncpy(ir.a[1], tk[2], 63);
        }
        else if (nt >= 2 && !strcmp(tk[0], "ARG")) {
            ir.op = IR_ARG; strncpy(ir.a[0], tk[1], 63);
        }
        else if (nt >= 2 && !strcmp(tk[0], "GOTO")) {
            ir.op = IR_GOTO; strncpy(ir.a[0], tk[1], 63);
        }
        else if (nt >= 2 && !strcmp(tk[0], "RETURN")) {
            ir.op = IR_RETURN; strncpy(ir.a[0], tk[1], 63);
        }
        else if (nt >= 6 && !strcmp(tk[0], "IF") && !strcmp(tk[4], "GOTO")) {
            ir.op = IR_IF_GOTO;
            strncpy(ir.a[0], tk[1], 63);   /* x */
            strncpy(ir.a[1], tk[2], 63);   /* relop */
            strncpy(ir.a[2], tk[3], 63);   /* y */
            strncpy(ir.a[3], tk[5], 63);   /* label */
        }
        else if (nt >= 3 && !strcmp(tk[1], ":=")) {
            if (nt == 4 && !strcmp(tk[2], "CALL")) {
                ir.op = IR_ASSIGN_CALL;
                strncpy(ir.a[0], tk[0], 63);
                strncpy(ir.a[1], tk[3], 63);
            } else if (nt == 3 && tk[2][0] == '*') {
                ir.op = IR_ASSIGN_LOAD;
                strncpy(ir.a[0], tk[0], 63);
                strncpy(ir.a[1], tk[2] + 1, 63);
            } else if (nt == 3 && tk[2][0] == '#') {
                ir.op = IR_ASSIGN_CONST;
                strncpy(ir.a[0], tk[0], 63);
                strncpy(ir.a[1], tk[2] + 1, 63);
            } else if (nt == 3) {
                ir.op = IR_ASSIGN_VAR;
                strncpy(ir.a[0], tk[0], 63);
                strncpy(ir.a[1], tk[2], 63);
            } else if (nt == 5) {
                ir.op = IR_ASSIGN_BINOP;
                strncpy(ir.a[0], tk[0], 63);
                strncpy(ir.a[1], tk[2], 63);
                strncpy(ir.a[2], tk[3], 63);   /* operator */
                strncpy(ir.a[3], tk[4], 63);   /* 2nd operand */
            }
        }
        else if (nt >= 3 && tk[0][0] == '*' && !strcmp(tk[1], ":=")) {
            ir.op = IR_STORE;
            strncpy(ir.a[0], tk[0] + 1, 63);
            strncpy(ir.a[1], tk[2], 63);
        }

        irs[nir++] = ir;
    }
}

/* ================================================================
 * Helpers: load value into $t0-t9 temp, get offset
 * ================================================================ */
static int off_of(const char* name) {
    int vi = find_var(name);
    return (vi >= 0) ? frm.offsets[vi] : -1;
}

/* Load variable/constant into $t0; returns 0 if it's a constant, 1 if from stack */
static int load_to_t0(const char* name) {
    if (is_const(name)) {
        fprintf(asm_out, "    li $t0, %s\n", name + 1);
        return 0;
    }
    int vi = find_var(name);
    if (vi >= 0) {
        fprintf(asm_out, "    lw $t0, %d($sp)\n", frm.offsets[vi]);
    } else {
        fprintf(asm_out, "    li $t0, 0\n");
    }
    return 1;
}

/* Load variable/constant into $t1 */
static int load_to_t1(const char* name) {
    if (is_const(name)) {
        fprintf(asm_out, "    li $t1, %s\n", name + 1);
        return 0;
    }
    int vi = find_var(name);
    if (vi >= 0) {
        fprintf(asm_out, "    lw $t1, %d($sp)\n", frm.offsets[vi]);
    } else {
        fprintf(asm_out, "    li $t1, 0\n");
    }
    return 1;
}

/* Store $t0 to variable */
static void store_t0(const char* name) {
    int vi = find_var(name);
    if (vi >= 0)
        fprintf(asm_out, "    sw $t0, %d($sp)\n", frm.offsets[vi]);
}

/* ================================================================
 * Call sequence
 * ================================================================ */
static char  arg_buf[16][64];
static int   arg_cnt = 0;

static void do_call(const char* fn, const char* dst) {
    int n = arg_cnt;
    int stack_args = (n > 4) ? n - 4 : 0;
    int save_space = (frm.has_calls ? 4 : 0) + stack_args * 4;

    /* Step 1: Load all argument values into temps BEFORE adjusting $sp.
       (Because offsets are relative to current $sp position.) */
    /* For $a0-$a3: load into $t4-$t7 (non-parameter temps) */
    char arg_temps[4][8];
    for (int j = 0; j < n && j < 4; j++) {
        int src_idx = n - 1 - j;  /* args are reversed in arg_buf */
        if (is_const(arg_buf[src_idx])) {
            fprintf(asm_out, "    li $t%d, %s\n", 4 + j, arg_buf[src_idx] + 1);
            snprintf(arg_temps[j], 8, "$t%d", 4 + j);
        } else {
            int vi = find_var(arg_buf[src_idx]);
            if (vi >= 0) {
                fprintf(asm_out, "    lw $t%d, %d($sp)\n", 4 + j, frm.offsets[vi]);
                snprintf(arg_temps[j], 8, "$t%d", 4 + j);
            }
        }
    }
    /* For stack args (beyond 4): load into $t0-$t3 and save to a temp array;
       but simpler: just load them now and we'll push after $sp change */
    char stack_temps[12][8];
    for (int j = 4; j < n; j++) {
        int src_idx = n - 1 - j;
        if (is_const(arg_buf[src_idx])) {
            fprintf(asm_out, "    li $t0, %s\n", arg_buf[src_idx] + 1);
        } else {
            int vi = find_var(arg_buf[src_idx]);
            if (vi >= 0)
                fprintf(asm_out, "    lw $t0, %d($sp)\n", frm.offsets[vi]);
        }
        /* Immediately push to stack area (before $sp change, we need to be careful).
           Actually, let's use $t0-$t3 for first 4 stack args and push them after $sp change */
        snprintf(stack_temps[j - 4], 8, "$t%d", (j - 4) % 4);
        fprintf(asm_out, "    move %s, $t0\n", stack_temps[j - 4]);
    }

    /* Step 2: Adjust $sp, save $ra */
    if (frm.has_calls) {
        fprintf(asm_out, "    addiu $sp, $sp, -%d\n", save_space);
        fprintf(asm_out, "    sw $ra, %d($sp)\n", save_space - 4);
    } else if (save_space > 0) {
        fprintf(asm_out, "    addiu $sp, $sp, -%d\n", save_space);
    }

    /* Step 3: Push stack args (beyond 4) */
    for (int j = 4; j < n; j++) {
        fprintf(asm_out, "    sw %s, %d($sp)\n", stack_temps[j - 4], (j - 4) * 4);
    }

    /* Step 4: Set $a0-$a3 */
    for (int j = 0; j < n && j < 4; j++) {
        fprintf(asm_out, "    move $a%d, %s\n", j, arg_temps[j]);
    }

    fprintf(asm_out, "    jal %s\n", fn);

    /* Step 5: Restore $sp and $ra */
    if (frm.has_calls) {
        fprintf(asm_out, "    lw $ra, %d($sp)\n", save_space - 4);
        fprintf(asm_out, "    addiu $sp, $sp, %d\n", save_space);
    } else if (save_space > 0) {
        fprintf(asm_out, "    addiu $sp, $sp, %d\n", save_space);
    }

    /* Step 6: Store $v0 to dst */
    if (dst) {
        int vi = find_var(dst);
        if (vi >= 0)
            fprintf(asm_out, "    sw $v0, %d($sp)\n", frm.offsets[vi]);
    }

    arg_cnt = 0;
}

/* ================================================================
 * Generate code for one function
 * ================================================================ */
static void gen_func(int start, int end) {
    fprintf(asm_out, "%s:\n", frm.name);

    /* Prologue */
    int fs = frm.frame_size;
    if (frm.has_calls) {
        /* Reserve extra 4 bytes for $ra save at top of frame */
        fs += 4;
    }
    if (fs > 0)
        fprintf(asm_out, "    addiu $sp, $sp, -%d\n", fs);
    if (frm.has_calls)
        fprintf(asm_out, "    sw $ra, %d($sp)\n", fs - 4);

    /* Copy params from $a0-$a3 to their stack slots */
    for (int i = 0; i < frm.nparam && i < 4; i++) {
        int vi = find_var(frm.params[i]);
        if (vi >= 0)
            fprintf(asm_out, "    sw $a%d, %d($sp)\n", i, frm.offsets[vi]);
    }

    arg_cnt = 0;

    for (int i = start + 1; i < end; i++) {
        IR* ir = &irs[i];

        switch (ir->op) {
        case IR_LABEL:
            fprintf(asm_out, "%s:\n", ir->a[0]);
            break;

        case IR_GOTO:
            fprintf(asm_out, "    j %s\n", ir->a[0]);
            break;

        case IR_ASSIGN_CONST: /* x := #k */
            fprintf(asm_out, "    li $t0, %s\n", ir->a[1]);
            store_t0(ir->a[0]);
            break;

        case IR_ASSIGN_VAR: /* x := y */
            load_to_t0(ir->a[1]);
            store_t0(ir->a[0]);
            break;

        case IR_ASSIGN_BINOP: { /* x := y op z */
            const char* op = ir->a[2];
            int y_is_const = is_const(ir->a[1]);
            int z_is_const = is_const(ir->a[3]);

            if (!strcmp(op, "+")) {
                if (z_is_const) {
                    load_to_t0(ir->a[1]);
                    fprintf(asm_out, "    addiu $t0, $t0, %s\n", ir->a[3] + 1);
                } else if (y_is_const) {
                    load_to_t0(ir->a[3]);
                    fprintf(asm_out, "    addiu $t0, $t0, %s\n", ir->a[1] + 1);
                } else {
                    load_to_t0(ir->a[1]);
                    load_to_t1(ir->a[3]);
                    fprintf(asm_out, "    addu $t0, $t0, $t1\n");
                }
            } else if (!strcmp(op, "-")) {
                if (z_is_const) {
                    load_to_t0(ir->a[1]);
                    fprintf(asm_out, "    addiu $t0, $t0, -%s\n", ir->a[3] + 1);
                } else {
                    load_to_t0(ir->a[1]);
                    load_to_t1(ir->a[3]);
                    fprintf(asm_out, "    subu $t0, $t0, $t1\n");
                }
            } else if (!strcmp(op, "*")) {
                load_to_t0(ir->a[1]);
                load_to_t1(ir->a[3]);
                fprintf(asm_out, "    mul $t0, $t0, $t1\n");
            } else if (!strcmp(op, "/")) {
                load_to_t0(ir->a[1]);
                load_to_t1(ir->a[3]);
                fprintf(asm_out, "    div $t0, $t1\n");
                fprintf(asm_out, "    mflo $t0\n");
            }
            store_t0(ir->a[0]);
            break;
        }

        case IR_ASSIGN_LOAD: /* x := *y */
            load_to_t0(ir->a[1]);
            fprintf(asm_out, "    lw $t0, 0($t0)\n");
            store_t0(ir->a[0]);
            break;

        case IR_STORE: /* *x := y */
            load_to_t0(ir->a[0]);  /* x = address */
            load_to_t1(ir->a[1]);  /* y = value */
            fprintf(asm_out, "    sw $t1, 0($t0)\n");
            break;

        case IR_READ: /* READ x */
            do_call("read", NULL);
            /* $v0 has the value, store to x */
            {
                int vi = find_var(ir->a[0]);
                if (vi >= 0)
                    fprintf(asm_out, "    sw $v0, %d($sp)\n", frm.offsets[vi]);
            }
            break;

        case IR_WRITE: /* WRITE x */
            load_to_t0(ir->a[0]);
            fprintf(asm_out, "    move $a0, $t0\n");
            do_call("write", NULL);
            break;

        case IR_ARG:
            if (arg_cnt < 16) {
                strncpy(arg_buf[arg_cnt], ir->a[0], 63);
                arg_cnt++;
            }
            break;

        case IR_ASSIGN_CALL: /* x := CALL fn */
            do_call(ir->a[1], ir->a[0]);
            break;

        case IR_RETURN: /* RETURN x */
            load_to_t0(ir->a[0]);
            fprintf(asm_out, "    move $v0, $t0\n");
            /* Epilogue */
            {
                int fs2 = frm.frame_size;
                if (frm.has_calls) fs2 += 4;
                if (frm.has_calls)
                    fprintf(asm_out, "    lw $ra, %d($sp)\n", fs2 - 4);
                if (fs2 > 0)
                    fprintf(asm_out, "    addiu $sp, $sp, %d\n", fs2);
            }
            fprintf(asm_out, "    jr $ra\n");
            break;

        case IR_IF_GOTO: { /* IF x relop y GOTO label */
            const char* mop;
            if (!strcmp(ir->a[1], "==")) mop = "beq";
            else if (!strcmp(ir->a[1], "!=")) mop = "bne";
            else if (!strcmp(ir->a[1], "<"))  mop = "blt";
            else if (!strcmp(ir->a[1], "<=")) mop = "ble";
            else if (!strcmp(ir->a[1], ">"))  mop = "bgt";
            else if (!strcmp(ir->a[1], ">=")) mop = "bge";
            else mop = "beq";

            load_to_t0(ir->a[0]);
            load_to_t1(ir->a[2]);
            fprintf(asm_out, "    %s $t0, $t1, %s\n", mop, ir->a[3]);
            break;
        }

        default:
            break;
        }
    }
}

/* ================================================================
 * Top-level
 * ================================================================ */
static void emit_prelude(void) {
    fprintf(asm_out, ".data\n");
    fprintf(asm_out, "_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(asm_out, "_ret: .asciiz \"\\n\"\n");
    fprintf(asm_out, ".globl main\n");
    fprintf(asm_out, ".text\n\n");

    fprintf(asm_out, "read:\n");
    fprintf(asm_out, "    li $v0, 4\n");
    fprintf(asm_out, "    la $a0, _prompt\n");
    fprintf(asm_out, "    syscall\n");
    fprintf(asm_out, "    li $v0, 5\n");
    fprintf(asm_out, "    syscall\n");
    fprintf(asm_out, "    jr $ra\n\n");

    fprintf(asm_out, "write:\n");
    fprintf(asm_out, "    li $v0, 1\n");
    fprintf(asm_out, "    syscall\n");
    fprintf(asm_out, "    li $v0, 4\n");
    fprintf(asm_out, "    la $a0, _ret\n");
    fprintf(asm_out, "    syscall\n");
    fprintf(asm_out, "    move $v0, $0\n");
    fprintf(asm_out, "    jr $ra\n\n");
}

static void generate_all(void) {
    emit_prelude();

    int i = 0;
    while (i < nir) {
        if (irs[i].op != IR_FUNCTION) { i++; continue; }

        int start = i;
        int end = nir;
        for (int j = i + 1; j < nir; j++)
            if (irs[j].op == IR_FUNCTION) { end = j; break; }

        /* Build frame */
        memset(&frm, 0, sizeof(frm));
        strncpy(frm.name, irs[start].a[0], 63);

        /* Pass 1: collect params */
        for (int j = start; j < end; j++) {
            if (irs[j].op == IR_PARAM) {
                if (frm.nparam < 16) {
                    strncpy(frm.params[frm.nparam], irs[j].a[0], 63);
                    frm.nparam++;
                }
            }
        }

        /* Pass 2: collect variables + detect calls */
        for (int j = start; j < end; j++) {
            IR* ir = &irs[j];
            if (ir->op == IR_ASSIGN_CALL) frm.has_calls = 1;
            if (ir->op == IR_ARG) frm.has_calls = 1;
            if (ir->op == IR_READ && !strcmp(frm.name, "main")) frm.has_calls = 1;
            if (ir->op == IR_WRITE && !strcmp(frm.name, "main")) frm.has_calls = 1;

            switch (ir->op) {
            case IR_ASSIGN_CONST: case IR_ASSIGN_VAR:
                add_var(ir->a[0], 4); add_var(ir->a[1], 4); break;
            case IR_ASSIGN_BINOP:
                add_var(ir->a[0], 4); add_var(ir->a[1], 4); 
                if(!is_const(ir->a[3])) add_var(ir->a[3], 4); break;
            case IR_ASSIGN_LOAD: case IR_STORE:
                add_var(ir->a[0], 4); add_var(ir->a[1], 4); break;
            case IR_ASSIGN_CALL: add_var(ir->a[0], 4); break;
            case IR_ARG: add_var(ir->a[0], 4); break;
            case IR_RETURN: if(!is_const(ir->a[0])) add_var(ir->a[0], 4); break;
            case IR_IF_GOTO:
                add_var(ir->a[0], 4); 
                if(!is_const(ir->a[2])) add_var(ir->a[2], 4); break;
            case IR_READ: case IR_WRITE: add_var(ir->a[0], 4); break;
            case IR_DEC:
                frm.uses_decs = 1;
                add_var(ir->a[0], atoi(ir->a[1]));
                break;
            default: break;
            }
        }

        gen_func(start, end);
        fprintf(asm_out, "\n");
        i = end;
    }
}

/* ================================================================
 * Public
 * ================================================================ */
int codegen_generate(FILE* ir_input, FILE* asm_output) {
    if (!ir_input || !asm_output) return 1;
    asm_out = asm_output;
    parse_ir(ir_input);
    if (nir == 0) { fprintf(stderr, "Error: empty IR input.\n"); return 1; }
    generate_all();
    return 0;
}
