.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text

read:
    li $v0, 4
    la $a0, _prompt
    syscall
    li $v0, 5
    syscall
    jr $ra

write:
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, _ret
    syscall
    move $v0, $0
    jr $ra

fact:
    addiu $sp, $sp, -32
    sw $ra, 28($sp)
    sw $a0, 8($sp)
    li $t0, 1
    sw $t0, 0($sp)
    lw $t0, 8($sp)
    lw $t1, 0($sp)
    beq $t0, $t1, label1
    j label2
label1:
    lw $t0, 8($sp)
    move $v0, $t0
    lw $ra, 28($sp)
    addiu $sp, $sp, 32
    jr $ra
    j label3
label2:
    li $t0, 1
    sw $t0, 12($sp)
    lw $t0, 8($sp)
    lw $t1, 12($sp)
    subu $t0, $t0, $t1
    sw $t0, 16($sp)
    lw $t4, 16($sp)
    addiu $sp, $sp, -4
    sw $ra, 0($sp)
    move $a0, $t4
    jal fact
    lw $ra, 0($sp)
    addiu $sp, $sp, 4
    sw $v0, 20($sp)
    lw $t0, 8($sp)
    lw $t1, 20($sp)
    mul $t0, $t0, $t1
    sw $t0, 24($sp)
    lw $t0, 24($sp)
    move $v0, $t0
    lw $ra, 28($sp)
    addiu $sp, $sp, 32
    jr $ra
label3:

main:
    addiu $sp, $sp, -40
    sw $ra, 36($sp)
    addiu $sp, $sp, -4
    sw $ra, 0($sp)
    jal read
    lw $ra, 0($sp)
    addiu $sp, $sp, 4
    sw $v0, 0($sp)
    lw $t0, 0($sp)
    sw $t0, 4($sp)
    li $t0, 1
    sw $t0, 8($sp)
    lw $t0, 4($sp)
    lw $t1, 8($sp)
    bgt $t0, $t1, label4
    j label5
label4:
    lw $t4, 4($sp)
    addiu $sp, $sp, -4
    sw $ra, 0($sp)
    move $a0, $t4
    jal fact
    lw $ra, 0($sp)
    addiu $sp, $sp, 4
    sw $v0, 16($sp)
    lw $t0, 16($sp)
    sw $t0, 20($sp)
    j label6
label5:
    li $t0, 1
    sw $t0, 24($sp)
    lw $t0, 24($sp)
    sw $t0, 20($sp)
label6:
    lw $t0, 20($sp)
    move $a0, $t0
    addiu $sp, $sp, -4
    sw $ra, 0($sp)
    jal write
    lw $ra, 0($sp)
    addiu $sp, $sp, 4
    li $t0, 0
    sw $t0, 28($sp)
    lw $t0, 28($sp)
    move $v0, $t0
    lw $ra, 36($sp)
    addiu $sp, $sp, 40
    jr $ra

