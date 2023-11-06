#pragma once
#include <cstdio>
#include <source_location>
#include <string_view>

namespace riscv {
namespace diag {
namespace detail {

[[noreturn]] void AbortImpl();

[[noreturn]] void AbortNoMessageImpl(FILE* stream, const std::source_location& location);

[[noreturn]] void AbortWithMessageImpl(FILE* stream, const std::source_location& location, std::string_view format, ...);

[[noreturn]] void UnexpectedDefaultNoMessageImpl(FILE* stream, const std::source_location location);

[[noreturn]] void UnexpectedDefaultWithMessageImpl(FILE* stream, const std::source_location& location, std::string_view format, ...);

} // namespace detail
} // namespace diag
} // namespace riscv
