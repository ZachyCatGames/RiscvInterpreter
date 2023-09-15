#pragma once
#include <cstdint>

namespace riscv {
namespace cpu {
namespace detail {

uint64_t Multiply64(uint64_t* pLower, uint64_t val1, uint64_t val2);

uint64_t MultiplyGetLower64(uint64_t val1, uint64_t val2);
uint64_t MultiplyGetUpper64(uint64_t val1, uint64_t val2);

} // namespace detail
} // namespace cpu
} // namespace riscv
