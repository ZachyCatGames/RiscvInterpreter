#pragma once
#include <RiscvEmu/result.h>

namespace riscv {
namespace cpu {

namespace detail {

constexpr inline Word ModuleId = 1;

} // namespace detail

class ResultInvalidInstruction : public result::ErrorBase<detail::ModuleId, 1> {};

class ResultNotImplemented : public result::ErrorBase<detail::ModuleId, 2> {};

} // namespace cpu
} // namespace riscv
