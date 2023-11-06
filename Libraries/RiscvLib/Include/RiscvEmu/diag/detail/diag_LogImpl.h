#pragma once
#include <string_view>

namespace riscv {
namespace diag {
namespace detail {

void LogImpl(FILE* stream, std::string_view format, ...);

} // namespace detail
} // namespace diag
} // namespace riscv
