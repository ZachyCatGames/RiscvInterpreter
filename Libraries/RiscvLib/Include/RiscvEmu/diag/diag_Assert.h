#pragma once
#include <RiscvEmu/diag/detail/diag_AssertImpl.h>
#include <source_location>
#include <string_view>

namespace riscv {
namespace diag {

template<typename... Args>
struct Assert {
    Assert(bool cond, std::string_view format, Args&&... args, std::source_location location = std::source_location::current()) {
        detail::AssertWithMessageImpl(stderr, cond, location, format, std::forward<Args...>(args)...);
    }

    Assert(bool cond, std::source_location location = std::source_location::current()) {
        detail::AssertNoMessageImpl(stderr, cond, location);
    }
}; // struct Assert

template<typename... Args>
Assert(bool cond, std::string_view format, Args&&... args) -> Assert<Args...>;

template<typename... Args>
Assert(bool cond) -> Assert<>;

} // namespace diag
} // namespace riscv
