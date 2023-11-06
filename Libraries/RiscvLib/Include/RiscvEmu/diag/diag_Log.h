#pragma once
#include <cstdio>
#include <string_view>

namespace riscv {
namespace diag {

template<typename... Args>
void Log(std::string_view format, Args&&... args) {
    std::printf(format, std::forward<Args...>(args)...);
}

} // namespace diag
} // namespace riscv
