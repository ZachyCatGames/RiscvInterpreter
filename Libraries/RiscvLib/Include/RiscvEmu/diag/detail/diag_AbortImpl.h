#pragma once
#include <cstdio>
#include <source_location>
#include <string_view>

namespace riscv {
namespace diag {
namespace detail {

void AbortImpl();

void AbortNoMessageImpl(FILE* stream, const std::source_location& location);

void AbortWithMessageImpl(FILE* stream, const std::source_location& location, std::string_view format, ...);

void UnexpectedDefaultNoMessageImpl(FILE* stream, const std::source_location location);

void UnexpectedDefaultWithMessageImpl(FILE* stream, const std::source_location& location, std::string_view format, ...);

} // namespace detail
} // namespace diag
} // namespace riscv
