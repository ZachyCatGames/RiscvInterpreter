#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/test/test_Result.h>

namespace riscv {
namespace test {

class ResultRegValMismatch : public result::ErrorBase<detail::ModuleId, 1> {};

class ResultMemValMismatch : public result::ErrorBase<detail::ModuleId, 2> {};

} // namespace test
} // namespace riscv
