#pragma once
#include <RiscvEmu/result.h>

namespace riscv {
namespace mem {

namespace detail {

constexpr inline Word ModuleId = 2;

} // namespace detail

class ResultBadMisalignedAddress : public result::ErrorBase<detail::ModuleId, 1> {};

} // namespace mem
} // namespace riscv
