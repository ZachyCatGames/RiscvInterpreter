#pragma once
#include <cstdio>
#include <source_location>
#include <string_view>

namespace riscv {
namespace diag {
namespace detail {

void AssertNoMessageImpl(FILE* stream, const std::source_location& location);

void AssertWithMessageImpl(FILE* stream, const std::source_location& location, std::string_view format, ...);

} // namespace detail
} // namespace diag
} // namespace riscv
