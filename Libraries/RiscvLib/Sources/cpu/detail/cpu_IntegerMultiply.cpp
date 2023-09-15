#include <RiscvEmu/cpu/detail/cpu_IntegerMultiply.h>

extern "C" uint64_t __riscvCpuMultiply64Impl(uint64_t* pOut, uint64_t val1, uint64_t val2);
extern "C" uint64_t __riscvCpuMultiplyGetLower64Impl(uint64_t val1, uint64_t val2);
extern "C" uint64_t __riscvCpuMultiplyGetUpper64Impl(uint64_t val1, uint64_t val2);

namespace riscv {
namespace cpu {
namespace detail {

uint64_t Multiply64(uint64_t* pLower, uint64_t val1, uint64_t val2) {
    return __riscvCpuMultiply64Impl(pLower, val1, val2);
}

uint64_t MultiplyGetLower64(uint64_t val1, uint64_t val2) {
    return __riscvCpuMultiplyGetLower64Impl(val1, val2);
}

uint64_t MultiplyGetUpper64(uint64_t val1, uint64_t val2) {
    return __riscvCpuMultiplyGetUpper64Impl(val1, val2);
}

} // namespace detail
} // namespace cpu
} // namespace riscv
