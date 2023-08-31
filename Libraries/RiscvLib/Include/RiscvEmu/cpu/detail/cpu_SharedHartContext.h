#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/mem/mem_MemoryController.h>

namespace riscv {
namespace cpu {
namespace detail {

class SharedHartContext {
public:
    constexpr auto GetMemoryController() noexcept {
        return &m_MemCtlr;
    }
    constexpr auto GetMemoryController() const noexcept {
        return &m_MemCtlr;
    }
private:
    mem::MemoryController m_MemCtlr;
}; // SharedHartContext

} // namespace detail
} // namespace cpu
} // namespace riscv