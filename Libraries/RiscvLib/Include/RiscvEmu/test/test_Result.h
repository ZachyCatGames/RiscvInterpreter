#pragma once
#include <RiscvEmu/result.h>

/**
 * The test library is not a part of the main source tree, however it does
 * have its own result module id. We reserve it here.
*/

namespace riscv {
namespace test {

namespace detail {

constexpr inline auto ModuleId = 255;

} // namespace detail

} // namespace test
} // namespace riscv