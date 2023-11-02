#pragma once
#include <RiscvEmu/result.h>

namespace riscv {
namespace intrpt {

namespace detail {

constexpr inline Word ModuleId = 3;

} // namespace detail

/* PLIC errors. */
class ResultPLICInvalidSourceCount : public result::ErrorBase<detail::ModuleId, 100> {};
class ResultPLICInvalidTargetCount : public result::ErrorBase<detail::ModuleId, 101> {};
class ResultPLICSourceAlreadyTaken : public result::ErrorBase<detail::ModuleId, 102> {};
class ResultPLICTargetAlreadyTaken : public result::ErrorBase<detail::ModuleId, 103> {};

/* Target results. */
class ResultTargetIntrptPending : public result::ErrorBase<detail::ModuleId, 1000> {};
class ResultTargetIntrptAccept  : public result::ErrorBase<detail::ModuleId, 1001> {};

} // namespace intrpt
} // namespace riscv
