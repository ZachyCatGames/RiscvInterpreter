#pragma once
#include <RiscvEmu/diag/detail/diag_LogImpl.h>
#include <string_view>

namespace riscv {
namespace diag {

template<typename... Args>
constexpr void Log(std::string_view format, Args&&... args) {
    if(!std::is_constant_evaluated()) {
        detail::LogImpl(stdout, format, std::forward<Args>(args)...);
    }
}

} // namespace diag
} // namespace riscv
