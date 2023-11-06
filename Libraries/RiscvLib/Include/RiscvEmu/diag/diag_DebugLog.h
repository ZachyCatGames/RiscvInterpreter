#pragma once
#include <RiscvEmu/diag/detail/diag_DebugLogImpl.h>
#include <source_location>
#include <string_view>

namespace riscv {
namespace diag {

#ifdef NDEBUG

template<typename... Args>
void DebugLog(std::string_view, Args&&...) { }

#else

template<typename... Args>
struct DebugLog {
    DebugLog(std::string_view format, Args&&... args, std::source_location location = std::source_location::current()) {
        detail::DebugPrintImpl(stdout, location, format, std::forward<Args...>(args)...);
    }
}; // struct DebugLog

template<typename... Args>
DebugLog(std::string_view format, Args&&... args) -> DebugLog<Args...>;

#endif

} // namespace diag
} // namespace riscv
