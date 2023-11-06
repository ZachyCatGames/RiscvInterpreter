#pragma once
#include <cstdarg>
#include <cstdio>
#include <source_location>
#include <string_view>

namespace riscv {
namespace diag {
namespace detail {

void PrintSourceLocation(FILE* stream, std::string_view logType, const std::source_location& location);

void PrintMessageWithSourceLocation(FILE* stream, std::string_view logType, const std::source_location& location, std::string_view format, va_list formatList);

} // namespace detail
} // namespace diag
} // namespace riscv
