#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/diag.h>

namespace riscv {
namespace cpu {

void Hart::SharedState::Initialize(Word hartCount, mem::MemoryController* pMemCtlr) noexcept {
    /* Assert memory controller isn't null. */
    diag::AssertNotNull(pMemCtlr);

    m_HartCount = hartCount;
    m_pMemCtlr = pMemCtlr;

    /* Init memory monitor. */
    m_MemMonitor.Initialize(hartCount);
}

mem::MemoryController* Hart::SharedState::GetMemController() noexcept { return m_pMemCtlr; }

detail::MemoryMonitor* Hart::SharedState::GetMemMonitor() noexcept { return &m_MemMonitor; }

} // namespace cpu
} // namespace riscv