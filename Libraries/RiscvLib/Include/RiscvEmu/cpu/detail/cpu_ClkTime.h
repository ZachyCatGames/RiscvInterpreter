#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/result.h>

namespace riscv {
namespace cpu {
namespace detail {

class ClkTime {
public:
    Result Initialize();

    Result GetCurrentTime(DWord* pOut);

    Result SetCurrentTime(DWord in);
private:
    DWord m_Offset;
}; // class ClkTime

} // namespace detail
} // namespace cpu
} // namespace riscv