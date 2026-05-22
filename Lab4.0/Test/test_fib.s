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

main:
    addiu $sp, $sp, -64
    sw $ra, 60($sp)
    li $t0, 0
    sw $t0, 0($sp)
    lw $t0, 0($sp)
    sw $t0, 8($sp)
    li $t0, 1
    sw $t0, 12($sp)
    lw $t0, 12($sp)
    sw $t0, 20($sp)
    li $t0, 0
    sw $t0, 24($sp)
    lw $t0, 24($sp)
    sw $t0, 28($sp)
    addiu $sp, $sp, -4
    sw $ra, 0($sp)
    jal read
    lw $ra, 0($sp)
    addiu $sp, $sp, 4
    sw $v0, 32($sp)
    lw $t0, 32($sp)
    sw $t0, 36($sp)
label1:
    lw $t0, 28($sp)
    lw $t1, 36($sp)
    blt $t0, $t1, label2
    j label3
label2:
    lw $t0, 8($sp)
    lw $t1, 20($sp)
    addu $t0, $t0, $t1
    sw $t0, 40($sp)
    lw $t0, 40($sp)
    sw $t0, 44($sp)
    lw $t0, 20($sp)
    move $a0, $t0
    addiu $sp, $sp, -4
    sw $ra, 0($sp)
    jal write
    lw $ra, 0($sp)
    addiu $sp, $sp, 4
    lw $t0, 20($sp)
    sw $t0, 8($sp)
    lw $t0, 44($sp)
    sw $t0, 20($sp)
    li $t0, 1
    sw $t0, 48($sp)
    lw $t0, 28($sp)
    lw $t1, 48($sp)
    addu $t0, $t0, $t1
    sw $t0, 52($sp)
    lw $t0, 52($sp)
    sw $t0, 28($sp)
    j label1
label3:
    li $t0, 0
    sw $t0, 56($sp)
    lw $t0, 56($sp)
    move $v0, $t0
    lw $ra, 60($sp)
    addiu $sp, $sp, 64
    jr $ra

