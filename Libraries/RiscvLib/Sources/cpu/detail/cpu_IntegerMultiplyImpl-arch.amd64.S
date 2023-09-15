.section .text

.global __riscvCpuMultiply64Impl
.global __riscvCpuMultiplyGetLower64Impl
.global __riscvCpuMultiplyGetUpper64Impl

#define POUT_REG %rdi

#define VAL0_REG %rsi
#define VAL1_REG %rdx

__riscvCpuMultiply64Impl:
    mov VAL0_REG, %rax
    mulq VAL1_REG
    mov %rdx, (POUT_REG)
    ret

__riscvCpuMultiplyGetLower64Impl:
    mov VAL0_REG, %rax
    mulq VAL1_REG
    mov %rdx, %rax
    ret

__riscvCpuMultiplyGetUpper64Impl:
    mov VAL0_REG, %rax
    mulq VAL1_REG
    ret
