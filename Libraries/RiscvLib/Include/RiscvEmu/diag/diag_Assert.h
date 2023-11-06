#pragma once
#include <RiscvEmu/diag/detail/diag_AssertImpl.h>
#include <source_location>
#include <string_view>

namespace riscv {
namespace diag {

namespace detail {

template<typename... Args>
struct AssertImpl {
    AssertImpl() = default;
    AssertImpl(bool cond, std::string_view format, Args&&... args, std::source_location location = std::source_location::current()) {
        detail::AssertWithMessageImpl(stderr, cond, location, format, std::forward<Args...>(args)...);
    }
}; // struct AssertImpl

struct AssertNoMsgImpl {
    AssertNoMsgImpl() = default;
    AssertNoMsgImpl(bool cond, std::source_location location = std::source_location::current()) {
        detail::AssertNoMessageImpl(stderr, cond, location);
    }
}; // struct AssertNoMsgImpl

} // namespace detail

template<typename... Args>
struct Assert : detail::AssertImpl<Args...> {
    using detail::AssertImpl<Args...>::AssertImpl;
}; // struct Assert

template<>
struct Assert<> : detail::AssertImpl<>, detail::AssertNoMsgImpl {
    using detail::AssertImpl<>::AssertImpl;
    using detail::AssertNoMsgImpl::AssertNoMsgImpl;
}; // struct Assert<>

template<typename... Args>
Assert(bool cond, std::string_view format, Args&&... args) -> Assert<Args...>;

template<typename... Args>
Assert(bool cond) -> Assert<>;

} // namespace diag
} // namespace riscv
