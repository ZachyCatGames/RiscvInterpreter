#pragma once
#include <RiscvEmu/diag/diag_FormatString.h>
#include <RiscvEmu/diag/detail/diag_DebugLogImpl.h>
#include <source_location>
#include <string_view>

namespace riscv {
namespace diag {

#ifdef NDEBUG

template<typename... Args>
constexpr void DebugLog(std::string_view, Args&&...) { }

#else

template<typename... Args>
constexpr void DebugLog(const FormatString& format, Args&&... args) {
    if(!std::is_constant_evaluated()) {
        detail::DebugPrintImpl(stdout, format.location, format.format, std::forward<Args...>(args)...);
    }
}

#endif

} // namespace diag
} // namespace riscv
