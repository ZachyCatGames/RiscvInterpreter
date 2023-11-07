#pragma once
#include <RiscvEmu/diag/diag_FormatString.h>
#include <RiscvEmu/diag/detail/diag_AssertImpl.h>
#include <source_location>
#include <string_view>
#include <type_traits>

namespace riscv {
namespace diag {

namespace detail {

template<typename T>
concept Pointer = std::is_pointer_v<T> || std::is_function_v<T> || std::is_member_function_pointer_v<T>;

}

#ifdef NDEBUG

constexpr void Assert(bool) {}
constexpr void Assert(bool, const std::source_location&) {}
template<typename... Args>
constexpr void Assert(bool, std::string_view, Args&&...) {}

template<typename T>
requires detail::Pointer<T>
constexpr void AssertNotNull(T) {}
template<typename T>
requires detail::Pointer<T>
constexpr void AssertNotNull(T, const std::source_location&) {}

template<typename T>
requires detail::Pointer<T>
constexpr void AssertNull(T) {}
template<typename T>
requires detail::Pointer<T>
constexpr void AssertNull(T, const std::source_location&) {}

#else

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
requires detail::Pointer<T>
constexpr void AssertNotNull(T p, const std::source_location& location = std::source_location::current()) {
    if(p == nullptr) {
        detail::AssertWithMessageImpl(stderr, location, "p == nullptr");
    }
}

template<typename T>
requires detail::Pointer<T>
constexpr void AssertNull(T p, const std::source_location& location = std::source_location::current()) {
    if(p != nullptr) {
        detail::AssertWithMessageImpl(stderr, location, "p != nullptr");
    }
}

#endif

} // namespace diag
} // namespace riscv
