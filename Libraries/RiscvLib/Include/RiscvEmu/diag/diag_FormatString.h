#pragma once
#include <source_location>
#include <string_view>

namespace riscv {
namespace diag {

struct FormatString {
    constexpr FormatString(const char* format, const std::source_location& location = std::source_location::current()) :
        format(format), location(location) {}

    constexpr FormatString(std::string_view format, const std::source_location& location = std::source_location::current()) :
        format(format), location(location) {}

    std::string_view format;
    std::source_location location;
}; // struct FormatString

} // namespace diag
} // namespace riscv
