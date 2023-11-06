#pragma once
#include <RiscvEmu/diag/detail/diag_AbortImpl.h>

namespace riscv {
namespace diag {

namespace detail {

template<typename... Args>
struct AbortMsgImpl {
    AbortMsgImpl(std::string_view format, Args&&... args, std::source_location location = std::source_location::current()) {
        detail::AbortWithMessageImpl(stderr, location, format, std::forward<Args...>(args)...);
    }
}; // struct AbortMsgImpl

struct AbortNoMsgImpl {
    AbortNoMsgImpl(std::source_location location = std::source_location::current()) {
        detail::AbortNoMessageImpl(stderr, location);
    }
}; // struct AbortNoMsgImpl

template<typename... Args>
struct UnexpectedDefaultMsgImpl {
    UnexpectedDefaultMsgImpl(std::string_view format, Args&&... args, std::source_location location = std::source_location::current()) {
        detail::UnexpectedDefaultWithMessageImpl(stderr, location, format, std::forward<Args...>(args)...);
    }
}; // struct UnexpectedDefaultMsgImpl

struct UnexpectedDefaultNoMsgImpl {
    UnexpectedDefaultNoMsgImpl(std::source_location location = std::source_location::current()) {
        detail::UnexpectedDefaultNoMessageImpl(stderr, location);
    }
}; // struct UnexpectedDefaultNoMsgImpl

} // namespace detail

template<typename... Args>
struct Abort : detail::AbortMsgImpl<Args...> {
    using detail::AbortMsgImpl<Args...>::AbortMsgImpl;
}; // struct Abort

template<>
struct Abort<> : detail::AbortMsgImpl<>, detail::AbortNoMsgImpl {
    using detail::AbortMsgImpl<>::AbortMsgImpl;
    using detail::AbortNoMsgImpl::AbortNoMsgImpl;
}; // struct Abort<>

template<typename... Args>
Abort(std::string_view format, Args&&... args) -> Abort<Args...>;

Abort() -> Abort<>;

template<typename... Args>
struct UnexpectedDefault : detail::UnexpectedDefaultMsgImpl<Args...> {
    using detail::UnexpectedDefaultMsgImpl<Args...>::UnexpectedDefaultMsgImpl;
}; // struct UnexpectedDefault

template<>
struct UnexpectedDefault<> : detail::UnexpectedDefaultMsgImpl<>, detail::UnexpectedDefaultNoMsgImpl {
    using detail::UnexpectedDefaultMsgImpl<>::UnexpectedDefaultMsgImpl;
    using detail::UnexpectedDefaultNoMsgImpl::UnexpectedDefaultNoMsgImpl;
}; // struct UnexpectedDefault<>

template<typename... Args>
UnexpectedDefault(std::string_view format, Args&&... args) -> UnexpectedDefault<Args...>;

UnexpectedDefault() -> UnexpectedDefault<>;

} // namespace diag
} // namespace riscv
