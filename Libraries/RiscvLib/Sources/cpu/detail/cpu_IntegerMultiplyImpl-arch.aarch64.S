.section .text

.global __riscvCpuMultiply64Impl
.global __riscvCpuMultiplyGetLower64Impl
.global __riscvCpuMultiplyGetUpper64Impl

__riscvCpuMultiply64Impl:
    umulh   x4, x1, x2
    mul     x3, x1, x2
    str     x4, [x0]
    mov     x0, x4
    ret

__riscvCpuMultiplyGetLower64Impl:
    mul     x0, x1, x2
    ret

__riscvCpuMultiplyGetUpper64Impl:
    umulh   x0, x1, x2
    ret


