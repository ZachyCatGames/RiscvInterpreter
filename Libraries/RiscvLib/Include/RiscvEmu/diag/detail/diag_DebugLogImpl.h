#pragma once
#include <cstdio>
#include <source_location>
#include <string_view>

namespace riscv {
namespace diag {
namespace detail {

void DebugPrintImpl(FILE* stream, const std::source_location& location, std::string_view format, ...);

} // namespace detail
} // namespace diag
} // namespace riscv
