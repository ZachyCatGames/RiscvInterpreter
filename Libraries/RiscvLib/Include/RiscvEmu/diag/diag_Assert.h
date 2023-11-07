#pragma once
#include <RiscvEmu/diag/diag_FormatString.h>
#include <RiscvEmu/diag/detail/diag_AssertImpl.h>
#include <source_location>
#include <string_view>
#include <type_traits>

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

template<typename T>
requires (std::is_pointer_v<T> || std::is_function_v<T> || std::is_member_function_pointer_v<T>)
constexpr void AssertNotNull(T p, const std::source_location& location = std::source_location::current()) {
    if(p == nullptr) {
        detail::AssertWithMessageImpl(stderr, location, "p == nullptr");
    }
}

template<typename T>
requires (std::is_pointer_v<T> || std::is_function_v<T> || std::is_member_function_pointer_v<T>)
constexpr void AssertNull(T p, const std::source_location& location = std::source_location::current()) {
    if(p != nullptr) {
        detail::AssertWithMessageImpl(stderr, location, "p != nullptr");
    }
}

} // namespace diag
} // namespace riscv
