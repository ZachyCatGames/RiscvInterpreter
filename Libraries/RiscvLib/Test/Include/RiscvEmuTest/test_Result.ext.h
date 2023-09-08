#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/test/test_Result.h>

namespace riscv {
namespace test {

class ResultRegValMismatch : public result::ErrorBase<detail::ModuleId, 1> {};

} // namespace test
} // namespace riscv
