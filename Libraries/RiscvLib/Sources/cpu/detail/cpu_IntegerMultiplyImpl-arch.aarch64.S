.section .text

.global __riscvCpuMultiply64UImpl
.global __riscvCpuMultiply64SImpl

.global __riscvCpuMultiplyGetUpper64UImpl
.global __riscvCpuMultiplyGetUpper64SImpl

__riscvCpuMultiply64UImpl:
    umulh   x4, x1, x2
    mul     x3, x1, x2
    str     x4, [x0]
    mov     x0, x4
    ret

__riscvCpuMultiply64SImpl:
    smulh   x4, x1, x2
    mul     x3, x1, x2
    str     x4, [x0]
    mov     x0, x4
    ret

__riscvCpuMultiplyGetUpper64UImpl:
    umulh   x0, x1, x2
    ret

__riscvCpuMultiplyGetUpper64SImpl:
    smulh   x0, x1, x2
    ret
