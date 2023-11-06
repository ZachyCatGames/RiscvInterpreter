#pragma once
#include <RiscvEmu/diag/diag_FormatString.h>
#include <RiscvEmu/diag/detail/diag_AssertImpl.h>
#include <source_location>
#include <string_view>

namespace riscv {
namespace diag {

constexpr void Assert(bool cond, const std::source_location& location = std::source_location::current()) {
    if(!cond) {
        detail::AssertNoMessageImpl(stderr, location);
    }
}

template<typename... Args>
constexpr void Assert(bool cond, const FormatString& format, Args&&... args) {
    if(!cond) {
        detail::AssertWithMessageImpl(stderr, format.location, format.format, std::forward<Args...>(args)...);
    }
}

} // namespace diag
} // namespace riscv
