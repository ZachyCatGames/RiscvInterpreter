#pragma once
#include <cstdint>

namespace riscv {
namespace cpu {
namespace detail {

uint64_t Multiply64U(uint64_t* pUpper, uint64_t val1, uint64_t val2);
int64_t  Multiply64S(int64_t* pUpper, int64_t val1, int64_t val2);

uint64_t MultiplyGetUpper64U (uint64_t val1, uint64_t val2);
int64_t  MultiplyGetUpper64S (int64_t val1, int64_t val2);
int64_t  MultiplyGetUpper64SU(int64_t val1, uint64_t val2);

} // namespace detail
} // namespace cpu
} // namespace riscv
