.section .text

.global __riscvCpuMultiply64UImpl
.global __riscvCpuMultiply64SImpl

.global __riscvCpuMultiplyGetUpper64UImpl
.global __riscvCpuMultiplyGetUpper64SImpl

#define POUT_REG %rdi

#define VAL0_REG %rsi
#define VAL1_REG %rdx

__riscvCpuMultiply64UImpl:
    mov VAL0_REG, %rax
    mulq VAL1_REG
    mov %rdx, (POUT_REG)
    ret

__riscvCpuMultiply64SImpl:
    mov VAL0_REG, %rax
    imulq VAL1_REG
    mov %rdx, (POUT_REG)
    ret

__riscvCpuMultiplyGetUpper64UImpl:
    mov VAL0_REG, %rax
    mulq VAL1_REG
    mov %rdx, %rax
    ret

__riscvCpuMultiplyGetUpper64SImpl:
    mov VAL0_REG, %rax
    imulq VAL1_REG
    mov %rdx, %rax
    ret
