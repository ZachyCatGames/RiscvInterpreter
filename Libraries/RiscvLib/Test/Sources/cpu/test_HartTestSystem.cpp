#include <RiscvEmuTest/cpu/test_HartTestSystem.h>

namespace riscv {
namespace test {

Result HartTestSystem::Initialize() {
    /* Initialize the Hart. */
    m_HartSharedState.Initialize(1, &m_MemCtlr);
    Result res = m_Hart.Initialize(&m_HartSharedState, 0);
    if(res.IsFailure()) {
        return res;
    }

    /* Reset the Hart. */
    res = m_Hart.Reset();
    if(res.IsFailure()) {
        return res;
    }

    static constexpr mem::RegionInfo memReg{ MemoryAddress, MemorySize, mem::RegionType::Memory };
    res = m_MemCtlr.Initialize(&memReg, 1);
    if(res.IsFailure()) {
        return res;
    }

    return ResultSuccess();
}

Result HartTestSystem::DefaultReset(HartTestSystem* pSys) {
    return pSys->GetHart()->Reset();
}

void HartTestSystem::ClearGPRs() noexcept {
    /* Write zero to each GPR. */
    for(int i = 0; i < cpu::Hart::NumGPR; i++) {
        this->WriteGPR(i, 0);
    }
}

Result HartTestSystem::ClearMem() {
    /* Memory size must be aligned to 8 bytes. */
    static_assert(MemoryAddress + MemorySize % 8);

    /* Write zero to entirety of ram. */
    Address curAddr = MemoryAddress;
    while(curAddr < MemoryAddress + MemorySize) {
        Result res = this->MemWriteDWord(0, curAddr);
        if(res.IsFailure()) {
            return res;
        }

        curAddr += 8;
    }

    return ResultSuccess();
}

} // namespace test
} // namespace riscv
