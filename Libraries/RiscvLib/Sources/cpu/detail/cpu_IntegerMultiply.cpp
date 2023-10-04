#include <RiscvEmu/cpu/detail/cpu_IntegerMultiply.h>

extern "C" uint64_t __riscvCpuMultiply64UImpl(uint64_t* pUpper, uint64_t val1, uint64_t val2);
extern "C" int64_t  __riscvCpuMultiply64SImpl(int64_t* pUpper, int64_t val1, int64_t val2);

extern "C" uint64_t  __riscvCpuMultiplyGetUpper64UImpl(uint64_t val1, uint64_t val2);
extern "C" int64_t __riscvCpuMultiplyGetUpper64SImpl(int64_t val1, int64_t val2);

namespace riscv {
namespace cpu {
namespace detail {

uint64_t Multiply64U(uint64_t* pUpper, uint64_t val1, uint64_t val2) {
    return __riscvCpuMultiply64UImpl(pUpper, val1, val2);
}

int64_t Multiply64S(int64_t* pUpper, int64_t val1, int64_t val2) {
    return __riscvCpuMultiply64SImpl(pUpper, val1, val2);
}

uint64_t MultiplyGetUpper64U(uint64_t val1, uint64_t val2) {
    return __riscvCpuMultiplyGetUpper64UImpl(val1, val2);
}

int64_t MultiplyGetUpper64S(int64_t val1, int64_t val2) {
    return __riscvCpuMultiplyGetUpper64SImpl(val1, val2);
}

int64_t MultiplyGetUpper64SU(int64_t val1, uint64_t val2) {
    uint64_t upper = MultiplyGetUpper64U(val1 < 0 ? static_cast<uint64_t>(-val1) : static_cast<uint64_t>(val1), val2);
    return static_cast<int64_t>(val1 < 0 ? ~upper + (val1 * static_cast<int64_t>(val2) == 0) : upper);
}

} // namespace detail
} // namespace cpu
} // namespace riscv
