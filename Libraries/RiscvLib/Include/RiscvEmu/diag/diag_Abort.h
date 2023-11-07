#pragma once
#include <RiscvEmu/diag/detail/diag_AbortImpl.h>
#include <RiscvEmu/diag/diag_FormatString.h>

namespace riscv {
namespace diag {

[[noreturn]] void Abort(const std::source_location& location = std::source_location::current());

template<typename... Args>
[[noreturn]] void Abort(const FormatString& format, Args&&... args) {
    detail::AbortWithMessageImpl(stderr, format.location, format.format, std::forward<Args>(args)...);
}

[[noreturn]] void UnexpectedDefault(const std::source_location& location = std::source_location::current());

template<typename... Args>
[[noreturn]] void UnexpectedDefault(const FormatString& format, Args&&... args) {
    detail::UnexpectedDefaultWithMessageImpl(stderr, format.location, format.format, std::forward<Args>(args)...);
}

} // namespace diag
} // namespace riscv
